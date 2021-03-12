#include <stdio.h>
#include <string.h>
#include "location.h"

typedef struct location {
	char tag[20];
	char description[128];
} location;

struct location locs[3] = {{"castle", "a grand, but run down castle"},
		           {"bridge", "a rickety, old wooden bridge, swaying in the breeze"},
		           {"observatory", "a small stone hut, with a telescope poking out from a hole in the roof"}};

int playerLocation = 0;
int numLocs = 3;

void look(char *noun) {
	if (!strcmp(noun, "around")) {
		printf("You can see %s.\n", locs[playerLocation].description);
	}
}

void go(char *noun) {
	for (int i=0; i<numLocs; i++) {
		if (!strcmp(noun, locs[i].tag)) {
			playerLocation = i;
			printf("Moving to the %s...\n", locs[i].tag);
			look("around");
		}
	}
}
