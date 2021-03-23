#ifndef NPC_H
#define NPC_H

#include <stdbool.h>
#include "location.h"

void generateNPCs();
void talk(char*);
void playerAttack(char*);
bool npcAttack();
extern struct NPC *npcs[];
extern int numNPCs;

typedef struct NPC {
	Location *super;
	Location *location;
	char voiceline[80];
	int health;
	int aggression;
	bool alive;
} NPC;

#endif
