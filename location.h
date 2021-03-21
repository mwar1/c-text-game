#ifndef LOCATION_H
#define LOCATION_H

void generateLocations();
void generatePaths();
void lookAround();
void go(char*);
void inventory();
extern struct Location *locs[];
extern struct Location player;
extern int numLocs;
extern int playerLocation;

typedef struct Location {
	char tag[16];
	char intro[128];
	char description[128];
	int connections[4];
	char directions[4];
	int capacity;
} Location;

#endif