#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "dictionary.h"

char *dictionary[][14] = {{"look", "observe", NULL},
			 {"go", "move", "walk", "run", NULL},
			 {"quit", "exit", NULL},
			 {"n", "north", NULL},
			 {"e", "east", NULL},
			 {"w", "west", NULL},
			 {"s", "south", NULL},
			 {"inventory", "i", NULL},
			 {"look", "examine", "inspect", NULL},
			 {"fight", "kill", "attack", NULL},
			 {"help", "instructions", "rules", NULL},
			 {"health", "hp", NULL},
			 {"talk", "speak", "address", NULL},
			 {"take", "pick up", "get", NULL}};

void wordToLower(char **word) {
	char *temp = *word;
	while (temp && *temp) {
		if (*temp != ' ') *temp = tolower(*temp);
		temp++;
	}
}

void getSynonyms(char **word) {
	wordToLower(word);
	if (*word != NULL) {
		for (int i=0; i<(sizeof(dictionary) / sizeof(dictionary[0])); i++) {
			for (int j=0; dictionary[i][j] != NULL; j++) {
				if (dictionary[i][j] != NULL && !strcmp(*word, dictionary[i][j])) {
					*word = dictionary[i][0];
				}
			}
		}
	}
}
