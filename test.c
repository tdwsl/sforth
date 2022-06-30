#include "sforth.h"

int main() {
	Forth *fth = newForth();
	fth_runFile(fth, "test.f");
	fth_printWord(fth, &fth->words[fth->num_words-1]);
	freeForth(fth);
	return 0;
}
