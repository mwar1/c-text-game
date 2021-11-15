#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "input.h"
#include "system.h"

bool getInput(char *input, size_t length) {
	printf("\n>>> ");
	if (readFileP == stdin) {
		fgets(input, length, readFileP);
		saveInput(input);
	} else {
		getline(&input, &length, readFileP);
		inputPointer++;
		printf("%s", input);
	}

	return true;
}
