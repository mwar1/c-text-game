#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "parser.h"
#include "location.h"
#include "object.h"
#include "dictionary.h"

char *verb;
char *verbSyn;
char *noun;
char *nounSyn;

bool parse(char* input) {
	verb = strtok(input, " \n");	
	getSynonyms(&verb);
	noun = strtok(NULL, " \n");
	getSynonyms(&noun);

	if (!strcmp(verb, "quit")) {
		return false;
	}
	
	if (!strcmp(verb, "look")) {
		if (noun == NULL) {
			noun = "around";
		}
		look(noun);
	} else if (!strcmp(verb, "go")) {
		go(noun);
	} else if (!strcmp(verb, "take")) {
		take(noun);
	} else if (!strcmp(verb, "drop")) {
		drop(noun);
	} else if (!strcmp(verb, "inventory")) {
		inventory();
	} else {
		printf("I'm not sure how to%s.\n", verb);
	}
	return true;
}
