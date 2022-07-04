#include "sforth.h"
#include <stdio.h>

void fth_chput(char c) {
	fputc(c, stdout);
}

char fth_chget() {
	char c = fgetc(stdin);
	while(fgetc(stdin) != '\n');
	return c;
}

size_t fth_readln(char *s, size_t len) {
	char c = 'a';
	size_t i;
	for(i = 0; i < len && c != '\n' && c; i++) {
		c = fgetc(stdin);
		*(s++) = c;
	}
	if(i >= len)
		while(fgetc(stdin) != '\n');
	i--;
	return i;
}
