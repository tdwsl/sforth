#include "sforth.h"
#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum {
	FTH_PUSH,
	FTH_PUSHB,
	FTH_PUSHSTR,
	FTH_PUSHPTR,
	FTH_CALL,
	FTH_RET,
	FTH_HERE,
	FTH_DSIZE,
	FTH_RPUSH,
	FTH_RPOP,
	FTH_RDUP,
	FTH_RDROP,
	FTH_RSET,
	FTH_JUMP,
	FTH_JZ,
	FTH_ADDINS,
	FTH_ADDVAL,
	FTH_ADDRETVAL,
	FTH_DROP,
	FTH_DUP,
	FTH_SWAP,
	FTH_ROT,
	FTH_OVER,
	FTH_DEPTH,
	FTH_PICK,
	FTH_RPICK,
	FTH_PLUS,
	FTH_MINUS,
	FTH_INC,
	FTH_DEC,
	FTH_MUL,
	FTH_DIV,
	FTH_MOD,
	FTH_EQUAL,
	FTH_NEQUAL,
	FTH_GREATER,
	FTH_LESS,
	FTH_AND,
	FTH_OR,
	FTH_XOR,
	FTH_INV,
	FTH_EMIT,
	FTH_TYPE,
	FTH_PRINTNUM,
	FTH_IMMEDIATE,
	FTH_INSERT,
	FTH_KEY,
	FTH_ACCEPT,
};


enum {
	FTHWORD_NORMAL,
	FTHWORD_IMMEDIATE,
	FTHWORD_INSERT,
};

enum {
	FTHMODE_RUN,
	FTHMODE_WORD,
	FTHMODE_WORDNAME,
	FTHMODE_QUOTE,
	FTHMODE_SQUOTE,
};

void fth_addWord(Forth *fth, const char *name, char type) {
	fth->num_words++;
	fth->words = (struct forthWord*)realloc(fth->words, sizeof(struct forthWord)*fth->num_words);
	char *s = malloc(strlen(name)+1);
	strcpy(s, name);
	fth->words[fth->num_words-1] = (struct forthWord){ s, type, fth->size };
}

void fth_dictSize(Forth *fth, size_t size) {
	fth->size = size;
	if(fth->size > fth->max_size - 10) {
		fth->max_size += 10;
		fth->dict = (char*)realloc(fth->dict, fth->max_size);
	}
}

void fth_addIns(Forth *fth, char ins) {
	fth_dictSize(fth, fth->size+1);
	fth->dict[fth->size-1] = ins;
}

void fth_addVal(Forth *fth, void *val) {
	fth_dictSize(fth, fth->size+sizeof(void*));
	*(void**)(fth->dict+fth->size-sizeof(void*)) = val;
}

