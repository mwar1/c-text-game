#ifndef NPC_H
#define NPC_H

#include "location.h"

void generateNPCs();
void talk(char*);
extern struct NPC *npcs[];
extern int numNPCs;

typedef struct NPC {
	Location *super;
	Location *location;
	int health;
} NPC;

#endif