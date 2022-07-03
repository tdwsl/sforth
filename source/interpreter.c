#include "sforth.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **args) {
	if(argc > 2) {
		printf("usage: %s <file>\n", args[0]);
		return 0;
	}

	Forth *fth = newForth();

	if(argc == 2) {
		fth_runFile(fth, (const char*)args[1]);
		freeForth(fth);
		return 0;
	}

	printf("sforth3 - forth interpreter - tdwsl 2022\n");
	printf("type 'bye' to quit\n");

	int max = 80;
	char *s = (char*)malloc(max);
	int buf = 20;
	int len;

	while(!fth->quit) {
		len = 0;
		for(;;) {
			char c;
			scanf("%c", &c);

			if(c == '\n' || c == 0 || c == 0x0a)
				break;

			s[len++] = c;

			if(len > max-buf) {
				max += buf;
				s = (char*)realloc(s, max);
			}
		}

		if(!len)
			continue;

		s[len] = 0;
		fth_runString(fth, s);

		if(fth->rsp == 0 && fth->size == fth->old_size && fth->mode == FTHMODE_RUN && !fth->quit) {
			printf("    ok\n");
			fth_printStack(fth);
		}
	}

	printf("bye!\n");

	free(s);
	freeForth(fth);

	return 0;
}