Forth *newForth() {
	Forth *fth = (Forth*)malloc(sizeof(Forth));
	fth->words = 0;
	fth->num_words = 0;
	fth->dict = 0;
	fth->size = 0;
	fth->old_size = 0;
	fth->max_size = 0;
	fth->sp = 0;
	fth->rsp = 0;
	fth->mode = FTHMODE_RUN;
	fth->quit = 0;
	fth->base = (void*)10;

	fth_addWord(fth, "+", FTHWORD_INSERT);
	fth_addIns(fth, FTH_PLUS);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "-", FTHWORD_INSERT);
	fth_addIns(fth, FTH_MINUS);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "1+", FTHWORD_INSERT);
	fth_addIns(fth, FTH_INC);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "1-", FTHWORD_INSERT);
	fth_addIns(fth, FTH_DEC);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "*", FTHWORD_INSERT);
	fth_addIns(fth, FTH_MUL);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "/", FTHWORD_INSERT);
	fth_addIns(fth, FTH_DIV);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "MOD", FTHWORD_INSERT);
	fth_addIns(fth, FTH_MOD);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "DUP", FTHWORD_INSERT);
	fth_addIns(fth, FTH_DUP);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "OVER", FTHWORD_INSERT);
	fth_addIns(fth, FTH_OVER);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "SWAP", FTHWORD_INSERT);
	fth_addIns(fth, FTH_SWAP);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "ROT", FTHWORD_INSERT);
	fth_addIns(fth, FTH_ROT);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "DROP", FTHWORD_INSERT);
	fth_addIns(fth, FTH_DROP);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "2DROP", FTHWORD_INSERT);
	fth_addIns(fth, FTH_DROP);
	fth_addIns(fth, FTH_DROP);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "2DUP", FTHWORD_INSERT);
	fth_addIns(fth, FTH_OVER);
	fth_addIns(fth, FTH_OVER);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "2SWAP", FTHWORD_INSERT);
	fth_addIns(fth, FTH_ROT);
	fth_addIns(fth, FTH_RPUSH);
	fth_addIns(fth, FTH_ROT);
	fth_addIns(fth, FTH_RPOP);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "2OVER", FTHWORD_INSERT);
	fth_addIns(fth, FTH_PUSHB);
	fth_addIns(fth, 3);
	fth_addIns(fth, FTH_PICK);
	fth_addIns(fth, FTH_PUSHB);
	fth_addIns(fth, 3);
	fth_addIns(fth, FTH_PICK);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "DEPTH", FTHWORD_INSERT);
	fth_addIns(fth, FTH_DEPTH);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "PICK", FTHWORD_INSERT);
	fth_addIns(fth, FTH_PICK);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, ">R", FTHWORD_INSERT);
	fth_addIns(fth, FTH_RPUSH);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "R>", FTHWORD_INSERT);
	fth_addIns(fth, FTH_RPOP);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "HERE", FTHWORD_INSERT);
	fth_addIns(fth, FTH_HERE);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "AND", FTHWORD_INSERT);
	fth_addIns(fth, FTH_AND);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "OR", FTHWORD_INSERT);
	fth_addIns(fth, FTH_OR);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "XOR", FTHWORD_INSERT);
	fth_addIns(fth, FTH_XOR);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "INV", FTHWORD_INSERT);
	fth_addIns(fth, FTH_INV);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "=", FTHWORD_INSERT);
	fth_addIns(fth, FTH_EQUAL);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "<>", FTHWORD_INSERT);
	fth_addIns(fth, FTH_NEQUAL);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, ">", FTHWORD_INSERT);
	fth_addIns(fth, FTH_GREATER);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "<", FTHWORD_INSERT);
	fth_addIns(fth, FTH_LESS);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, ">=", FTHWORD_INSERT);
	fth_addIns(fth, FTH_DEC);
	fth_addIns(fth, FTH_GREATER);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "<=", FTHWORD_INSERT);
	fth_addIns(fth, FTH_INC);
	fth_addIns(fth, FTH_LESS);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "0=", FTHWORD_INSERT);
	fth_addIns(fth, FTH_PUSHB);
	fth_addIns(fth, 0);
	fth_addIns(fth, FTH_EQUAL);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "0<>", FTHWORD_INSERT);
	fth_addIns(fth, FTH_PUSHB);
	fth_addIns(fth, 0);
	fth_addIns(fth, FTH_EQUAL);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "0<", FTHWORD_INSERT);
	fth_addIns(fth, FTH_PUSHB);
	fth_addIns(fth, 0);
	fth_addIns(fth, FTH_LESS);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "0>", FTHWORD_INSERT);
	fth_addIns(fth, FTH_PUSHB);
	fth_addIns(fth, 0);
	fth_addIns(fth, FTH_GREATER);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "0<=", FTHWORD_INSERT);
	fth_addIns(fth, FTH_PUSHB);
	fth_addIns(fth, 1);
	fth_addIns(fth, FTH_LESS);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "0>=", FTHWORD_INSERT);
	fth_addIns(fth, FTH_PUSHB);
	fth_addIns(fth, -1);
	fth_addIns(fth, FTH_GREATER);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, ".", FTHWORD_INSERT);
	fth_addIns(fth, FTH_PRINTNUM);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "EMIT", FTHWORD_INSERT);
	fth_addIns(fth, FTH_EMIT);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "I", FTHWORD_INSERT);
	fth_addIns(fth, FTH_PUSHB);
	fth_addIns(fth, 1);
	fth_addIns(fth, FTH_RPICK);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "J", FTHWORD_INSERT);
	fth_addIns(fth, FTH_PUSHB);
	fth_addIns(fth, 3);
	fth_addIns(fth, FTH_RPICK);
	fth_addIns(fth, FTH_RET);

	fth_addWord(fth, "IMMEDIATE", FTHWORD_IMMEDIATE);
	fth_addIns(fth, FTH_IMMEDIATE);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "INSERT", FTHWORD_IMMEDIATE);
	fth_addIns(fth, FTH_INSERT);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "IF", FTHWORD_IMMEDIATE);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_JZ);
	fth_addIns(fth, FTH_DSIZE);
	fth_addIns(fth, FTH_RPUSH);
	fth_addIns(fth, FTH_ADDVAL);
		fth_addVal(fth, 0);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "THEN", FTHWORD_IMMEDIATE);
	fth_addIns(fth, FTH_RSET);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "ELSE", FTHWORD_IMMEDIATE);
	fth_addIns(fth, FTH_RSET);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_JUMP);
	fth_addIns(fth, FTH_ADDVAL);
		fth_addVal(fth, 0);
	fth_addIns(fth, FTH_DSIZE);
	fth_addIns(fth, FTH_RPUSH);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "DO", FTHWORD_IMMEDIATE);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_RPUSH);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_DEC);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_RPUSH);
	fth_addIns(fth, FTH_DSIZE);
	fth_addIns(fth, FTH_RPUSH);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "LOOP", FTHWORD_IMMEDIATE);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_RPOP);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_RPOP);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_INC);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_OVER);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_OVER);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_LESS);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_JZ);
	fth_addIns(fth, FTH_ADDRETVAL);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "+LOOP", FTHWORD_IMMEDIATE);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_RPOP);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_RPOP);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_ROT);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_PLUS);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_OVER);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_OVER);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_LESS);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_JZ);
	fth_addIns(fth, FTH_ADDRETVAL);
	fth_addIns(fth, FTH_RET);

	fth->old_size = fth->size;

	return fth;
}

