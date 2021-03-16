#include <string.h>
#include <stdio.h>
#include "dictionary.h"

char *dictionary[10][2] = {{"look", "observe"},
			 {"go", "move"},
			 {"quit", "exit"},
			 {"n", "north"},
			 {"e", "east"},
			 {"w", "west"},
			 {"s", "south"},
			 {"inventory", "i"},
			 {"look", "examine"}};

void getSynonyms(char **word) {
	if (*word != NULL) {
		for (int i=0; i<(sizeof(dictionary) / sizeof(dictionary[0])); i++) {
			for (int j=0; j<2; j++) {
				if (dictionary[i][j] != NULL && !strcmp(*word, dictionary[i][j])) {
					*word = dictionary[i][0];
				}
			}
		}
	}
}
