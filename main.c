#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include "parser.h"

char input[] = "look around";

int main() {
	while (parse(input) && getInput(input));
	printf("Thank you for playing.\n");
}
