#ifndef NPC_H
#define NPC_H

#include "location.h"

void generateNPCs();
void talk(char*);
void fight(char*);
extern struct NPC *npcs[];
extern int numNPCs;

typedef struct NPC {
	Location *super;
	Location *location;
	char voiceline[80];
	int health;
} NPC;

#endif
