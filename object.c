#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "parser.h"
#include "location.h"
#include "object.h"
#include "npc.h"

Object *objs[12];
int numObjs = 12;

FILE *objFile;
FILE *locsFile;
FILE *npcFile;

void generateObjects() {
	objFile = fopen("objects.txt", "r");
	locsFile = fopen("locations.txt", "r");
	npcFile = fopen("npcs.txt", "r");

	char *locLine = NULL;
	size_t m;

	getline(&locLine, &m, locsFile), getline(&locLine, &m, locsFile);

	char locTags[numLocs][16];
	for (int j=0; j<numLocs; j++) {
		getline(&locLine, &m, locsFile);
		strtok(locLine, "/");
		strtok(NULL, "/");
		strcpy(locTags[j], strtok(NULL, "/"));
	}
	fclose(locsFile);

	char *npcLine = NULL;
	size_t x;

	getline(&npcLine, &x, npcFile), getline(&npcLine, &x, npcFile);

	char npcTags[numNPCs][16];
	for (int k=0; k<numNPCs; k++) {
		getline(&npcLine, &x, npcFile);	
		strtok(npcLine, "/");
		strcpy(npcTags[k], strtok(NULL, "/"));
	}
	fclose(npcFile);

	char *line = NULL;
	size_t n;
	getline(&line, &n, objFile), getline(&line, &n, objFile);

	for (int i=0; i<numObjs; i++) {
		objs[i] = malloc(sizeof(Object));

		char *article, *id, *tag, *description, *weight,
			 *damage, *foodPoints, *locTag;
		int locIndex = -1;

		getline(&line, &n, objFile);
		
		article = strtok(line, "/");
		id = strtok(NULL, "/");
		tag = strtok(NULL, "/");
		description = strtok(NULL, "/");
		weight = strtok(NULL, "/");
		int intWeight = atoi(weight);
		damage = strtok(NULL, "/");
		int intDamage = atoi(damage);
		foodPoints = strtok(NULL, "/");
		int intFoodPoints = atoi(foodPoints);
		locTag = strtok(NULL, "/\n");

		for (int j=0; j<numLocs; j++) {
			if (!strcmp(locTags[j], locTag)) {
				locIndex = j;
				objs[i]->location = locs[locIndex];
				break;
			}
		}

		if (locIndex == -1) {
			for (int j=0; j<numNPCs; j++) {
				if (!strcmp(npcTags[j], locTag)) {
					locIndex = j;
					objs[i]->location = npcs[locIndex]->super;
					break;
				}
			}
		}
		
		strcpy(objs[i]->article, article);
		strcpy(objs[i]->id, id);
		strcpy(objs[i]->tag, tag);
		strcpy(objs[i]->description, description);
		objs[i]->weight = intWeight;
		objs[i]->damage = intDamage;
		objs[i]->foodPoints = intFoodPoints;
	}
	fclose(objFile);
}

bool take() {
	bool taken = false;
	for (int i=0; i<numObjs; i++) {
		if (params[0] != NULL && !strcmp(params[0], objs[i]->tag) && !strcmp(objs[i]->location->id, player->location->id)) {
			int load = 0;
			for (int j=0; j<numObjs; j++) {
				if (objs[j]->location != NULL && !strcmp(objs[j]->location->id, "player")) {
					load += objs[j]->weight;
				}
			}
			if ((load + objs[i]->weight) <= player->super->capacity) {
				taken = true;
				printf("%s added to inventory.\n", objs[i]->tag);
				objs[i]->location = player->super;
			} else {
				taken = true;
				printf("You're carrying too much already.\n");
			}
		}
	}
	if (!taken) {
		bool targetNPC = false;
		for (int j=0; j<numNPCs; j++) {
			if (!npcs[j]->alive && !strcmp(npcs[j]->location->tag, player->location->tag) && !strcmp(npcs[j]->super->tag, params[0])) {
				printf("You try to pick up the rotting corpse of the %s, but its too heavy.\n", npcs[j]->super->tag);
				targetNPC = true;
			} else if (!strcmp(npcs[j]->location->tag, player->location->tag) && !strcmp(npcs[j]->super->tag, params[0])) {
				printf("I don't think the %s would take too kindly to that.\n", npcs[j]->super->tag);
				targetNPC = true;
			}
		}
		if (!targetNPC) {
			printf("I can't see a %s nearby.\n", params[0]);
		}
	}
	return true;
}

