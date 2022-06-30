#include "sforth.h"
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
	FTH_HERE,
	FTH_RPUSH,
	FTH_RPOP,
	FTH_RDUP,
	FTH_RDROP,
	FTH_RSET,
	FTH_JMP,
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
	FTH_IMMEDIATE,
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
	fth->dictSize(fth, fth->size+sizeof(void*));
	memcpy(fth->dict+fth->size-sizeof(void*), &val);
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

	fth_addWord(fth, "IF", FTHWORD_IMMEDIATE);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_JZ);
	fth_addIns(fth, FTH_ADDVAL);
		fth_addVal(fth, 0);
	fth_addIns(fth, FTH_HERE);
	fth_addIns(fth, FTH_RPUSH);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "THEN", FTHWORD_IMMEDIATE);
	fth_addIns(fth, FTH_RSET);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "ELSE", FTHWORD_IMMEDIATE);
	fth_addIns(fth, FTH_RSET);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_JMP);
	fth_addIns(fth, FTH_ADDVAL);
		fth_addVal(fth, 0);
	fth_addIns(fth, FTH_HERE);
	fth_addIns(fth, FTH_RPUSH);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "DO", FTHWORD_IMMEDIATE);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_RPUSH);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_DEC);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_RPUSH);
	fth_addIns(fth, FTH_HERE);
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
		fth->emit('0');
		return;
	}

	if(n < 0) {
		fth->emit('-');
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
				fth->emit(d+'0');
			else
				fth->emit(d-10+'A');
			ng = 1;
		}

		x /= (intptr_t)(fth->base);
	}
}
