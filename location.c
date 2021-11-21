#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "parser.h"
#include "location.h"
#include "object.h"
#include "npc.h"
#include "dictionary.h"
#include "input.h"

int playerLocation = 0;
int numConnections;

int numLocs = 20;
Location *locs[20];

int numContainers = 1;
struct Container *containers[1];

Location *currentLoc;
NPC *player;

void generateContainers() {
	FILE *contFile = fopen("containers.txt", "r");

	char *line = NULL;
	size_t n;
	getline(&line, &n, contFile), getline(&line, &n, contFile);

	for (int i=0; i<numContainers; i++) {
		containers[i] = malloc(sizeof(Container));
		containers[i]->super = malloc(sizeof(Object));
		containers[i]->super->location = malloc(sizeof(Location));
		containers[i]->contents = malloc(sizeof(Location));


		getline(&line, &n, contFile);

		char *article, *id, *tag, *description,
			 *capacity, *weight, *locked, *open, *locID, *unlockID;

		article = strtok(line, "/");
		id = strtok(NULL, "/");
		tag = strtok(NULL, "/");
		description = strtok(NULL, "/");
		capacity = strtok(NULL, "/");
		int intCapacity = atoi(capacity);
		weight = strtok(NULL, "/");
		int intWeight = atoi(weight);
		locked = strtok(NULL, "/");
		int intLocked = atoi(locked);
		open = strtok(NULL, "/");
		int intOpen = atoi(open);
		locID = strtok(NULL, "/");
		unlockID = strtok(NULL, "\n");

		for (int i=0; i<numLocs; i++) {
			if (!strcmp(locID, locs[i]->id)) {
				containers[i]->super->location = locs[i];
			}
		}

		strcpy(containers[i]->super->article, article);
		strcpy(containers[i]->super->id, id);
		strcpy(containers[i]->super->tag, tag);
		strcpy(containers[i]->super->description, description);
		containers[i]->capacity = intCapacity;
		containers[i]->super->weight = intWeight;
		containers[i]->locked = intLocked;
		containers[i]->open = intOpen;
		strcpy(containers[i]->unlockWith, unlockID);

		strcpy(containers[i]->contents->article, article);
		strcpy(containers[i]->contents->id, id);
		strcpy(containers[i]->contents->tag, tag);
		containers[i]->contents->locked = containers[i]->contents->isDoor = false;
		containers[i]->contents->open = true;
	}
	fclose(contFile);
}

void generateLocations() {
	FILE *locFile = fopen("locations.txt", "r");

	char *line = NULL;
	size_t n;
	getline(&line, &n, locFile), getline(&line, &n, locFile);

	for (int i=0; i<numLocs; i++) {
		locs[i] = malloc(sizeof(Location));

		getline(&line, &n, locFile);

		char *article, *id, *tag, *intro, *description,
			 *doorOrLoc, *locked, *open, *unlockWith;
		char directions[4];
		int connections[4];

		doorOrLoc = strtok(line, "/");
		article = strtok(NULL, "/");
		id = strtok(NULL, "/");
		tag = strtok(NULL, "/");
		intro = strtok(NULL, "/");
		description = strtok(NULL, "/");
		char *tempConns[] = {strtok(NULL, "/"), strtok(NULL, "/"), strtok(NULL, "/"), strtok(NULL, "/")};
		for (int i=0; i<4; i++) {
			char *tempP;
			long ret;

			ret = strtol(tempConns[i], &tempP, 10);
			connections[i] = ret;
		}

		directions[0] = (char) *strtok(NULL, "/");
		directions[1] = (char) *strtok(NULL, "/");
		directions[2] = (char) *strtok(NULL, "/");
		directions[3] = (char) *strtok(NULL, "/");
		locked = strtok(NULL, "/");
		open = strtok(NULL, "/");

		if (!strcmp(doorOrLoc, "door")) {
			unlockWith = strtok(NULL, "\n");

			strcpy(locs[i]->unlockWith, unlockWith);
			locs[i]->isDoor = true;
			locs[i]->open = atoi(open);
			locs[i]->locked = atoi(locked);
		} else {
			strcpy(locs[i]->unlockWith, "null");
			locs[i]->locked = false;
			locs[i]->isDoor = false;
			locs[i]->open = true;
		}

		strcpy(locs[i]->article, article);
		strcpy(locs[i]->id, id);
		strcpy(locs[i]->tag, tag);
		strcpy(locs[i]->intro, intro);
		strcpy(locs[i]->description, description);
		memcpy(locs[i]->connections, connections, sizeof(int)*4);
		strcpy(locs[i]->directions, directions);
		locs[i]->capacity = 9999;
	}
	fclose(locFile);
	generateContainers();
}

