#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>

#define MAX_PARAMETERS 10
extern char *params[];

bool parse(char*, FILE*);
bool help();

typedef struct COMMAND {
	char *pattern;
	bool (*function)(void);
} COMMAND;

#endif