void freeForth(Forth *fth) {
	for(int i = 0; i < fth->num_words; i++)
		free(fth->words[i].name);
	if(fth->words)
		free(fth->words);

	if(fth->dict)
		free(fth->dict);

	free(fth);
}

void fth_nextInstruction(Forth *fth, size_t *pc) {
	switch(fth->dict[*pc]) {
	default:
		(*pc)++;
		break;
	case FTH_PUSHB:
	case FTH_ADDINS:
		(*pc) += 2;
		break;
	case FTH_JUMP:
	case FTH_JZ:
	case FTH_CALL:
	case FTH_PUSH:
	case FTH_PUSHPTR:
	case FTH_ADDVAL:
		(*pc) += sizeof(void*) + 1;
		break;
	case FTH_PUSHSTR:
		(*pc) += fth->dict[(*pc)+1] + 1;
		break;
	}
}

void fth_capitalize(char *s) {
	for(char *c = s; *c; c++)
		if(*c >= 'a' && *c <= 'z')
			*c += 'A' - 'a';
}

struct forthWord *fth_findWord(Forth *fth, const char *name) {
	for(size_t i = fth->num_words; i > 0; i--)
		if(strcmp(fth->words[i-1].name, name) == 0)
			return &fth->words[i-1];
	return 0;
}

void fth_printNum(Forth *fth, intptr_t n) {
	if(!n) {
		chput('0');
		return;
	}

	if(n < 0) {
		chput('-');
		n *= -1;
	}

	intptr_t x = 1;
	while(x < n)
		x *= (intptr_t)(fth->base);

	int ng = 0;
	while(x) {
		int d = 0;
		while(n-x >= 0) {
			n -= x;
			d++;
		}
		if(ng || d) {
			if(d < 10)
				chput(d+'0');
			else
				chput(d-10+'A');
			ng = 1;
		}

		x /= (intptr_t)(fth->base);
	}
}