void createPlayer() {
	player = malloc(sizeof(NPC));
	player->super = malloc(sizeof(Location));
	int tempConns[4] = {-1, -1, -1, -1};
	char tempDirs[4] = {'x', 'x', 'x', 'x'};

	strcpy(player->super->id, "player");
	strcpy(player->super->tag, "player");
	strcpy(player->super->intro, "you");
	strcpy(player->super->description, "you");
	memcpy(player->super->connections, tempConns, sizeof(int)*4);
	strcpy(player->super->directions, tempDirs);
	player->super->capacity = 30;

	player->location = locs[0];
	strcpy(player->voiceline, ".");
	player->health = 50;
}

bool lookAround() {
	numConnections = 0;
	for (int i=0; i<4; i++) {
		if (player->location->connections[i] != -1) {
			numConnections += 1;
		}
	}
	printf("You are %s.\n\n", player->location->description);
	for (int i=0; i<numConnections; i++) {
		if (player->location->connections[i] != -1) {
			if (player->location->directions[i] == 'n') {
				printf("To the north ");
			} else if (player->location->directions[i] == 'e') {
				printf("To the east ");
			} else if (player->location->directions[i] == 's') {
				printf("To the south ");
			} else {
				printf("To the west ");
			}
			if (locs[player->location->connections[i]]->isDoor && locs[player->location->connections[i]]->open) {
				if (!strcmp(locs[locs[player->location->connections[i]]->connections[0]]->tag, player->location->tag)) {
					printf("%s, through which you can see %s.\n", locs[player->location->connections[i]]->intro, locs[locs[player->location->connections[i]]->connections[1]]->intro);
				} else {
					printf("%s, through which you can see %s.\n", locs[player->location->connections[i]]->intro, locs[locs[player->location->connections[i]]->connections[0]]->intro);
				}
			} else if (locs[player->location->connections[i]]->isDoor){
				printf("%s, which is closed.\n", locs[player->location->connections[i]]->intro);
			} else {
				printf("%s.\n", locs[player->location->connections[i]]->intro);
			}
		}
	}
	
	printf("\nOn the floor there is:\n");
	bool isObject = false;

	int *p = getObjsInLoc(false);
	for (int i=0; i<10; i++) {
		if (*(p+i) == 999) break;
		printf("%s %s\n", objs[*(p+i)]->article, objs[*(p+i)]->tag);
		isObject = true;
	}
	p = getContainersInLoc(false);
	for (int i=0; i<10; i++) {
		if (*(p+i) == 999) break;
		Object *temp = containers[*(p+i)]->super;
		printf("%s %s\n", temp->article, temp->tag);
		isObject = true;
	}
	if (!isObject) {
		printf("nothing.\n");
	}

	p = getNPCsInLoc(false);
	for (int i=0; i<10; i++) {
		if (*(p+i) == 999) break;
		if (npcs[*(p+i)]->alive) {
			bool hasWeapon = false;
			for (int j=0; j<numObjs; j++) {
				if (!strcmp(objs[j]->location->id, npcs[*(p+i)]->super->id)) {
					hasWeapon = true;
					printf("\nYou can also see %s %s, wielding %s %s.\n", npcs[*(p+i)]->super->article, npcs[*(p+i)]->super->description, objs[j]->article, objs[j]->tag);
				}
			}
			if (!hasWeapon) {
				printf("\nYou can also see %s %s.\n", npcs[*(p+i)]->super->article, npcs[*(p+i)]->super->description);
			}
		} else {
			printf("\nThe body of a %s lies on the floor.\n", npcs[*(p+i)]->super->tag);
		}
	}
	return true;
}

bool go() {
	bool moved = false;
	bool locked = false;
	if (params[0] != NULL) {
		for (int i=0; i<4; i++) {
			if (!moved && player->location->connections[i] != -1 && strlen(params[0]) == 1 && (int) params[0][0] == player->location->directions[i]) {
				if (locs[player->location->connections[i]]->isDoor) {
					if (!locs[player->location->connections[i]]->open) {
						locked = true;
						printf("The door is closed. You should open it first.\n");
					} else if (locs[player->location->connections[i]]->locked){
						locked = true;
						printf("The door is locked. Find something to unlock it first.\n");
					} else {
						if (!strcmp(locs[locs[player->location->connections[i]]->connections[0]]->tag, player->location->tag)) {
							player->location = locs[locs[player->location->connections[i]]->connections[1]];
						} else {
							player->location = locs[locs[player->location->connections[i]]->connections[0]];
						}
					}
				} else {
					player->location = locs[player->location->connections[i]];
				}
				if (!locked) {
					lookAround();
				}
				moved = true;
			}		}
		if (!moved && !locked) {
			if (strlen(params[0]) == 1 && ((int) params[0][0] == 'n' || (int) params[0][0] == 'e' || (int) params[0][0] == 's' || (int) params[0][0] == 'w')) {
				printf("You can't move in that direction at the moment.\n");
			} else { 
				printf("I don't know how to move in that direction.\n");
			}
		}
	} else {
		printf("Please specify a direction to move in.\n");
	}
	currentLoc = getPlayerLocation();
	return true;
}

