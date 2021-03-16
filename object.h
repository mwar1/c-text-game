struct Object;
void generateObjects();
void take(char*);
void drop(char*);
extern struct Object* getObjs();
extern struct Object *objs[];
extern int numObjs;

typedef struct Object {
	char tag[16];
	char description[128];
	struct Location *location;
} Object;