void fth_printInstruction(Forth *fth, size_t pc, char detail) {
	size_t i;

	if(detail)
		printf("%lu\t", pc);

	switch(fth->dict[pc]) {
	case FTH_PUSH: printf("push"); break;
	case FTH_PUSHB: printf("pushb"); break;
	case FTH_PUSHSTR: printf("pushstr"); break;
	case FTH_PUSHPTR: printf("pushptr"); break;
	case FTH_CALL: printf("call"); break;
	case FTH_RET: printf("ret"); break;
	case FTH_HERE: printf("here"); break;
	case FTH_DSIZE: printf("dsize"); break;
	case FTH_RPUSH: printf("rpush"); break;
	case FTH_RPOP: printf("rpop"); break;
	case FTH_RDUP: printf("rdup"); break;
	case FTH_RDROP: printf("rdrop"); break;
	case FTH_RSET: printf("rset"); break;
	case FTH_JUMP: printf("jump"); break;
	case FTH_JZ: printf("jz"); break;
	case FTH_ADDINS: printf("addins"); break;
	case FTH_ADDVAL: printf("addval"); break;
	case FTH_ADDRETVAL: printf("addretval"); break;
	case FTH_DROP: printf("drop"); break;
	case FTH_DUP: printf("dup"); break;
	case FTH_SWAP: printf("swap"); break;
	case FTH_ROT: printf("rot"); break;
	case FTH_OVER: printf("over"); break;
	case FTH_DEPTH: printf("depth"); break;
	case FTH_PICK: printf("pick"); break;
	case FTH_RPICK: printf("rpick"); break;
	case FTH_PLUS: printf("plus"); break;
	case FTH_MINUS: printf("minus"); break;
	case FTH_INC: printf("inc"); break;
	case FTH_DEC: printf("dec"); break;
	case FTH_MUL: printf("mul"); break;
	case FTH_DIV: printf("div"); break;
	case FTH_MOD: printf("mod"); break;
	case FTH_EQUAL: printf("equal"); break;
	case FTH_NEQUAL: printf("nequal"); break;
	case FTH_GREATER: printf("greater"); break;
	case FTH_LESS: printf("less"); break;
	case FTH_AND: printf("and"); break;
	case FTH_OR: printf("or"); break;
	case FTH_XOR: printf("xor"); break;
	case FTH_INV: printf("inv"); break;
	case FTH_EMIT: printf("emit"); break;
	case FTH_TYPE: printf("type"); break;
	case FTH_PRINTNUM: printf("printnum"); break;
	case FTH_IMMEDIATE: printf("immediate"); break;
	case FTH_INSERT: printf("insert"); break;
	case FTH_KEY: printf("key"); break;
	case FTH_ACCEPT: printf("accept"); break;
	}

	if(!detail)
		return;

	switch(fth->dict[pc]) {
	case FTH_PUSHSTR:
		printf(" ");
		for(i = 0; i < fth->dict[pc+1]; i++)
			printf("%c", fth->dict[pc+1+i]);
		break;
	case FTH_ADDINS:
		printf(" ");
		fth_printInstruction(fth, pc+1, 0);
		break;
	case FTH_JUMP:
	case FTH_JZ:
	case FTH_CALL:
		printf(" %lu", *(void**)(fth->dict+pc+1));
		break;
	case FTH_PUSH:
	case FTH_ADDVAL:
		printf(" %ld", *(void**)(fth->dict+pc+1));
		break;
	case FTH_PUSHB:
		printf(" %d", fth->dict[pc+1]);
		break;
	}

	printf("\n");
}

void fth_printWord(Forth *fth, struct forthWord *wd) {
	switch(wd->type) {
	case FTHWORD_IMMEDIATE:
		printf("immediate\n");
		break;
	case FTHWORD_INSERT:
		printf("insert\n");
		break;
	}
	for(size_t pc = wd->addr; fth->dict[pc] != FTH_RET; fth_nextInstruction(fth, &pc))
		fth_printInstruction(fth, pc, 1);
}

