#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "input.h"
#include "parser.h"
#include "location.h"
#include "object.h"
#include "npc.h"
#include "dictionary.h"
#include "system.h"

char *verb, *verbSyn, *noun, *nounSyn;
char *params[MAX_PARAMETERS];
char *otherWords[MAX_WORDS];

bool health() {
	printf("Health : %i\n", player->health);
	return true;
}

bool quit() {
	saveInput("\n");
	return false;
}

bool help() {
	printf("Type in any command to navigate and interact with the world, and hope that the parser knows how to handle it.\n");
	printf("Most commands should be understood, provided they are not too lengthy.\n");
	printf("e.g. <verb> <noun> <preposition> <noun> e.g. 'put coin in box'.\n");
	printf("\nType 'commands' for a list of possible commands.\n");
	return true;
}

bool listCommands(void) {
	char *allCommands =
	"  *   look / look around             - provides a description of your current surroundings\n"
	"  *   look <item>                    - gives a description of <item>\n"
	"  *   look in <container>            - lists the contents of the specified container\n"
	"  *   go <direction>                 - moves in the direction specified (direction should be north, south, east or west)\n"
	"  *   take <item>                    - pick up <item>\n"
	"  *   drop <item>                    - drops <item>\n"
	"  *   inventory                      - list all objects currently in your inventory\n"
	"  *   talk to <NPC>                  - start a conversation with <NPC>\n"
	"  *   attack <NPC> (with <weapon>)   - starts a fight with <NPC>, using <weapon> if specified\n"
	"  *   eat <item>                     - eat the specified <item>, causing you to gain (or lose) HP and effects\n"
	"  *   open <direction> door          - opens a door in the specified direction, provided it is unlocked first\n"
	"  *   close <direction> door         - closes a door\n"
	"  *   unlock/lock <direction> door   - unlocks or locks the door, provided you have the correct key\n"
	"  *   open/close <container>         - opens or closes the specified container\n"
	"  *   unlock/lock <container>        - unlocks or locks the door, provided you have the correct key\n"
	"  *   put <item> in <container>      - puts the item into the specified container\n"
	"  *   take <item> from <container>   - takes the item out of the container\n"
	"  *   health                         - displays your current health points\n"
	"  *   quit                           - quits the game\n";

	printf("Possible commands:\n\n");
	printf("size of commands = %li bytes.\n", strlen(allCommands));
	puts(allCommands);
	return true;
}

bool noMatch() {
	if (params[0][0] > 0) {
		printf("I don't know how to '%s'.\n", params[0]);
	}
	return true;
}

void matchTerminal(char *input, char *pattern) {
	params[pattern[0] - 'A'] = input;
}

bool matchWord(char *input, char *pattern) {
	if (!strcmp(input, pattern)) {
		for (int i=0; i<MAX_WORDS; i++) {
			if (strlen(otherWords[i]) == 0) {
				otherWords[i] = input;
				break;
			}
		}
		return true;
	}
	return false;
}

char splitInput[8][16], splitPattern[8][16];
bool matchPattern(char *input, char *pattern) {
	// Empty the list of paramaters
	for (int i=0; i<MAX_PARAMETERS; i++) {
		params[i] = "";
	}
	// Empty the list of verbs
	for (int i=0; i<MAX_WORDS; i++) {
		otherWords[i] = "";
	}
	// Empty the lists of split inputs
	for (int i=0; i<8; i++) {
		strcpy(splitInput[i], "");
		strcpy(splitPattern[i], "");
	}

	char compPattern[16], compInput[24], *inpWord, *patWord;
	int inpLength, patLength;
	inpLength = patLength = 0;

	strcpy(compInput, input);
	inpWord = strtok(compInput, " ");
	while (inpWord != NULL) {
		strcpy(splitInput[inpLength], inpWord);

		inpWord = strtok(NULL, " ");
		inpLength++;
	}

	if (!strcmp(pattern, "A")) {
		params[0] = input;
		return true;
	}

	strcpy(compPattern, pattern);
	patWord = strtok(compPattern, " ");
	while (patWord != NULL) {
		strcpy(splitPattern[patLength], patWord);

		patWord = strtok(NULL, " ");
		patLength++;
	}

	if (inpLength != patLength) return false;

	bool matching = true;
	for (int i=0; i<inpLength && matching; i++) {
		if (isupper(splitPattern[i][0])) {
			matchTerminal(splitInput[i], splitPattern[i]);
		} else {
			matching = matchWord(splitInput[i], splitPattern[i]);
		}
	}
	return matching;
}

bool parse (char *input) {
	COMMAND commands[] = {
		{"quit", quit},
		{"help", help},
		{"commands", listCommands},
		{"take A", take},
		{"take A from B", take},
		{"take A out of B", take},
		{"pick up A", take},
		{"pick up A from B", take},	
		{"drop A", drop},
		{"inventory", inventory},
		{"talk to A", talk},
		{"talk A", talk},
		{"fight A", playerAttack},
		{"fight A with B", playerAttack},
		{"eat A", eat},
		{"put A in B", put},
		{"put A B", put},
		{"put A in", put},
		{"put A", put},
		{"put", put},
		{"open A door", interactDoor},
		{"close A door", interactDoor},
		{"lock A door", interactDoor},
		{"unlock A door", interactDoor},
		{"open B", interactContainer},
		{"close B", interactContainer},
		{"lock B", interactContainer},
		{"unlock B", interactContainer},
		{"health", health},
		{"look in A", lookIn},
		{"look at A", look},
		{"look A", look},
		{"look", lookAround},
		{"go to A", go},
		{"go A", go},
		{"A", noMatch}
	};

	// Get synonyms for all input words
	char *word, synInput[24];
	int i, total = 0;
	word = strtok(input, " \n");
	while (word != NULL) {
		getSynonyms(&word);

		for (int j=0; j<strlen(word); j++) {
			synInput[total] = word[j];
			total++;
		}
		synInput[total] = ' ';
		total++;

		word = strtok(NULL, " \n");
		i++;
	}
	synInput[total-1] = '\0';

	// Parse the command
	COMMAND *cp;
	for (cp = commands; !matchPattern(synInput, cp->pattern); cp++);
	return (*cp->function)();
}