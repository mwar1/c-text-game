#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>

#define MAX_PARAMETERS 10
#define MAX_WORDS 10
extern char *params[];
extern char *otherWords[];

bool parse(char*);
bool help();

typedef struct COMMAND {
	char *pattern;
	bool (*function)(void);
} COMMAND;

#endif