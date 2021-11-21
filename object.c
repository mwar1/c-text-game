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

int *getContainersInLoc(bool isInventory) {
	static int contIs[10];
	int x=0;
	for (int i=0; i<numContainers; i++) {
		if (isInventory) {
			if (containers[i]->super->location != NULL && !strcmp(containers[i]->super->location->tag, "player")) {
				contIs[x] = i;
				x++;
			}
		} else {
			if (containers[i]->super->location != NULL && !strcmp(containers[i]->super->location->id, player->location->id)) {
				contIs[x] = i;
				x++;
			}
		}
	}	
	contIs[x] = 999;
	return contIs;
}

bool take() {
	// Check if the user wants to take from a specific container
	if (strlen(params[1]) != 0) {
		return takeFrom();
	}

	int load = 0;
	for (int j=0; j<numObjs; j++) {
		if (objs[j]->location != NULL && !strcmp(objs[j]->location->id, "player")) {
			load += objs[j]->weight;
		}
	}
	for (int j=0; j<numContainers; j++) {
		if (containers[j]->super->location != NULL && !strcmp(containers[j]->super->location->id, "player")) {
			load += containers[j]->super->weight;
		}
	}

	bool taken = false;
	for (int i=0; i<numObjs && !taken; i++) {
		if (strlen(params[0]) != 0 && !strcmp(params[0], objs[i]->tag) && !strcmp(objs[i]->location->id, player->location->id)) {
			taken = true;
			if ((load + objs[i]->weight) <= player->super->capacity) {
				printf("%s added to inventory.\n", objs[i]->tag);
				objs[i]->location = player->super;
			} else if (load == 0) {
				printf("The %s is too heavy to pick up.\n", objs[i]->tag);
			} else {
				printf("You're carrying too much already.\n");
			}
		}
	}
	if (!taken) {
		for (int i=0; i<numContainers && !taken; i++) {
			if (params[0] != NULL && !strcmp(params[0], containers[i]->super->tag) && !strcmp(containers[i]->super->location->id, player->location->id)) {
				taken = true;
				if ((load + containers[i]->super->weight) <= player->super->capacity) {
					printf("%s added to inventory.\n", containers[i]->super->tag);
					containers[i]->super->location = player->super;
				} else if (load == 0) {
					printf("The %s is too heavy to pick up.\n", containers[i]->super->tag);
				} else {
					printf("You're carrying too much already.\n");
				}
			}
			if (!taken) {
				params[1] = containers[i]->super->tag;
				return takeFrom();
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
			printf("Dropped %s.\n", objs[i]->tag);
			objs[i]->location = player->location;
		}
	}
	if (!dropped) {
		for (int i=0; i<numContainers; i++) {
			if (params[0] != NULL && containers[i]->super->location != NULL && !strcmp(params[0], containers[i]->super->tag) && !strcmp(containers[i]->super->location->tag, "player")) {
				dropped = true;
				printf("Dropped %s.\n", containers[i]->super->tag);
				containers[i]->super->location = player->location;
			}
		}
	}
	if (!dropped) {
		printf("You're not holding a %s.\n", params[0]);
	}
	return true;
}

bool look() {
	bool looked, lookedObj, lookedLoc, lookedNPC = false;

	if (params[0] == NULL || !strcmp(params[0], "around")) {
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
			for (int i=0; i<numContainers; i++) {
				if (containers[i]->super->location != NULL && !strcmp(params[0], containers[i]->super->tag) && (!strcmp(containers[i]->super->location->tag, "player") || !strcmp(containers[i]->super->location->tag, player->location->tag))) {
					lookedObj = true;
					char *openStatus = containers[i]->open ? "open" : "closed";
					printf("Its %s %s, currently %s.\n", containers[i]->super->article, containers[i]->super->description, openStatus);
				}
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

bool interactContainer() {
	bool foundCont = false;
	bool namedCont = false;
	if (strlen(params[1]) == 0) {
		printf("Please specify what you would like to %s.\n", otherWords[0]);
		return true;
	} else if (!strcmp(params[1], "door")) {
		return interactDoor();
	}
	for (int i=0; i<numContainers; i++) {
		if (!strcmp(params[1], containers[i]->super->tag)) namedCont = true;
		if (!strcmp(containers[i]->super->tag, params[1]) && (!strcmp(containers[i]->super->location->tag, player->location->tag) || !strcmp(containers[i]->super->location->tag, "player")) && !foundCont) {
			foundCont = true;
			Container *currentCont = containers[i];
			if (!strcmp(otherWords[0], "open")) {
				if (!currentCont->open && !currentCont->locked) {
					currentCont->open = true;
					printf("The %s opens.\n", currentCont->super->tag);
				} else if (!currentCont->locked) {
					printf("The %s is already open.\n", currentCont->super->tag);
				} else {
					printf("The %s is locked. You must unlock it before it can be opened.\n", currentCont->super->tag);
				}
			} else if (!strcmp(otherWords[0], "close")) {
				if (!currentCont->open) {
					printf("The %s is already closed.\n", currentCont->super->tag);
				} else {
					currentCont->open = false;
					printf("The %s swings shut.\n", currentCont->super->tag);
				}
			} else if (!strcmp(otherWords[0], "lock")) {
				if (currentCont->unlockWith[0] == '0') {
					printf("The %s doesn't have a lock.\n", currentCont->super->tag);
				} else {
					if (!currentCont->locked) {
						bool foundKey = false;;
						for (int k=0; k<numObjs && !foundKey; k++) {
							if (!strcmp(objs[k]->id, currentCont->unlockWith) && !strcmp(objs[k]->location->tag, "player")) {
								printf("You lock the %s.\n", currentCont->super->tag);
								currentCont->locked = true;
								foundKey = true;
							}
						}
						if (!foundKey) {
							printf("You need to find something to lock it with first.\n");
						}
					} else {
						printf("The %s is already locked.\n", currentCont->super->tag);
					}
				}
			} else if (!strcmp(otherWords[0], "unlock")) {
				if (currentCont->locked) {
					bool foundKey = false;
					for (int k=0; k<numObjs && !foundKey; k++) {
						currentCont->unlockWith[strcspn(currentCont->unlockWith, "\r")] = 0;
						if (!strcmp(objs[k]->id, currentCont->unlockWith) && !strcmp(objs[k]->location->tag, "player")) {
							printf("The lock on the %s clicks open.\n", currentCont->super->tag);
							currentCont->locked = false;
							foundKey = true;
						}
					}
					if (!foundKey) {
						printf("You need to find something to unlock it with first.\n");
					}
				} else {
					printf("The %s is already unlocked.\n", currentCont->super->tag);
				}
			}
		}
	}
	if (namedCont && !foundCont) {
		printf("I can't see a %s nearby.\n", params[1]);
	} else {
		printf("I'm not sure how to open a %s.\n", params[1]);
	}
	return true;
}

bool takeFrom() {
	int indexOfObj = -1;
	int indexOfBox = -1;

	bool foundObj = false;
	for (int i=0; i<numObjs && !foundObj; i++) {
		if (objs[i]->location != NULL && !strcmp(objs[i]->tag, params[0])) {
			indexOfObj = i;
			foundObj = true;
		}
	}

	bool foundCont = false;
	for (int i=0; i<numContainers && !foundCont; i++) {
		if ((!strcmp(containers[i]->super->location->id, player->location->id) || !strcmp(containers[i]->super->location->id, "player")) && !strcmp(containers[i]->super->tag, params[1])) {
			indexOfBox = i;
			foundCont = true;
		}
	}

	if (indexOfBox == -1) {
		printf("I can't see a %s nearby.\n", params[1]);
		return true;
	} else if (!containers[indexOfBox]->open) {
		printf("You must open the %s first.\n", params[1]);
		return true;
	}

	if (strcmp(objs[indexOfObj]->location->id, containers[indexOfBox]->contents->id)) {
		printf("There is no %s in the %s.\n", params[0], params[1]);
	} else {
		int currentLoad = 0;
		for (int i=0; i<numObjs; i++) {
			if (!strcmp(objs[i]->location->id, "player")) {
				currentLoad += objs[i]->weight;
			}
		}

		if (currentLoad + objs[indexOfObj]->weight <= player->super->capacity) {
			printf("You take the %s from the %s.\n", params[0], params[1]);
			objs[indexOfObj]->location = player->super;
			containers[indexOfBox]->super->weight -= objs[indexOfObj]->weight;
		} else {
			printf("You are carrying too much already.\n");
		}
	}
	return true;
}

bool put() {
	if (strlen(params[0]) == 0 && strlen(params[1]) == 0) {
		printf("Please specify what you want to put, and where.\n");
		return true;
	} else if (strlen(params[1]) == 0) {
		printf("Please specify where you want to put the %s.\ne.g. put <noun> in <noun>.\n", params[0]);
		return true;
	}

	bool foundObj, foundCont;
	int tempWeight;
	foundObj = foundCont = false;
	Container *aContainer = NULL;
	Object *aObj = NULL;

	// Search for the object to be put in the container
	// i.e. A in 'put A in B'
	for (int i=0; i<numObjs && !foundObj; i++) {
		if (objs[i]->location != NULL && !strcmp(params[0], objs[i]->tag) && (!strcmp(objs[i]->location->tag, player->location->tag) || !strcmp(objs[i]->location->tag, "player"))) {
			foundObj = true;
			aObj = objs[i];
			tempWeight = aObj->weight;
		}
	}
	if (!foundObj) {
		for (int i=0; i<numContainers && !foundObj; i++) {
			if (!strcmp(params[0], containers[i]->super->tag) && (!strcmp(containers[i]->super->location->tag, player->location->tag) || !strcmp(containers[i]->super->location->tag, "player"))) {
				foundObj = true;
				aContainer = containers[i];
				tempWeight = aContainer->super->weight;
			}
		}
	}
	if (!foundObj) {
		printf("I can't see a %s nearby.\n", params[0]);
		return true;
	}

	// Search for the container, specified in params[1]
	for (int i=0; i<numContainers && !foundCont; i++) {
		if (!strcmp(params[1], containers[i]->super->tag) && (!strcmp(containers[i]->super->location->tag, player->location->tag) || !strcmp(containers[i]->super->location->tag, "player"))) {
			foundCont = true;

			// Check if the container is open
			if (!containers[i]->open) {
				printf("You must open the %s first.\n", params[1]);
				return true;
			}

			// Check if the object is too large to fit in the container
			int currentLoad = 0;
			for (int j=0; j<numObjs; j++) {
				if (objs[j]->location != NULL && !strcmp(objs[j]->location->id, containers[i]->super->id)) {
					currentLoad += objs[j]->weight;
				}
			}
			for (int j=0; j<numContainers; j++) {
				if (containers[j]->super->location != NULL && !strcmp(containers[j]->super->location->id, containers[i]->super->id)) {
					currentLoad += containers[j]->super->weight;
				}
			}

			if (currentLoad + tempWeight > containers[i]->capacity) {
				printf("The %s is too full to fit the %s.\n", params[1], params[0]);
			} else {
				if (aContainer && !strcmp(aContainer->super->id, containers[i]->super->id)) {
					printf("You can't put the %s in itself.\n", params[0]);
					return true;
				}
				printf("You put the %s in the %s.\n", params[0], params[1]);
				if (aObj) {
					aObj->location = containers[i]->contents;
				} else {
					aContainer->super->location = containers[i]->contents;
				}
				containers[i]->super->weight += tempWeight;
			}
		}
	}
	if (!foundCont) {
		printf("I can't see a %s nearby, or it isn't a valid container.\n", params[1]);
	}

	return true;
}

bool lookIn() {
	bool foundCont = false;

	if (!strlen(params[0])) {
		printf("Please specify what you want to look in.\n");
		return true;
	}

	for (int i=0; i<numContainers && !foundCont; i++) {
		if (containers[i]->super->location != NULL && (!strcmp(containers[i]->super->tag, params[0]) || !strcmp(containers[i]->super->tag, "player"))) {
			foundCont = true;

			if (!containers[i]->open) {
				printf("You need to open the %s first.\n", containers[i]->super->tag);
				return true;
			}

			bool empty = true;

			printf("In the %s, there is:\n", containers[i]->super->tag);
			for (int j=0; j<numObjs; j++) {
				if (objs[j]->location != NULL && !strcmp(objs[j]->location->id, containers[i]->super->id)) {
					empty = false;
					printf("%s %s\n", objs[j]->article, objs[j]->tag);
				}
			}
			if (empty) {
				printf("nothing\n");
			}
		}
	}

	return true;
}
