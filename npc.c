#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "npc.h"
#include "object.h"
#include "location.h"

int numNPCs = 3;
NPC *npcs[3];

FILE *npcFile;
FILE *locsFile;

void generateNPCs() {
	npcFile = fopen("npcs.txt", "r");
	locsFile = fopen("locations.txt", "r");

	char *locLine = NULL;
	size_t m;

	char tags[numLocs][16];
	for (int j=0; j<numLocs; j++) {
		getline(&locLine, &m, locsFile);
		//printf("%s\n", locLine);	
		strcpy(tags[j], strtok(locLine, "/"));
	}
	fclose(locsFile);

	for (int i=0; i<numNPCs; i++) {
		npcs[i] = malloc(sizeof(NPC));
		npcs[i]->super = malloc(sizeof(Location));

		char *line = NULL;
		size_t n;

		char *tag;
		char *intro;
		char *description;
		char *voiceline;
		int connections[4];
		char directions[4];
		char *capacity;
		char *health;
		char *locTag;
		int locIndex;

		getline(&line, &n, npcFile);

		tag = strtok(line, "/");
		intro = strtok(NULL, "/");
		description = strtok(NULL, "/");
		voiceline = strtok(NULL, "/");
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
		capacity = strtok(NULL, "/");
		int intCapacity = atoi(capacity);
		health = strtok(NULL, "/");
		int intHealth = atoi(health);
		locTag = strtok(NULL, "\n");
		locTag[strlen(locTag)-1] = '\0';

		for (int j=0; j<numLocs; j++) {
			if (!strcmp(tags[j], locTag)) {
				locIndex = j;
				break;
			}
		}

		strcpy(npcs[i]->super->tag, tag);
		strcpy(npcs[i]->super->intro, intro);
		strcpy(npcs[i]->super->description, description);
		strcpy(npcs[i]->voiceline, voiceline);
		memcpy(npcs[i]->super->connections, connections, sizeof(int)*4);
		strcpy(npcs[i]->super->directions, directions);
		npcs[i]->super->capacity = intCapacity;
		npcs[i]->health = intHealth;
		npcs[i]->location = locs[locIndex];
	}

	fclose(npcFile);
}

void talk(char *noun) {
	bool talked = false;
	if (noun != NULL) {
		for (int i=0; i<numNPCs; i++) {
			if (!strcmp(noun, npcs[i]->super->tag)) {
				printf("%s\n", npcs[i]->voiceline);
				talked = true;
			}
		}
		if (!talked) {
			printf("The %s doesn't respond.\n", noun);
		}
	} else {
		printf("You talk to yourself.\n");
	}
}

void fight(char *noun) {
	bool found = false;
	if (noun != NULL) {
		for (int i=0; i<numNPCs; i++) {
			if (!strcmp(noun, npcs[i]->super->tag)) {
				found = true;
				char weapon[16];
				bool gotWeapon = false;

				printf("with what?\n\n>>> ");
				fgets(weapon, 48, stdin);
				weapon[strlen(weapon)-1] = '\0';

				for (int j=0; j<numObjs; j++) {
					if (!strcmp(weapon, objs[j]->tag) && !gotWeapon) {
						if (!strcmp(objs[j]->location->tag, "player")) {
							gotWeapon = true;
							if (objs[j]->damage == 0) {
								printf("You hit the %s with the %s.\nIt doesn't do much damage.\n", npcs[i]->super->tag, objs[j]->tag);
							} else {
								printf("You attack the %s with the %s.\nIt does %i damage.\n", npcs[i]->super->tag, objs[j]->tag, objs[j]->damage);
							}
						} else {
							printf("You're not holding a %s.\n", weapon);
						}
					}
				}
				if (!gotWeapon) {
					printf("use fists\n");
				}
			}
		}
		if (!found) {
			printf("I can't see a %s nearby.\n", noun);
		}
	} else {
		printf("You start attacking the air. Nothing happens...\n");
	}
}
