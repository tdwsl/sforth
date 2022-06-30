#ifndef SFORTH_H
#define SFORTH_H

#include <stdlib.h>

#define FTH_STACKSZ 512
#define FTH_RSTACKSZ 1024

struct forthWord {
	char *name;
	char type;
	size_t addr;
};

typedef struct forth {
	struct forthWord *words;
	int num_words;

	char *dict;
	int size, old_size, max_size;

	void *stack[FTH_STACKSZ];
	size_t sp;

	size_t rstack[FTH_RSTACKSZ];
	size_t rsp;

	char mode;
} Forth;

Forth *newForth();
void freeForth(Forth *fth);

void fth_runString(Forth *fth, const char *text);
void fth_runFile(Forth *fth, const char *filename);

#endif