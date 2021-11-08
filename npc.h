#ifndef NPC_H
#define NPC_H

#include <stdbool.h>
#include "location.h"

void generateNPCs();
bool talk();
bool playerAttack();
bool npcAttack();
extern int numNPCs;
extern struct NPC *npcs[];

typedef struct NPC {
	Location *super;
	Location *location;
	char voiceline[80];
	int health;
	int aggression;
	bool alive;
} NPC;

#endif
