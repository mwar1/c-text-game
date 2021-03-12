#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "location.h"

char *verb;
char *noun;

bool parse(char* input) {
	verb = strtok(input, " \n");
	noun = strtok(NULL, " \n");
	if (!strcmp(verb, "quit") || !strcmp(verb, "exit")) {
		return false;
	}

	if (!strcmp(verb, "look")) {
		look(noun);
	} else if (!strcmp(verb, "go")) {
		go(noun);
	}
	return true;
}
