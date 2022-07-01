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

struct forthWord {
	char *name;
	char type;
	size_t addr;
};

typedef struct forth {
	struct forthWord *words;
	int num_words;

	char *dict;
	size_t size, old_size, max_size;

	void *stack[FTH_STACKSZ];
	size_t sp;

	void *rstack[FTH_RSTACKSZ];
	size_t rsp;

	size_t pc;
	size_t immediate;

	char *buf;

	void *base;
	char mode, old_mode;
	char quit;
	char trace;
} Forth;

Forth *newForth();
void freeForth(Forth *fth);
void fth_runString(Forth *fth, const char *text);
void fth_runFile(Forth *fth, const char *filename);

#endif
