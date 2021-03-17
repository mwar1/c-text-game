#include <stdio.h>
#include <stdbool.h>
#include "input.h"

bool getInput(char *input) {
	printf("\n>>> ");
	fgets(input, 48, stdin);
	return true;
}
