#include <string.h>
#include <stdio.h>

char *dictionary[3][3] = {{"look", "observe", NULL},
			  {"go", "move", "travel"},
			  {"quit", "exit", NULL}};

void getSynonyms(char **word) {
	for (int i=0; i<3; i++) {
		for (int j=0; j<3; j++) {
			if (dictionary[i][j] != NULL && !strcmp(*word, dictionary[i][j])) {
				*word = dictionary[i][0];
			}
		}
	}
}
