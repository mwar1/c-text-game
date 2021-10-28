#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "system.h"
#include "input.h"
#include "location.h"
#include "object.h"
#include "npc.h"
#include "parser.h"

char input[24];
char welcomeMessage[] =
"			░██╗░░░░░░░██╗███████╗██╗░░░░░░█████╗░░█████╗░███╗░░░███╗███████╗██╗\n"
"			░██║░░██╗░░██║██╔════╝██║░░░░░██╔══██╗██╔══██╗████╗░████║██╔════╝██║\n"
"			░╚██╗████╗██╔╝█████╗░░██║░░░░░██║░░╚═╝██║░░██║██╔████╔██║█████╗░░██║\n"
"			░░████╔═████║░██╔══╝░░██║░░░░░██║░░██╗██║░░██║██║╚██╔╝██║██╔══╝░░╚═╝\n"
"			░░╚██╔╝░╚██╔╝░███████╗███████╗╚█████╔╝╚█████╔╝██║░╚═╝░██║█████████╗╗\n"
"			░░░╚═╝░░░╚═╝░░╚══════╝╚══════╝░╚════╝░░╚════╝░╚═╝░░░░░╚═╝╚══════╝╚═╝\n\n\n"
"					      ▄▄ ▄▄ █▀█ █░░ ▄▀█ █▄█ ▄▄ ▄▄\n"
"					      ░░ ░░ █▀▀ █▄▄ █▀█ ░█░ ░░ ░░\n\n"
"			  		      ▄▄ ▄▄ █░░ █▀█ ▄▀█ █▀▄ ▄▄ ▄▄\n"
"			  		      ░░ ░░ █▄▄ █▄█ █▀█ █▄▀ ░░ ░░\n\n"
"					      ▄▄ ▄▄ █░█ █▀▀ █░░ █▀█ ▄▄ ▄▄\n"
"					      ░░ ░░ █▀█ ██▄ █▄▄ █▀▀ ░░ ░░\n\n"
"					      ▄▄ ▄▄ █▀█ █░█ █░░ ▀█▀ ▄▄ ▄▄\n"
"					      ░░ ░░ ▀▀█ █▄█ █░░ ░█░ ░░ ░░\n";

void finish() {
	printf("Thank you for playing.\n");
	exit(0);
}

bool getInitalInput() {
	bool ready, newGame = false;
	while (!ready) {
		printf("\n>>> ");
		fgets(input, 32, stdin);
		input[strlen(input)-1] = '\0';

		if (!strcmp(input, "help")) {
			printf("Type in any command to navigate the world, and hope that the parser knows how to handle it.\n");
			printf("At the moment, commands should be in the form '<noun> <verb>', for example 'look book'.\n");
			printf("This will be improved in a further update.\n");
		} else if (!strcmp(input, "load")){
			bool success = load();
			ready = success;
		} else if (!strcmp(input, "play")) {
			printf("Good luck!\n");
			ready = newGame = true;
		} else if (!strcmp(input, "quit")) {
			finish();
		}
	}
	return newGame;
}

int main() {
	system("clear");
	printf("\n\n\n");
	puts(welcomeMessage);

	generateLocations();
	generateNPCs();
	createPlayer();
	generateObjects();

	bool newGame = getInitalInput();

	if (newGame) {
		sleep(2);
		system("clear");
		printf("\n");

		strcpy(input, "look around\n");
	} else {
		getInput(input, 24, stdin);
	}
	while (parse(input, stdin) && getInput(input, 24, stdin));
	finish();
}