bool drop() {
	bool dropped = false;
	for (int i=0; i<numObjs; i++) {
		if (params[0] != NULL && objs[i]->location != NULL && !strcmp(params[0], objs[i]->tag) && !strcmp(objs[i]->location->tag, "player")) {
			dropped = true;
			printf("Dropped %s\n", objs[i]->tag);
			objs[i]->location = player->location;
		}
	}
	if (!dropped) {
		printf("You're not holding a %s.\n", params[0]);
	}
	return true;
}

bool look() {
	bool looked, lookedObj, lookedLoc, lookedNPC = false;

	if (params[0] != NULL || !strcmp(params[0], "around\n")) {
		lookAround();
		looked = true;
	}

	if (!looked && params[0] != NULL) {
		for (int i=0; i<numObjs; i++) {
			if (objs[i]->location != NULL && !strcmp(params[0], objs[i]->tag) && (!strcmp(objs[i]->location->tag, "player") || !strcmp(objs[i]->location->tag, player->location->tag))) {
				lookedObj = true;
				printf("It's %s %s.\n", objs[i]->article, objs[i]->description);
			}
		}
		if (!lookedObj) {
			for (int i=0; i<numLocs; i++) {
				if (!strcmp(params[0], locs[i]->tag) && !lookedLoc) {
					for (int j=0; j<4; j++) {
						if (locs[i]->connections[j] != -1 && !strcmp(locs[locs[i]->connections[j]]->tag, player->location->tag)) {
							char *tempIntro = locs[i]->intro;
							tempIntro[0] -= 32;

							printf("%s.\n", locs[i]->intro);
							lookedLoc = true;
						}
					}
				}
			}
			if (!lookedLoc) {
				for (int i=0; i<numNPCs; i++) {
					if (!strcmp(params[0], npcs[i]->super->tag) && !strcmp(player->location->tag, npcs[i]->location->tag)) {
						printf("It's %s %s.\n", npcs[i]->super->article, npcs[i]->super->description);
						lookedNPC = true;
					}
				}
				if (!lookedNPC) {
					printf("I can't see a %s nearby.\n", params[0]);
				}
			}
		}
	}
	return true;
}

bool eat() {
	bool eaten = false;
	bool found = false;
	for (int i=0; i<numObjs; i++) {
		if (params[0] != NULL && !eaten) {
			if (!strcmp(params[0], objs[i]->tag) && !strcmp(objs[i]->location->tag, "player")) {
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
	if (!eaten && !found && params[0] != NULL) {
		bool targetNPC = false;
		for (int j=0; j<numNPCs; j++) {
			if (!npcs[j]->alive && !strcmp(npcs[j]->location->tag, player->location->tag) && !strcmp(npcs[j]->super->tag, params[0])) {
				printf("Judging by the smell, I don't think you should do that.\n");
				targetNPC = true;
			} else if (!strcmp(npcs[j]->location->tag, player->location->tag) && !strcmp(npcs[j]->super->tag, params[0])) {
				printf("I don't think the %s would take too kindly to that.\n", npcs[j]->super->tag);
				targetNPC = true;
			} else if (!strcmp(npcs[j]->super->tag, params[0])) {
				printf("I can't see a %s nearby.\n", params[0]);
				targetNPC = true;
			}
		}
		if (!targetNPC) {
			printf("You're not holding a %s.\n", params[0]);
		}
	} else if (params[0] == NULL) {
		printf("Please specify something to eat.\n");
	}
	return true;
}