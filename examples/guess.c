/* guess a random number */

#include "sforth.h"
#include <stdlib.h>
#include <time.h>

void f_rand(Forth *fth) {
	fth->stack[fth->sp++] = (void*)(size_t)rand();
}

int main() {
	Forth *fth = newForth();
	srand(time(0));
	fth_addFunction(fth, f_rand, "RAND");

	fth_runFile(fth, "guess.fth");

	freeForth(fth);
	return 0;
}
