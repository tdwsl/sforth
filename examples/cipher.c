/* change the 'emit' function to output text in caesar cipher */

#include "sforth.h"
#include <stdio.h>

void emit(char c) {
	const int offset = 20;

	if(c >= 'A' && c <= 'Z')
		c = (c-'A'+offset)%26 + 'A';
	else if(c >= 'a' && c <= 'z')
		c = (c-'a'+offset)%26 + 'a';

	printf("%c", c);
}

int main() {
	Forth *fth = newForth();

	const char *str = ".( Hello, world!) CR";
	fth_runString(fth, str);

	fth->emit = emit;
	fth_runString(fth, str);

	freeForth(fth);
	return 0;
}
