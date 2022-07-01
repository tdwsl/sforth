#ifndef SFORTH_MACHINE_H
#define SFORTH_MACHINE_H

#include "sforth.h"
#include <stdlib.h>

enum {
	FTH_PUSH,
	FTH_PUSHB,
	FTH_PUSHSTR,
	FTH_PUSHPTR,
	FTH_PUSHPTR2,
	FTH_CALL,
	FTH_RET,
	FTH_EXIT,
	FTH_BYE,
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
	FTH_KEY,
	FTH_ACCEPT,
	FTH_SET,
	FTH_GET,
	FTH_SETC,
	FTH_GETC,
	FTH_GETNEXT,
	FTH_CAPITALIZE,
	FTH_FREEBUF,
	FTH_PRINTBUF,
	FTH_ADDSTR,
	FTH_ADDCH,
	FTH_SEE,
	FTH_INCLUDE,
	FTH_ADDWORD,
	FTH_ALLOT,
};

void fth_addWord(Forth *fth, const char *name, char type);
struct forthWord *fth_findWord(Forth *fth, const char *name);
void fth_capitalize(char *s);
void fth_addIns(Forth *fth, char ins);
void fth_addVal(Forth *fth, void *val);
void fth_nextInstruction(Forth *fth, size_t *pc);
void fth_addDefaultWords(Forth *fth);
void fth_printWord(Forth *fth, struct forthWord *wd);
void fth_printNum(Forth *fth, intptr_t n);
void fth_printStack(Forth *fth);
void fth_printRStack(Forth *fth);
void fth_run(Forth *fth);

#endif
