#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "location.h"
#include "object.h"

Object *objs[4];
int numObjs = 4;

FILE *objFile;
FILE *locsFile;

void generateObjects() {
	objFile = fopen("objects.txt", "r");
	locsFile = fopen("locations.txt", "r");

	char tempTag[20];
	char *locLine = NULL;
	size_t m;

	char tags[numLocs][20];
	for (int j=0; j<numLocs; j++) {
		getline(&locLine, &m, locsFile);	
		strcpy(tempTag, strtok(locLine, "/"));
		strcpy(tags[j], tempTag);
	}
	fclose(locsFile);

	for (int i=0; i<numObjs; i++) {
		objs[i] = malloc(sizeof(Object));

		char *line = NULL;
		size_t n;

		char *tag;
		char *description;
		char *weight;
		char *locTag;
		int locIndex;

		getline(&line, &n, objFile);
		tag = strtok(line, "/");
		description = strtok(NULL, "/");
		weight = strtok(NULL, "/");
		int intWeight = atoi(weight);
		locTag = strtok(NULL, "/\n");

		for (int j=0; j<numLocs; j++) {
			if (!strcmp(tags[j], locTag)) {
				locIndex = j;
				break;
			}
		}
		
		strcpy(objs[i]->tag, tag);
		strcpy(objs[i]->description, description);
		objs[i]->weight = intWeight;
		objs[i]->location = locs[locIndex];
	}
	fclose(objFile);
}

void take(char *noun) {
	bool taken = false;
	for (int i=0; i<numObjs; i++) {
		if (noun != NULL && !strcmp(noun, objs[i]->tag) && !strcmp(objs[i]->location->tag, locs[playerLocation]->tag)) {
			int load = 0;
			for (int j=0; j<numObjs; j++) {
				if (!strcmp(objs[j]->location->tag, "player")) {
					load += objs[j]->weight;
				}
			}
			if ((load + objs[i]->weight) <= player.capacity) {
				taken = true;
				printf("%s added to inventory.\n", objs[i]->tag);
				objs[i]->location = &player;
			} else {
				printf("You're carrying too much already.\n");
			}
		}
	}
	if (!taken) {
		printf("I can't see a %s nearby.\n", noun);
	}
}

void drop(char *noun) {
	bool dropped = false;
	for (int i=0; i<numObjs; i++) {
		if (noun != NULL && !strcmp(noun, objs[i]->tag) && !strcmp(objs[i]->location->tag, "player")) {
			dropped = true;
			printf("Dropped %s\n", objs[i]->tag);
			objs[i]->location = locs[playerLocation];
		}
	}
	if (!dropped) {
		printf("You're not holding a %s.\n", noun);
	}
}

void look(char *noun) {
	bool looked = false;
	for (int i=0; i<numObjs; i++) {
		if (noun != NULL && !strcmp(noun, objs[i]->tag) && (!strcmp(objs[i]->location->tag, "player") || !strcmp(objs[i]->location->tag, locs[playerLocation]->tag))) {
			looked = true;
			printf("It's %s\n", objs[i]->description);
		}
	}
	if (!looked) {
		printf("I can't see a %s nearby.\n", noun);
	}
}