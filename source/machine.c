#include "sforth.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

void fth_addWord(Forth *fth, const char *name, char type) {
	fth->num_words++;
	if(fth->num_words < fth->max_words-10) {
		fth->max_words += 10;
		fth->words = (struct forthWord*)realloc(fth->words, sizeof(struct forthWord)*fth->max_words);
	}
	char *s = malloc(strlen(name)+1);
	strcpy(s, name);
	fth->words[fth->num_words-1] = (struct forthWord){ s, type, fth->size };
}

struct forthWord *fth_findWord(Forth *fth, const char *name) {
	for(size_t i = fth->num_words; i > 0; i--)
		if(strcmp(fth->words[i-1].name, name) == 0)
			return &fth->words[i-1];
	return 0;
}

void fth_capitalize(char *s) {
	for(char *c = s; *c; c++)
		if(*c >= 'a' && *c <= 'z')
			*c += 'A' - 'a';
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

void fth_addDefaultWords(Forth *fth) {
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
	fth_addWord(fth, "ABS", FTHWORD_INSERT);
	fth_addIns(fth, FTH_ABS);
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
	fth_addWord(fth, "TYPE", FTHWORD_INSERT);
	fth_addIns(fth, FTH_TYPE);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "CR", FTHWORD_INSERT);
	fth_addIns(fth, FTH_PUSHB);
	fth_addIns(fth, '\n');
	fth_addIns(fth, FTH_EMIT);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "SPACE", FTHWORD_INSERT);
	fth_addIns(fth, FTH_PUSHB);
	fth_addIns(fth, ' ');
	fth_addIns(fth, FTH_EMIT);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "BYE", FTHWORD_INSERT);
	fth_addIns(fth, FTH_BYE);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "KEY", FTHWORD_INSERT);
	fth_addIns(fth, FTH_KEY);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "ACCEPT", FTHWORD_INSERT);
	fth_addIns(fth, FTH_ACCEPT);
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
	fth_addWord(fth, "@", FTHWORD_INSERT);
	fth_addIns(fth, FTH_GET);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "!", FTHWORD_INSERT);
	fth_addIns(fth, FTH_SET);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "C@", FTHWORD_INSERT);
	fth_addIns(fth, FTH_GETC);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "C!", FTHWORD_INSERT);
	fth_addIns(fth, FTH_SETC);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "BASE", FTHWORD_INSERT);
	fth_addIns(fth, FTH_PUSH);
	fth_addVal(fth, &fth->base);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "CHARS", FTHWORD_INSERT);
	fth_addIns(fth, FTH_PUSHB);
	fth_addIns(fth, sizeof(char));
	fth_addIns(fth, FTH_MUL);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "CHAR+", FTHWORD_INSERT);
	fth_addIns(fth, FTH_PUSHB);
	fth_addIns(fth, sizeof(char));
	fth_addIns(fth, FTH_PLUS);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "CELL", FTHWORD_INSERT);
	fth_addIns(fth, FTH_PUSHB);
	fth_addIns(fth, sizeof(void*));
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "CELLS", FTHWORD_INSERT);
	fth_addIns(fth, FTH_PUSHB);
	fth_addIns(fth, sizeof(void*));
	fth_addIns(fth, FTH_MUL);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "CELL+", FTHWORD_INSERT);
	fth_addIns(fth, FTH_PUSHB);
	fth_addIns(fth, sizeof(void*));
	fth_addIns(fth, FTH_PLUS);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "EXIT", FTHWORD_INSERT);
	fth_addIns(fth, FTH_EXIT);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "WORDS", FTHWORD_INSERT);
	fth_addIns(fth, FTH_WORDS);
	fth_addIns(fth, FTH_RET);

	fth_addWord(fth, "IMMEDIATE", FTHWORD_IMMEDIATE);
	fth_addIns(fth, FTH_IMMEDIATE);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "IF", FTHWORD_IMMEDIATE);
	fth_addWord(fth, "WHILE", FTHWORD_IMMEDIATE);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_JZ);
	fth_addIns(fth, FTH_DSIZE);
	fth_addIns(fth, FTH_RPUSH);
	fth_addIns(fth, FTH_PUSHB);
	fth_addIns(fth, 0);
	fth_addIns(fth, FTH_ADDVAL);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "THEN", FTHWORD_IMMEDIATE);
	fth_addIns(fth, FTH_DSIZE);
	fth_addIns(fth, FTH_RSET);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "ELSE", FTHWORD_IMMEDIATE);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_JUMP);
	fth_addIns(fth, FTH_DSIZE);
	fth_addIns(fth, FTH_PUSHB);
	fth_addIns(fth, 0);
	fth_addIns(fth, FTH_ADDVAL);
	fth_addIns(fth, FTH_DSIZE);
	fth_addIns(fth, FTH_RSET);
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
		fth_addIns(fth, FTH_RPUSH);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_RPUSH);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_LESS);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_JZ);
	fth_addIns(fth, FTH_ADDRETVAL);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_RDROP);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_RDROP);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "+LOOP", FTHWORD_IMMEDIATE);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_RPOP);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_RPOP);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_OVER);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_OVER);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_LESS);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_RPUSH);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_ROT);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_PLUS);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_RPOP);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_ROT);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_ROT);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_OVER);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_OVER);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_RPUSH);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_RPUSH);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_LESS);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_NEQUAL);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_JZ);
	fth_addIns(fth, FTH_ADDRETVAL);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_RDROP);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_RDROP);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "BEGIN", FTHWORD_IMMEDIATE);
	fth_addIns(fth, FTH_DSIZE);
	fth_addIns(fth, FTH_RPUSH);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "UNTIL", FTHWORD_IMMEDIATE);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_JZ);
	fth_addIns(fth, FTH_RPOP);
	fth_addIns(fth, FTH_ADDVAL);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "REPEAT", FTHWORD_IMMEDIATE);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_JUMP);
	fth_addIns(fth, FTH_RPOP);
	fth_addIns(fth, FTH_RPOP);
	fth_addIns(fth, FTH_ADDVAL);
	fth_addIns(fth, FTH_RPUSH);
	fth_addIns(fth, FTH_DSIZE);
	fth_addIns(fth, FTH_RSET);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "S\"", FTHWORD_IMMEDIATE);
	fth_addIns(fth, FTH_GETNEXT);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_PUSHSTR);
	fth_addIns(fth, FTH_ADDSTR);
	fth_addIns(fth, FTH_FREEBUF);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, ".\"", FTHWORD_IMMEDIATE);
	fth_addIns(fth, FTH_GETNEXT);
	fth_addIns(fth, FTH_PRINTBUF);
	fth_addIns(fth, FTH_FREEBUF);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "[CHAR]", FTHWORD_IMMEDIATE);
	fth_addIns(fth, FTH_GETNEXT);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_PUSHB);
	fth_addIns(fth, FTH_ADDCH);
	fth_addIns(fth, FTH_FREEBUF);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "CHAR", FTHWORD_IMMEDIATE);
	fth_addIns(fth, FTH_GETNEXT);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_PUSHB);
	fth_addIns(fth, FTH_ADDCH);
	fth_addIns(fth, FTH_FREEBUF);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "SEE", FTHWORD_IMMEDIATE);
	fth_addIns(fth, FTH_GETNEXT);
	fth_addIns(fth, FTH_CAPITALIZE);
	fth_addIns(fth, FTH_SEE);
	fth_addIns(fth, FTH_FREEBUF);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "INCLUDE", FTHWORD_IMMEDIATE);
	fth_addIns(fth, FTH_OLDSIZE);
	fth_addIns(fth, FTH_GETNEXT);
	fth_addIns(fth, FTH_INCLUDE);
	fth_addIns(fth, FTH_FREEBUF);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "CONSTANT", FTHWORD_IMMEDIATE);
	fth_addIns(fth, FTH_OLDSIZE);
	fth_addIns(fth, FTH_GETNEXT);
	fth_addIns(fth, FTH_CAPITALIZE);
	fth_addIns(fth, FTH_ADDWORD);
	fth_addIns(fth, FTH_FREEBUF);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_PUSH);
	fth_addIns(fth, FTH_ADDVAL);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_RET);
	fth_addIns(fth, FTH_RESIZE);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "CREATE", FTHWORD_IMMEDIATE);
	fth_addIns(fth, FTH_OLDSIZE);
	fth_addIns(fth, FTH_GETNEXT);
	fth_addIns(fth, FTH_CAPITALIZE);
	fth_addIns(fth, FTH_ADDWORD);
	fth_addIns(fth, FTH_FREEBUF);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_PUSHPTR2);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_RET);
	fth_addIns(fth, FTH_RESIZE);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "VARIABLE", FTHWORD_IMMEDIATE);
	fth_addIns(fth, FTH_OLDSIZE);
	fth_addIns(fth, FTH_GETNEXT);
	fth_addIns(fth, FTH_CAPITALIZE);
	fth_addIns(fth, FTH_ADDWORD);
	fth_addIns(fth, FTH_FREEBUF);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_PUSHPTR2);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_RET);
	fth_addIns(fth, FTH_PUSHB);
	fth_addIns(fth, sizeof(void*));
	fth_addIns(fth, FTH_ALLOT);
	fth_addIns(fth, FTH_RESIZE);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "VALUE", FTHWORD_IMMEDIATE);
	fth_addIns(fth, FTH_OLDSIZE);
	fth_addIns(fth, FTH_GETNEXT);
	fth_addIns(fth, FTH_CAPITALIZE);
	fth_addIns(fth, FTH_ADDWORD);
	fth_addIns(fth, FTH_FREEBUF);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_PUSHPTR);
	fth_addIns(fth, FTH_ADDVAL);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_GET);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_RET);
	fth_addIns(fth, FTH_RESIZE);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "TO", FTHWORD_IMMEDIATE);
	fth_addIns(fth, FTH_OLDSIZE);
	fth_addIns(fth, FTH_GETNEXT);
	fth_addIns(fth, FTH_CAPITALIZE);
	fth_addIns(fth, FTH_TO);
	fth_addIns(fth, FTH_FREEBUF);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "ALLOT", FTHWORD_IMMEDIATE);
	fth_addIns(fth, FTH_OLDSIZE);
	fth_addIns(fth, FTH_ALLOT);
	fth_addIns(fth, FTH_RESIZE);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "MARKER", FTHWORD_IMMEDIATE);
	fth_addIns(fth, FTH_OLDSIZE);
	fth_addIns(fth, FTH_GETNEXT);
	fth_addIns(fth, FTH_CAPITALIZE);
	fth_addIns(fth, FTH_ADDWORD);
	fth_addIns(fth, FTH_FREEBUF);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_MARKER);
	fth_addIns(fth, FTH_ADDINS);
		fth_addIns(fth, FTH_RET);
	fth_addIns(fth, FTH_RESIZE);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "FORGET", FTHWORD_IMMEDIATE);
	fth_addIns(fth, FTH_GETNEXT);
	fth_addIns(fth, FTH_CAPITALIZE);
	fth_addIns(fth, FTH_FORGET);
	fth_addIns(fth, FTH_FREEBUF);
	fth_addIns(fth, FTH_RET);

	fth_addWord(fth, "TRACEON", FTHWORD_INSERT);
	fth_addIns(fth, FTH_PUSHB);
	fth_addIns(fth, 1);
	fth_addIns(fth, FTH_PUSH);
	fth_addVal(fth, &fth->trace);
	fth_addIns(fth, FTH_SETC);
	fth_addIns(fth, FTH_RET);
	fth_addWord(fth, "TRACEOFF", FTHWORD_INSERT);
	fth_addIns(fth, FTH_PUSHB);
	fth_addIns(fth, 0);
	fth_addIns(fth, FTH_PUSH);
	fth_addVal(fth, &fth->trace);
	fth_addIns(fth, FTH_SETC);
	fth_addIns(fth, FTH_RET);

	fth->old_size = fth->size;
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
	case FTH_FUNCTION:
		(*pc) += sizeof(void*) + 1;
		break;
	case FTH_PUSHSTR:
		(*pc) += fth->dict[(*pc)+1] + 2;
		break;
	}
}

