void generateObjects();
void take(char*);
void drop(char*);
void look(char*);
extern struct Object* getObjs();
extern struct Object *objs[];
extern int numObjs;

typedef struct Object {
	char tag[16];
	char description[128];
	int weight;
	struct Location *location;
} Object;