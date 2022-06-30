#include "io.h"
#include <stdio.h>

void chput(char c) {
	fputc(c, stdout);
}

char chget() {
	return fgetc(stdin);
}