int *getObjsInLoc(bool isInventory) {
	static int objIs[10];
	int x=0;
	for (int i=0; i<numObjs; i++) {
		if (isInventory) {
			if (objs[i]->location != NULL && !strcmp(objs[i]->location->tag, "player")) {
				objIs[x] = i;
				x++;
			}
		} else {
			if (objs[i]->location != NULL && !strcmp(objs[i]->location->id, player->location->id)) {
				objIs[x] = i;
				x++;
			}
		}
	}	
	objIs[x] = 999;
	return objIs;
}

int *getNPCsInLoc(bool requireAlive) {
	static int NPCIs[10];
	int x=0;
	for (int i=0; i<numNPCs; i++) {
		if ((npcs[i]->location != NULL && !strcmp(npcs[i]->location->id, player->location->id)) && ((requireAlive && npcs[i]->alive) || !requireAlive)) {
			NPCIs[x] = i;
			x++;
		}
	}
	NPCIs[x] = 999;
	return NPCIs;	
}

Location *getPlayerLocation() {
	for (int i=0; i<numLocs; i++) {
		if (!strcmp(locs[i]->tag, player->location->tag)) {
			return locs[i];
		}
	}
}

bool interactDoor() {
	char *direction;
	bool correctDirection = false;

	if (strlen(params[0]) == 0) {
		direction = malloc(8);

		printf("In which direction?\n");
		getInput(direction, 8);
		direction[strlen(direction)-1] = '\0';
		getSynonyms(&direction);
	} else {
		direction = params[0];
	}
	if (strlen(direction) == 1 && (direction[0] == 'n' || direction[0] == 'e' || direction[0] == 'w' || direction[0] == 's')) {
		for (int i=0; i<numLocs; i++) {
			if (!strcmp(locs[i]->tag, player->location->tag)) {
				for (int j=0; j<4; j++) {
					if (locs[i]->connections[j] != -1 && locs[locs[i]->connections[j]]->isDoor && direction[0] == locs[i]->directions[j] && !correctDirection) {
						correctDirection = true;
						Location *currentDoor = locs[locs[i]->connections[j]];
						if (!strcmp(otherWords[0], "open")) {
							if (!currentDoor->locked && !currentDoor->open) {
								printf("The door swings open.\n");
								currentDoor->open = true;
							} else if (!currentDoor->locked) {
								printf("The door is already open.\n");
							} else {
								printf("That door is locked, you must unlock it before it can be opened.\n");
							}
						} else if (!strcmp(otherWords[0], "close")) {
							if (currentDoor->open) {
								printf("The door swings shut.\n");
								currentDoor->open = false;
							} else {
								printf("The door is already closed.\n");
							}
						} else if (!strcmp(otherWords[0], "lock")) {
							bool done = false;
							if (!currentDoor->locked) {
								for (int k=0; k<numObjs; k++) {
									if (!strcmp(objs[k]->id, currentDoor->unlockWith) && !currentDoor->open && !strcmp(objs[k]->location->id, "player")) {
										printf("You lock the door.\n");
										done = true;
										currentDoor->locked = true;
									} else if (!strcmp(objs[k]->id, currentDoor->unlockWith) && !strcmp(objs[k]->location->id, "player") && !done) {
										printf("Close the door first.\n");
										done = true;
									}
								}
								if (!done) {
									printf("You need to find something to lock it with first.\n");
								}
							} else {
								printf("That door is already locked.\n");
							}
						} else if (!strcmp(otherWords[0], "unlock")) {
							bool done = false;
							if (currentDoor->locked) {
								for (int k=0; k<numObjs; k++) {
									if (!strcmp(objs[k]->id, currentDoor->unlockWith) && !strcmp(objs[k]->location->id, "player")) {
										printf("The door is now unlocked.\n");
										done = true;
										currentDoor->locked = false;
									}
								}
								if (!done) {
									printf("You need to find something to unlock it with first.\n");
								}
							} else {
								printf("That door is already unlocked.\n");
							}
						} 
					} else if (!correctDirection) {
						printf("There isn't a door in that direction.\n");
					}
				}
			}
		}
		if (!correctDirection) {
			printf("There isn't a door in that direction.\n");
		}
	} else {
		printf("I'm not sure which direction you mean.\n");
	}
	return true;
}

bool inventory() {
	bool holding = false;
	printf("You are holding:\n");

	int *p = getObjsInLoc(true);
	for (int i=0; i<10; i++) {
		if (*(p+i) == 999) break;
		holding = true;
		printf("%s %s\n", objs[*(p+i)]->article, objs[*(p+i)]->tag);
	}
	p = getContainersInLoc(true);
	for (int i=0; i<10; i++) {
		if (*(p+i) == 999) break;
		holding = true;
		printf("%s %s\n", containers[*(p+i)]->super->article, containers[*(p+i)]->super->tag);
	}

	if (!holding) {
		printf("nothing.\n");
	}

	return true;
}