void fth_run(Forth *fth, size_t pc, char immediate) {
	size_t call = 0;
	void *v1;
	size_t i;
	char c;

	while(pc < fth->size) {
		//fth_printInstruction(fth, pc, 1);
		switch(fth->dict[pc]) {
		case FTH_PUSH:
			fth->stack[fth->sp++] = *(void**)(fth->dict+pc+1);
			break;
		case FTH_PUSHB:
			fth->stack[fth->sp++] = (void*)(intptr_t)fth->dict[pc+1];
			break;
		case FTH_PUSHSTR:
			fth->stack[fth->sp++] = (void*)(fth->dict+pc+2);
			fth->stack[fth->sp++] = (void*)(intptr_t)fth->dict[pc+1];
			break;
		case FTH_PUSHPTR:
			fth->stack[fth->sp++] = (void*)(fth->dict+pc+1);
			break;
		case FTH_CALL:
			call++;
			fth->rstack[fth->rsp++] = (void*)pc;
			pc = (size_t)(*(void**)(fth->dict+pc+1));
			continue;
		case FTH_RET:
			if(!call && immediate)
				return;
			call--;
			pc = (size_t)fth->rstack[--(fth->rsp)];
			break;
		case FTH_HERE:
			fth->stack[fth->sp++] = (void*)(fth->dict+fth->size);
			break;
		case FTH_DSIZE:
			fth->stack[fth->sp++] = (void*)fth->size;
			break;
		case FTH_RPUSH:
			fth->rstack[fth->rsp++] = fth->stack[--(fth->sp)];
			break;
		case FTH_RPOP:
			fth->stack[fth->sp++] = fth->rstack[--(fth->rsp)];
			break;
		case FTH_RDUP:
			fth->rstack[fth->rsp++] = fth->stack[--(fth->sp)];
			break;
		case FTH_RDROP:
			fth->rsp--;
		case FTH_RSET:
			*(void**)(fth->rstack[--(fth->rsp)]) = fth->stack[--(fth->sp)];
			break;
		case FTH_JUMP:
			pc = (size_t)*(void**)(fth->dict+pc+1);
			break;
		case FTH_JZ:
			if(!fth->stack[--(fth->sp)])
				pc = (size_t)(*(void**)(fth->dict+pc+1));
			break;
		case FTH_ADDINS:
			fth_addIns(fth, fth->dict[pc+1]);
			break;
		case FTH_ADDVAL:
			fth_addVal(fth, *(void**)(fth->dict+pc+1));
			break;
		case FTH_ADDRETVAL:
			fth_addVal(fth, fth->rstack[--(fth->rsp)]);
			break;
		case FTH_DROP:
			fth->sp--;
			break;
		case FTH_DUP:
			fth->stack[fth->sp] = fth->stack[fth->sp-1];
			fth->sp++;
			break;
		case FTH_SWAP:
			v1 = fth->stack[fth->sp-1];
			fth->stack[fth->sp-1] = fth->stack[fth->sp-2];
			fth->stack[fth->sp-2] = v1;
			break;
		case FTH_ROT:
			v1 = fth->stack[fth->sp-3];
			fth->stack[fth->sp-3] = fth->stack[fth->sp-2];
			fth->stack[fth->sp-2] = fth->stack[fth->sp-1];
			fth->stack[fth->sp-1] = v1;
			break;
		case FTH_OVER:
			fth->stack[fth->sp] = fth->stack[fth->sp-1];
			fth->sp++;
			break;
		case FTH_DEPTH:
			fth->stack[fth->sp] = (void*)(fth->sp++);
			break;
		case FTH_PICK:
			fth->stack[fth->sp-1] = fth->stack[fth->sp-(size_t)fth->stack[fth->sp-1]-1];
			break;
		case FTH_RPICK:
			fth->stack[fth->sp-1] = fth->rstack[fth->rsp-(size_t)fth->stack[fth->sp-1]-1];
			break;
		case FTH_PLUS:
			fth->stack[fth->sp-2] = (void*)((intptr_t)fth->stack[fth->sp-2] + (intptr_t)fth->stack[--(fth->sp)]);
			break;
		case FTH_MINUS:
			fth->stack[fth->sp-2] = (void*)((intptr_t)fth->stack[fth->sp-2] - (intptr_t)fth->stack[--(fth->sp)]);
			break;
		case FTH_INC:
			fth->stack[fth->sp-1] = (void*)((intptr_t)fth->stack[fth->sp-1] + 1);
			break;
		case FTH_DEC:
			fth->stack[fth->sp-1] = (void*)((intptr_t)fth->stack[fth->sp-1] - 1);
			break;
		case FTH_MUL:
			fth->stack[fth->sp-2] = (void*)((intptr_t)fth->stack[fth->sp-2] * (intptr_t)fth->stack[fth->sp-1]);
			fth->sp--;
			break;
		case FTH_DIV:
			fth->stack[fth->sp-2] = (void*)((intptr_t)fth->stack[fth->sp-2] / (intptr_t)fth->stack[fth->sp-1]);
			fth->sp--;
			break;
		case FTH_MOD:
			fth->stack[fth->sp-2] = (void*)((intptr_t)fth->stack[fth->sp-2] % (intptr_t)fth->stack[fth->sp-1]);
			fth->sp--;
			break;
		case FTH_EQUAL:
			fth->stack[fth->sp-2] = (void*)(intptr_t)((intptr_t)fth->stack[fth->sp-2] == (intptr_t)fth->stack[fth->sp-1]);
			fth->sp--;
			break;
		case FTH_NEQUAL:
			fth->stack[fth->sp-2] = (void*)(intptr_t)((intptr_t)fth->stack[fth->sp-2] != (intptr_t)fth->stack[fth->sp-1]);
			fth->sp--;
			break;
		case FTH_GREATER:
			fth->stack[fth->sp-2] = (void*)(intptr_t)((intptr_t)fth->stack[fth->sp-2] > (intptr_t)fth->stack[fth->sp-1]);
			fth->sp--;
			break;
		case FTH_LESS:
			fth->stack[fth->sp-2] = (void*)(intptr_t)((intptr_t)fth->stack[fth->sp-2] < (intptr_t)fth->stack[fth->sp-1]);
			fth->sp--;
			break;
		case FTH_AND:
			fth->stack[fth->sp-2] = (void*)((intptr_t)fth->stack[fth->sp-2] & (intptr_t)fth->stack[fth->sp-1]);
			fth->sp--;
			break;
		case FTH_OR:
			fth->stack[fth->sp-2] = (void*)((intptr_t)fth->stack[fth->sp-2] | (intptr_t)fth->stack[fth->sp-1]);
			fth->sp--;
			break;
		case FTH_XOR:
			fth->stack[fth->sp-2] = (void*)((intptr_t)fth->stack[fth->sp-2] ^ (intptr_t)fth->stack[fth->sp-1]);
			fth->sp--;
			break;
		case FTH_INV:
			fth->stack[fth->sp-1] = (void*)(~((intptr_t)fth->stack[fth->sp-1]));
			break;
		case FTH_EMIT:
			chput((char)(intptr_t)(fth->stack[--(fth->sp)]));
			break;
		case FTH_TYPE:
			for(i = 0; i < (size_t)fth->stack[fth->sp-1]; i++)
				chput(((char*)(fth->stack[fth->sp-2]))[i]);
			fth->sp -= 2;
			break;
		case FTH_PRINTNUM:
			fth_printNum(fth, (intptr_t)(fth->stack[--(fth->sp)]));
			break;
		case FTH_IMMEDIATE:
			fth->words[fth->num_words-1].type = FTHWORD_IMMEDIATE;
			break;
		case FTH_INSERT:
			fth->words[fth->num_words-1].type = FTHWORD_INSERT;
			break;
		case FTH_KEY:
			fth->stack[fth->sp++] = (void*)(intptr_t)chget();
			break;
		case FTH_ACCEPT:
			for(i = 0; i < (size_t)fth->stack[fth->sp-1]; i++) {
				c = chget();
				if(c == '\n' || c == 0)
					break;
				((char*)(fth->stack[fth->sp-2]))[i] = c;
			}
			fth->sp -= 2;
			fth->stack[fth->sp++] = (void*)i;
			break;
		}

		fth_nextInstruction(fth, &pc);
	}
}

