#include <stdio.h>
#include <stdbool.h>
#include "input.h"

bool getInput(char *input) {
	printf(">>> ");
	scanf("%[^\n]", input);
	getchar();
	return true;
}
