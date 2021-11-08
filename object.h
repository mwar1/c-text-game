#ifndef OBJECT_H
#define OBJECT_H

void generateObjects();
bool take();
bool drop();
bool look();
bool eat();
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

#endif