void fth_printNum(Forth *fth, intptr_t n) {
	if(!n) {
		fth_chput('0');
		return;
	}

	if(n < 0) {
		fth_chput('-');
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
				fth_chput(d+'0');
			else
				fth_chput(d-10+'A');
			ng = 1;
		}

		x /= (intptr_t)(fth->base);
	}
}

void fth_printStack(Forth *fth) {
	printf("<");
	for(size_t i = 0; i < fth->sp; i++) {
		fth_printNum(fth, (intptr_t)fth->stack[i]);
		if(i < fth->sp-1)
			printf(" ");
	}
	printf(">\n");
}

void fth_printRStack(Forth *fth) {
	printf("<");
	for(size_t i = 0; i < fth->rsp; i++) {
		fth_printNum(fth, (intptr_t)fth->rstack[i]);
		if(i < fth->rsp-1)
			printf(" ");
	}
	printf(">\n");
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
	case FTH_PUSHPTR2: printf("pushptr2"); break;
	case FTH_CALL: printf("call"); break;
	case FTH_RET: printf("ret"); break;
	case FTH_EXIT: printf("exit"); break;
	case FTH_BYE: printf("bye"); break;
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
	case FTH_ABS: printf("abs"); break;
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
	case FTH_KEY: printf("key"); break;
	case FTH_ACCEPT: printf("accept"); break;
	case FTH_SET: printf("set"); break;
	case FTH_GET: printf("get"); break;
	case FTH_SETC: printf("setc"); break;
	case FTH_GETC: printf("getc"); break;
	case FTH_GETNEXT: printf("getnext"); break;
	case FTH_CAPITALIZE: printf("capitalize"); break;
	case FTH_FREEBUF: printf("freebuf"); break;
	case FTH_ADDSTR: printf("addstr"); break;
	case FTH_ADDCH: printf("addch"); break;
	case FTH_SEE: printf("see"); break;
	case FTH_INCLUDE: printf("include"); break;
	case FTH_ADDWORD: printf("addword"); break;
	case FTH_ALLOT: printf("allot"); break;
	case FTH_RESIZE: printf("resize"); break;
	case FTH_OLDSIZE: printf("oldsize"); break;
	case FTH_TO: printf("to"); break;
	case FTH_FUNCTION: printf("function"); break;
	case FTH_MARKER: printf("marker"); break;
	case FTH_WORDS: printf("words"); break;
	case FTH_FORGET: printf("forget"); break;
	}

	if(!detail)
		return;

	switch(fth->dict[pc]) {
	case FTH_PUSHSTR:
		printf(" ");
		for(i = 0; i < fth->dict[pc+1]; i++)
			printf("%c", fth->dict[pc+2+i]);
		break;
	case FTH_ADDINS:
		printf(" ");
		fth_printInstruction(fth, pc+1, 0);
		break;
	case FTH_JUMP:
	case FTH_JZ:
	case FTH_CALL:
		printf(" %lu", (size_t)*(void**)(fth->dict+pc+1));
		break;
	case FTH_PUSH:
	case FTH_FUNCTION:
		printf(" %ld", (intptr_t)*(void**)(fth->dict+pc+1));
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

void fth_run(Forth *fth) {
	void *v1;
	size_t i, j;
	struct forthWord *wd;
	void (*fun)(Forth*);

	while(fth->pc < fth->size && !fth->quit) {
		if(fth->trace) {
			fth_printStack(fth);
			fth_printRStack(fth);
			fth_printInstruction(fth, fth->pc, 1);
		}
		switch(fth->dict[fth->pc]) {
		case FTH_PUSH:
			fth->stack[fth->sp++] = *(void**)(fth->dict+fth->pc+1);
			break;
		case FTH_PUSHB:
			fth->stack[fth->sp++] = (void*)(intptr_t)fth->dict[fth->pc+1];
			break;
		case FTH_PUSHSTR:
			fth->stack[fth->sp++] = (void*)(fth->dict+fth->pc+2);
			fth->stack[fth->sp++] = (void*)(intptr_t)fth->dict[fth->pc+1];
			break;
		case FTH_PUSHPTR:
			fth->stack[fth->sp++] = (void*)(fth->dict+fth->pc+1);
			break;
		case FTH_PUSHPTR2:
			fth->stack[fth->sp++] = (void*)(fth->dict+fth->pc+2);
			break;
		case FTH_CALL:
			if(fth->immediate)
				fth->immediate++;
			fth->rstack[fth->rsp++] = (void*)fth->pc;
			fth->pc = (size_t)(*(void**)(fth->dict+fth->pc+1));
			continue;
		case FTH_RET:
		case FTH_EXIT:
			if(fth->immediate)
				if(--(fth->immediate) < 1)
					return;
			if(fth->rsp <= 0)
				return;
			fth->pc = (size_t)fth->rstack[--(fth->rsp)];
			break;
		case FTH_BYE:
			fth->quit = 1;
			return;
		case FTH_HERE:
			fth->stack[fth->sp++] = (void*)(fth->dict+fth->size);
			break;
		case FTH_DSIZE:
			fth->stack[fth->sp++] = (void*)(fth->size);
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
			break;
		case FTH_RSET:
			i = fth->size;
			fth->size = (size_t)fth->rstack[--(fth->rsp)];
			fth_addVal(fth, fth->stack[--(fth->sp)]);
			fth->size = i;
			break;
		case FTH_JUMP:
			fth->pc = (size_t)*(void**)(fth->dict+fth->pc+1);
			continue;
		case FTH_JZ:
			if(!fth->stack[--(fth->sp)]) {
				fth->pc = (size_t)(*(void**)(fth->dict+fth->pc+1));
				continue;
			}
			break;
		case FTH_ADDINS:
			fth_addIns(fth, fth->dict[fth->pc+1]);
			break;
		case FTH_ADDVAL:
			fth_addVal(fth, fth->stack[--(fth->sp)]);
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
			fth->stack[fth->sp] = fth->stack[fth->sp-2];
			fth->sp++;
			break;
		case FTH_DEPTH:
			fth->stack[fth->sp] = (void*)(intptr_t)fth->sp;
			fth->sp++;
			break;
		case FTH_PICK:
			fth->stack[fth->sp-1] = fth->stack[fth->sp-(size_t)fth->stack[fth->sp-1]-1];
			break;
		case FTH_RPICK:
			fth->stack[fth->sp-1] = fth->rstack[fth->rsp-(size_t)fth->stack[fth->sp-1]-1];
			break;
		case FTH_PLUS:
			fth->stack[fth->sp-2] = (void*)((intptr_t)fth->stack[fth->sp-2] + (intptr_t)fth->stack[fth->sp-1]);
			fth->sp--;
			break;
		case FTH_MINUS:
			fth->stack[fth->sp-2] = (void*)((intptr_t)fth->stack[fth->sp-2] - (intptr_t)fth->stack[fth->sp-1]);
			fth->sp--;
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
		case FTH_ABS:
			if((intptr_t)fth->stack[fth->sp-1] < 0)
				fth->stack[fth->sp-1] = (void*)((intptr_t)fth->stack[fth->sp-1]*-1);
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
			fth_chput((char)(intptr_t)(fth->stack[--(fth->sp)]));
			break;
		case FTH_TYPE:
			for(i = 0; i < (size_t)fth->stack[fth->sp-1]; i++)
				fth_chput(((char*)(fth->stack[fth->sp-2]))[i]);
			fth->sp -= 2;
			break;
		case FTH_PRINTNUM:
			fth_printNum(fth, (intptr_t)(fth->stack[--(fth->sp)]));
			fth_chput(' ');
			break;
		case FTH_IMMEDIATE:
			fth->words[fth->num_words-1].type = FTHWORD_IMMEDIATE;
			break;
		case FTH_KEY:
			fth->stack[fth->sp++] = (void*)(intptr_t)fth_chget();
			break;
		case FTH_ACCEPT:
			i = fth_readln((char*)fth->stack[fth->sp-2], (size_t)fth->stack[fth->sp-1]);
			fth->sp -= 2;
			fth->stack[fth->sp++] = (void*)i;
			break;
		case FTH_SET:
			*(void**)(fth->stack[fth->sp-1]) = fth->stack[fth->sp-2];
			fth->sp -= 2;
			break;
		case FTH_GET:
			fth->stack[fth->sp-1] = *(void**)(fth->stack[fth->sp-1]);
			break;
		case FTH_SETC:
			*(char*)fth->stack[fth->sp-1] = (char)(intptr_t)fth->stack[fth->sp-2];
			fth->sp -= 2;
			break;
		case FTH_GETC:
			fth->stack[fth->sp-1] = (void*)(intptr_t)*(char*)fth->stack[fth->sp-1];
			break;
		case FTH_GETNEXT:
			fth->old_mode = fth->mode;
			fth->mode = FTHMODE_GETNEXT;
			fth->pc++;
			return;
		case FTH_CAPITALIZE:
			fth_capitalize(fth->buf);
			break;
		case FTH_FREEBUF:
			free(fth->buf);
			break;
		case FTH_PRINTBUF:
			for(i = 0; fth->buf[i]; i++)
				fth_chput(fth->buf[i]);
			break;
		case FTH_ADDSTR:
			fth_addIns(fth, strlen(fth->buf));
			for(i = 0; i < fth->buf[i]; i++)
				fth_addIns(fth, fth->buf[i]);
			break;
		case FTH_ADDCH:
			fth_addIns(fth, fth->buf[0]);
			break;
		case FTH_SEE:
			wd = fth_findWord(fth, fth->buf);
			if(wd)
				fth_printWord(fth, wd);
			else
				printf("%s ?\n", fth->buf);
			break;
		case FTH_INCLUDE:
			fth_runFile(fth, fth->buf);
			break;
		case FTH_ADDWORD:
			fth_addWord(fth, fth->buf, FTHWORD_NORMAL);
			break;
		case FTH_ALLOT:
			for(i = 0; i < (size_t)fth->stack[fth->sp-1]; i++)
				fth_addIns(fth, 0);
			fth->sp--;
			break;
		case FTH_OLDSIZE:
			fth->size = fth->old_size;
			fth->rsp = 0;
			fth->immediate = fth->immediate != 0;
			break;
		case FTH_RESIZE:
			fth->old_size = fth->size;
			break;
		case FTH_TO:
			wd = fth_findWord(fth, fth->buf);
			if(!wd)
				printf("%s ?\n", fth->buf);
			else {
				fth_addIns(fth, FTH_PUSH);
				fth_addVal(fth, (void*)(fth->dict+wd->addr+1));
				fth_addIns(fth, FTH_SET);
			}
			break;
		case FTH_FUNCTION:
			memcpy(&fun, fth->dict+fth->pc+1, sizeof(void*));
			fun(fth);
			break;
		case FTH_MARKER:
			fth->size = fth->pc;
			fth->old_size = fth->size;
			for(i = 0; i < fth->num_words && fth->words[i].addr < fth->size; i++);
			for(j = i; j < fth->num_words; j++)
				free(fth->words[j].name);
			fth->num_words = i;
			fth->rsp = 0;
			return;
		case FTH_WORDS:
			for(i = 0; i < fth->num_words; i++) {
				printf("%s\t", fth->words[i].name);
				if((i+1)%4 == 0)
					printf("\n");
			}
			if((i+1)%4)
				printf("\n");
			break;
		case FTH_FORGET:
			wd = fth_findWord(fth, fth->buf);
			if(wd) {
				free(wd->name);
				fth->num_words--;
				for(i = wd-fth->words; i < fth->num_words; i++)
					fth->words[i] = fth->words[i+1];
			}
			else
				printf("%s ?\n", wd->name);
			break;
		}

		if(fth->sp < 0) {
			fth->sp = 0;
			printf("stack underflow !\n");
		}
		if(fth->rsp < 0) {
			fth->rsp = 0;
			printf("return stack underflow !\n");
		}

		fth_nextInstruction(fth, &fth->pc);
	}
}
