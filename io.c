#include "io.h"
#include <stdio.h>

void fth_chput(char c) {
	fputc(c, stdout);
}

char fth_chget() {
	return fgetc(stdin);
}
