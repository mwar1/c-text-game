struct Location;
void generateLocations();
void look(char*);
void go(char*);
void inventory();
extern struct Location *locs[];
extern struct Location player;
extern int numLocs;
extern int playerLocation;

typedef struct Location {
	char tag[16];
	char description[128];
	int connections[4];
	char directions[4];
} Location;