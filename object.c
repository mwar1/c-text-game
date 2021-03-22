#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "location.h"
#include "object.h"
#include "npc.h"

Object *objs[8];
int numObjs = 8;

FILE *objFile;
FILE *locsFile;

void generateObjects() {
	objFile = fopen("objects.txt", "r");
	locsFile = fopen("locations.txt", "r");

	char tempTag[16];
	char *locLine = NULL;
	size_t m;

	char tags[numLocs][16];
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
		char *damage;
		char *foodPoints;
		char *locTag;
		int locIndex;

		getline(&line, &n, objFile);
		
		tag = strtok(line, "/");
		description = strtok(NULL, "/");
		weight = strtok(NULL, "/");
		int intWeight = atoi(weight);
		damage = strtok(NULL, "/");
		int intDamage = atoi(damage);
		foodPoints = strtok(NULL, "/");
		int intFoodPoints = atoi(foodPoints);
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
		objs[i]->damage = intDamage;
		objs[i]->foodPoints = intFoodPoints;
		objs[i]->location = locs[locIndex];
	}
	fclose(objFile);
}

void take(char *noun) {
	bool taken = false;
	for (int i=0; i<numObjs; i++) {
		if (noun != NULL && !strcmp(noun, objs[i]->tag) && !strcmp(objs[i]->location->tag, player->location->tag)) {
			int load = 0;
			for (int j=0; j<numObjs; j++) {
				if (objs[j]->location != NULL && !strcmp(objs[j]->location->tag, "player")) {
					load += objs[j]->weight;
				}
			}
			if ((load + objs[i]->weight) <= player->super->capacity) {
				taken = true;
				printf("%s added to inventory.\n", objs[i]->tag);
				objs[i]->location = player->super;
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
		if (noun != NULL && objs[i]->location != NULL && !strcmp(noun, objs[i]->tag) && !strcmp(objs[i]->location->tag, "player")) {
			dropped = true;
			printf("Dropped %s\n", objs[i]->tag);
			objs[i]->location = player->location;
		}
	}
	if (!dropped) {
		printf("You're not holding a %s.\n", noun);
	}
}

void look(char *noun) {
	bool looked = false;
	for (int i=0; i<numObjs; i++) {
		if (noun != NULL && objs[i]->location != NULL && !strcmp(noun, objs[i]->tag) && (!strcmp(objs[i]->location->tag, "player") || !strcmp(objs[i]->location->tag, player->location->tag))) {
			looked = true;
			printf("It's %s\n", objs[i]->description);
		}
	}
	if (!looked) {
		printf("I can't see a %s nearby.\n", noun);
	}
}

void eat(char *noun) {
	bool eaten = false;
	bool found = false;
	for (int i=0; i<numObjs; i++) {
		if (noun != NULL && !eaten) {
			if (!strcmp(noun, objs[i]->tag) && !strcmp(objs[i]->location->tag, "player")) {
				if (objs[i]->foodPoints != 0) {
					printf("Eating the %s...\n", objs[i]->tag);
					if (objs[i]->foodPoints > 0) {
						printf("You gain %i HP.\n", objs[i]->foodPoints);
					} else {
						printf("You lose %i HP.\n", objs[i]->foodPoints);
					}
					player->health += objs[i]->foodPoints;
					objs[i]->location = NULL;
					eaten = true;
				} else {
					printf("I wouldn't advise eating that.\n");
					found = true;
				}
			}
		}
	}
	if (!eaten && !found && noun != NULL) {
		printf("You're not holding a %s.\n", noun);
	} else if (!eaten && !found) {
		printf("Please specify something to eat.\n");
	}
}