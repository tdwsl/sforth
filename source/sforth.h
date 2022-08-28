#ifndef SFORTH_H
#define SFORTH_H

#include <stdlib.h>
#include <stdint.h>

#define FTH_STACKSZ 512
#define FTH_RSTACKSZ 1024

enum {
	FTHWORD_NORMAL,
	FTHWORD_IMMEDIATE,
	FTHWORD_INSERT,
};

enum {
	FTHMODE_RUN,
	FTHMODE_WORD,
	FTHMODE_WORDNAME,
	FTHMODE_GETNEXT,
};

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
	FTH_SKIPB,
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
	FTH_ABS,
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
	FTH_RESIZE,
	FTH_OLDSIZE,
	FTH_TO,
	FTH_FUNCTION,
	FTH_MARKER,
	FTH_WORDS,
	FTH_FORGET,
};

struct forthWord {
	char *name;
	char type;
	size_t addr;
};

typedef struct forth {
	struct forthWord *words;
	size_t num_words, max_words;

	char *dict;
	size_t size, old_size, max_size;

	void *stack[FTH_STACKSZ];
	int sp;

	void *rstack[FTH_RSTACKSZ];
	int rsp;

	size_t pc;
	size_t immediate;

	char *buf;

	/* these apply to ACCEPT, KEY, EMIT, . and TYPE */
	void (*emit)(char);
	size_t (*accept)(char*,size_t);
	char (*key)();

	void *base;
	char mode, old_mode;
	char quit;
	char trace;
} Forth;

Forth *newForth();
void freeForth(Forth *fth);
void fth_addFunction(Forth *fth, void (*fun)(Forth*), const char *name);
void fth_addConstant(Forth *fth, void *val, const char *name);
void fth_runString(Forth *fth, const char *text);
void fth_runFile(Forth *fth, const char *filename);

char fth_chget();
void fth_chput(char c);
size_t fth_readln(char *s, size_t len);

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
