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