char fth_isInteger(Forth *fth, char *s, intptr_t *n) {
	*n = 0;
	char neg = 0;
	size_t base = (size_t)fth->base;

	if(!s[0])
		return 0;

	for(size_t i = 0; s[i]; i++) {
		if(s[i] == '-') {
			if(i == 0 && s[i+1])
				neg = 1;
			else
				return 0;
		}
		else if(s[i] >= '0' && s[i] <= '9')
			*n = (*n)*base + s[i]-'0';
		else if(s[i]-'A'+10 < base)
			*n = (*n)*base + s[i]-'A'+10;
		else
			return 0;
	}

	if(neg)
		*n *= -1;

	return 1;
}

void fth_runToken(Forth *fth, char *s) {
	struct forthWord *wd;
	size_t i, j;
	intptr_t n;
	switch(fth->mode) {
	case FTHMODE_WORDNAME:
		fth_capitalize(s);
		fth_addWord(fth, s, FTHWORD_NORMAL);
		fth->mode = FTHMODE_WORD;
		break;
	case FTHMODE_WORD:
		if(strcmp(s, ":") == 0) {
			printf("cannot define word within word !\n");
			break;
		}
		if(strcmp(s, ";") == 0) {
			fth_addIns(fth, FTH_RET);
			fth->old_size = fth->size;
			fth->mode = FTHMODE_RUN;
			break;
		}
	case FTHMODE_RUN:
		if(strcmp(s, ":") == 0) {
			fth->mode = FTHMODE_WORDNAME;
			break;
		}
		fth_capitalize(s);
		wd = fth_findWord(fth, s);
		if(wd) {
			switch(wd->type) {
			case FTHWORD_INSERT:
				for(i = wd->addr; fth->dict[i] != FTH_RET; fth_nextInstruction(fth, &i));
				for(j = wd->addr; j < i; j++)
					fth_addIns(fth, fth->dict[j]);
				break;
			case FTHWORD_IMMEDIATE:
				fth_run(fth, wd->addr, 1);
				break;
			case FTHWORD_NORMAL:
				fth_addIns(fth, FTH_CALL);
				fth_addVal(fth, (void*)wd->addr);
				break;
			}
		}
		else if(fth_isInteger(fth, s, &n)) {
			if(n >= -128 && n <= 127) {
				fth_addIns(fth, FTH_PUSHB);
				fth_addIns(fth, (char)n);
			}
			else {
				fth_addIns(fth, FTH_PUSH);
				fth_addVal(fth, (void*)n);
			}
		}
		else
			printf("%s ?\n", s);
	}

	if(fth->mode == FTHMODE_RUN && fth->rsp == 0 && fth->size != fth->old_size) {
		fth_run(fth, fth->old_size, 0);
		fth->size = fth->old_size;
	}
}

