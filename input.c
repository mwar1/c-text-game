#include <stdio.h>
#include <stdbool.h>

bool getInput(char* input) {
	printf(">>> ");
	fgets(input, 100, stdin);
	return true;
}
