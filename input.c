#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "input.h"
#include "system.h"

bool getInput(char *input, size_t length) {
	printf("\n>>> ");
	fgets(input, length, readFileP);
	if (readFileP != stdin) printf("%s", input);

	saveInput(input);
	return true;
}
