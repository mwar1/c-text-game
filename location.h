#ifndef LOCATION_H
#define LOCATION_H

#include <stdbool.h>

void generateLocations();
void createPlayer();
void lookAround();
void go(char*);
void openCloseDoor(char*, char*);
void inventory();
extern struct Location *locs[];
extern struct NPC *player;
extern int numLocs;
extern int playerLocation;

typedef struct Location {
	char article[5];
	char tag[16];
	char intro[128];
	char description[128];
	int connections[4];
	char directions[4];
	int capacity;
	bool locked;
	bool isDoor;
	bool open;
} Location;

#endif