void fth_runString(Forth *fth, const char *text) {
	int max = 100;
	int buf = 30;
	char *s = (char*)malloc(max);
	int len = 0;

	char quote = 0;
	char comment = 0;

	for(const char *c = text; ; c++) {
		if(*c == '\n' || *c == 0) {
			if(quote || len) {
				s[len] = 0;
				char wq = quote;

				if(!s[1] && (s[0]=='('||s[0]=='\\')) {
					comment = s[0];
				}
				else if(strlen(s) == 2)
					if((s[0]=='.'||s[0]=='s'||s[0]=='S') && (s[1]=='"'||s[1]=='('||s[1]=='\'')) {
						quote = s[1];
						s[1] = '"';
						if(quote == '(')
							quote = ')';
					}

				if(!comment)
					fth_runToken(fth, s);
				if(quote && !wq)
					fth_runToken(fth, (char*)"");
			}

			len = 0;
			quote = 0;
			comment = 0;

			if(*c == 0)
				break;
		}

		else if(comment) {
			if(comment == '(' && *c == ')')
				comment = 0;
			continue;
		}

		else if(quote) {
			if(*c == quote) {
				s[len] = 0;
				fth_runToken(fth, s);
				len = 0;
				quote = 0;
			}
			else
				s[len++] = *c;
		}

		else if(*c == ' ' || *c == '\t') {
			if(len) {
				s[len] = 0;
				len = 0;

				if(!s[1] && (s[0]=='('||s[0]=='\\')) {
					comment = s[0];
					continue;
				}
				else if(strlen(s) == 2)
					if((s[0]=='.'||s[0]=='s'||s[0]=='S') && (s[1]=='"'||s[1]=='('||s[1]=='\'')) {
						quote = s[1];
						s[1] = '"';
						if(quote == '(')
							quote = ')';
					}

				fth_runToken(fth, s);
			}
		}

		else
			s[len++] = *c;

		if(len > max-buf) {
			max += buf;
			s = (char*)realloc(s, max);
		}
	}

	free(s);
}

void fth_runFile(Forth *fth, const char *filename) {
	FILE *fp = fopen(filename, "r");
	if(!fp) {
		printf("failed to open %s !\n", filename);
		return;
	}

	int max = 100;
	int buf = 30;
	char *s = (char*)malloc(max);
	int len = 0;

	while(!feof(fp)) {
		char c = fgetc(fp);
		s[len++] = c;
		if(len > max-buf) {
			max += buf;
			s = (char*)realloc(s, max);
		}
	}
	fclose(fp);
	s[len-1] = 0;

	fth_runString(fth, s);
	free(s);
	return;
}
