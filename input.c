#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "input.h"

#define NUMBER_OF_COMMANDS 20

int currentCommand = 1;
char commands[NUMBER_OF_COMMANDS][24] = {"look around\n"};

bool getInput(char *input, size_t length, FILE *fp) {
	printf("\n>>> ");
	fgets(input, length, fp);
	if (fp != stdin) printf("%s", input);

	strcpy(commands[currentCommand], input);
	currentCommand++;
	return true;
}
