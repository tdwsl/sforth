sforth3 - FORTH interpreter - tdwsl 2022

This is a simple, incomplete FORTH interpreter written in C.

It can be embedded in a C program using the following functions:

Forth* newForth()
void freeForth(Forth *fth)
void fth_addFunction(Forth *fth, void(*fun)(Forth*), const char *name)
void fth_addConstant(Forth *fth, void *val, const char *name)
void fth_runString(Forth *fth, const char *text)
void fth_runFile(Forth *fth, const char *filename)

For the most part, these are pretty self explanitory. Examples have been
included in the 'examples' folder.

Note: 'I' doesn't work poperly at the moment.

This software uses the MIT license. Check out 'license.txt' for more info.
