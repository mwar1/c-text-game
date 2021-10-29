#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "input.h"
#include "system.h"

#define NUMBER_OF_COMMANDS 20

bool getInput(char *input, size_t length, FILE *fp) {
	printf("\n>>> ");
	fgets(input, length, fp);
	if (fp != stdin) printf("%s", input);

	saveInput(input);
	return true;
}
