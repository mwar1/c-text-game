#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include "parser.h"

char input[] = "look around";

int main() {
	while (getInput(input) && parse(input));
	printf("Thank you for playing.\n");
}
