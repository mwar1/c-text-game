#ifndef OBJECT_H
#define OBJECT_H

#include <stdbool.h>

void generateObjects();
bool take();
bool drop();
bool look();
bool eat();
bool takeFrom();
bool interactContainer();
int *getContainersInLoc(bool);
bool put();
bool lookIn();
extern struct Object* getObjs();
extern struct Object *objs[];
extern int numObjs;

typedef struct Object {
	char article[5];
	char id[16];
	char tag[16];
	char description[128];
	int weight;
	int damage;
	int foodPoints;
	struct Location *location;
} Object;

typedef struct Container {
	struct Object *super;
	struct Location *contents;
	char unlockWith[16];
	int capacity;
	bool locked, open;
} Container;

#endif