#include <stdbool.h>
#include <string.h>
#include <stdio.h>

char *verb;
char *noun;

bool parse(char* input) {
	verb = strtok(input, " \n");
	noun = strtok(NULL, " \n");
	if (!strcmp(verb, "quit") || !strcmp(verb, "exit")) {
		return false;
	}
	return true;
}
