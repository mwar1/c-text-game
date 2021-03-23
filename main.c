#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include "location.h"
#include "object.h"
#include "npc.h"
#include "parser.h"

char input[32] = "look around";

int main() {
	generateLocations();
	generateNPCs();
	createPlayer();
	generateObjects();
	while (parse(input) && getInput(input));
	printf("Thank you for playing.\n");
}
