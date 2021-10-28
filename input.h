#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>

#define NUMBER_OF_COMMANDS 20

bool getInput(char*, size_t, FILE*);
void output(char[]);
extern char commands[NUMBER_OF_COMMANDS][24];

#endif