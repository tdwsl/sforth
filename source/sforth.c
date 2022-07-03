#include "sforth.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

Forth *newForth() {
	Forth *fth = (Forth*)malloc(sizeof(Forth));
	fth->words = 0;
	fth->num_words = 0;
	fth->size = 0;
	fth->old_size = 0;
	fth->max_size = 20;
	fth->dict = (char*)malloc(fth->max_size);
	fth->sp = 0;
	fth->rsp = 0;
	fth->mode = FTHMODE_RUN;
	fth->quit = 0;
	fth->trace = 0;
	fth->base = (void*)10;

	fth_addDefaultWords(fth);

	return fth;
}

void freeForth(Forth *fth) {
	for(int i = 0; i < fth->num_words; i++)
		free(fth->words[i].name);
	if(fth->words)
		free(fth->words);

	if(fth->dict)
		free(fth->dict);

	free(fth);
}

void fth_addFunction(Forth *fth, void (*fun)(Forth*), const char *name) {
	fth_addWord(fth, name, FTHWORD_NORMAL);
	fth_addIns(fth, FTH_FUNCTION);
	fth_addVal(fth, fun);
	fth_addIns(fth, FTH_RET);
	fth->old_size = fth->size;
}

void fth_addConstant(Forth *fth, void *val, const char *name) {
	fth_addWord(fth, name, FTHWORD_NORMAL);
	fth_addIns(fth, FTH_PUSH);
	fth_addVal(fth, val);
	fth_addIns(fth, FTH_RET);
	fth->old_size = fth->size;
}

char fth_isInteger(Forth *fth, char *s, intptr_t *n) {
	*n = 0;
	char neg = 0;
	size_t base = (size_t)fth->base;

	if(!s[0])
		return 0;

	for(size_t i = 0; s[i]; i++) {
		if(s[i] == '-') {
			if(i == 0 && s[i+1])
				neg = 1;
			else
				return 0;
		}
		else if(s[i] >= '0' && s[i] <= '9')
			*n = (*n)*base + s[i]-'0';
		else if(s[i]-'A'+10 < base)
			*n = (*n)*base + s[i]-'A'+10;
		else
			return 0;
	}

	if(neg)
		*n *= -1;

	return 1;
}

void fth_runToken(Forth *fth, char *s) {
	struct forthWord *wd;
	size_t i, j;
	intptr_t n;
	if(fth->quit)
		return;
	switch(fth->mode) {
	case FTHMODE_GETNEXT:
		fth->buf = malloc(strlen(s)+1);
		strcpy(fth->buf, s);
		fth->mode = fth->old_mode;
		fth_run(fth);
		break;
	case FTHMODE_WORDNAME:
		fth_capitalize(s);
		fth_addWord(fth, s, FTHWORD_NORMAL);
		fth->mode = FTHMODE_WORD;
		break;
	case FTHMODE_WORD:
		if(strcmp(s, ":") == 0) {
			printf("cannot define word within word !\n");
			break;
		}
		if(strcmp(s, ";") == 0) {
			fth_addIns(fth, FTH_RET);
			fth->old_size = fth->size;
			fth->mode = FTHMODE_RUN;
			break;
		}
	case FTHMODE_RUN:
		if(strcmp(s, ":") == 0) {
			fth->mode = FTHMODE_WORDNAME;
			break;
		}
		fth_capitalize(s);
		wd = fth_findWord(fth, s);
		if(wd) {
			switch(wd->type) {
			case FTHWORD_IMMEDIATE:
				fth->pc = wd->addr;
				fth->immediate = 1;
				fth_run(fth);
				break;
			case FTHWORD_INSERT:
				for(i = wd->addr; fth->dict[i] != FTH_RET; fth_nextInstruction(fth, &i));
				for(j = wd->addr; j < i; j++)
					fth_addIns(fth, fth->dict[j]);
				break;
			case FTHWORD_NORMAL:
				fth_addIns(fth, FTH_CALL);
				fth_addVal(fth, (void*)wd->addr);
				break;
			}
		}
		else if(fth_isInteger(fth, s, &n)) {
			if(n >= -128 && n <= 127) {
				fth_addIns(fth, FTH_PUSHB);
				fth_addIns(fth, (char)n);
			}
			else {
				fth_addIns(fth, FTH_PUSH);
				fth_addVal(fth, (void*)n);
			}
		}
		else
			printf("%s ?\n", s);
		break;
	}

	if(fth->mode == FTHMODE_RUN && fth->rsp == 0 && fth->size != fth->old_size) {
		fth->pc = fth->old_size;
		fth->immediate = 0;
		fth_run(fth);
		fth->size = fth->old_size;
	}
}

void fth_runString(Forth *fth, const char *text) {
	int max = 100;
	int buf = 30;
	char *s = (char*)malloc(max);
	int len = 0;

	char quote = 0;
	char comment = 0;

	for(const char *c = text; ; c++) {
		if(*c == '\n' || *c == 0) {
			if(quote || len) {
				s[len] = 0;
				char wq = quote;

				if(!s[1] && (s[0]=='('||s[0]=='\\')) {
					comment = s[0];
				}
				else if(strlen(s) == 2)
					if((s[0]=='.'||s[0]=='s'||s[0]=='S') && (s[1]=='"'||s[1]=='('||s[1]=='\'')) {
						quote = s[1];
						s[1] = '"';
						if(quote == '(')
							quote = ')';
					}

				if(!comment)
					fth_runToken(fth, s);
				if(quote && !wq)
					fth_runToken(fth, (char*)"");
			}

			len = 0;
			quote = 0;
			comment = 0;

			if(*c == 0)
				break;
		}

		else if(comment) {
			if(comment == '(' && *c == ')')
				comment = 0;
			continue;
		}

		else if(quote) {
			if(*c == quote) {
				s[len] = 0;
				fth_runToken(fth, s);
				len = 0;
				quote = 0;
			}
			else
				s[len++] = *c;
		}

		else if(*c == ' ' || *c == '\t') {
			if(len) {
				s[len] = 0;
				len = 0;

				if(!s[1] && (s[0]=='('||s[0]=='\\')) {
					comment = s[0];
					continue;
				}
				else if(strlen(s) == 2)
					if((s[0]=='.'||s[0]=='s'||s[0]=='S') && (s[1]=='"'||s[1]=='('||s[1]=='\'')) {
						quote = s[1];
						s[1] = '"';
						if(quote == '(')
							quote = ')';
					}

				fth_runToken(fth, s);
			}
		}

		else
			s[len++] = *c;

		if(len > max-buf) {
			max += buf;
			s = (char*)realloc(s, max);
		}
	}

	free(s);
}

void fth_runFile(Forth *fth, const char *filename) {
	FILE *fp = fopen(filename, "r");
	if(!fp) {
		printf("failed to open %s !\n", filename);
		return;
	}

	int max = 100;
	int buf = 30;
	char *s = (char*)malloc(max);
	int len = 0;

	while(!feof(fp)) {
		char c = fgetc(fp);
		s[len++] = c;
		if(len > max-buf) {
			max += buf;
			s = (char*)realloc(s, max);
		}
	}
	fclose(fp);
	s[len-1] = 0;

	fth_runString(fth, s);
	free(s);
	return;
}
