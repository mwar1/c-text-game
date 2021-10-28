#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "input.h"

#define NUMBER_OF_COMMANDS 20

int currentCommand = 0;
char commands[NUMBER_OF_COMMANDS][24];

bool getInput(char *input, size_t length) {
	printf("\n>>> ");
	fgets(input, length, stdin);

	strcpy(commands[currentCommand], input);
	currentCommand++;
	return true;
}
