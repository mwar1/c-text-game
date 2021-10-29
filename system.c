#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "parser.h"

extern int seed;
int inputPointer;

void createNewSave() {
	char saveName[4];
	char yesOrNo[2];
	char *line;
	size_t len = 24;
	int i, overwriteIndex;
	bool foundName, overwrite = false;
	FILE *fp;

	printf("\nEnter save name (3 chars)\n>>> ");
	fgets(saveName, 4, stdin);
	while ((getchar()) != '\n');

	fp = fopen("saves.txt", "a+");
	while (!foundName) {
		rewind(fp);
		i = 0;
		foundName = true;
		while (getline(&line, &len, fp) != -1) {
			if (!strcmp(line, "\n")) {
				getline(&line, &len, fp);
				i++;
				line[strcspn(line, "\n")] = 0;
				if (!strcmp(line, saveName)) {
					foundName = false;
					printf("\nName already exists, overwrite? (y/n)\n>>> ");
					fgets(yesOrNo, 2, stdin);
					while ((getchar()) != '\n');

					if (!strcmp(yesOrNo, "y")) {
						foundName = overwrite = true;
						overwriteIndex = i;
					} else {
						printf("\nPlease choose a new name (3 chars)\n>>> ");
						fgets(saveName, 4, stdin);
						while ((getchar()) != '\n');
					}
				}
			}
			i++;
		}
	}
	fclose(fp);

	if (!overwrite) {
		fp = fopen("saves.txt", "a+");
		fprintf(fp, "%s", saveName);
		fprintf(fp, "\n%i", (int) seed);
		fprintf(fp, "\nlook around\n\n");
		fclose(fp);
	} else {
		fp = fopen("saves.txt", "r");
		FILE *temp = fopen("temp.txt", "w");
		int index = 0;
		char ch = getc(fp);
		bool deleting = false;

		while (ch != EOF) {
			if (ch == 10) {
				index++;
				if (index != overwriteIndex && !deleting) {
					putc(ch, temp);
				} else if (!deleting) {
					deleting = true;
				} else {
					ch = getc(fp);
					if (ch == 10) {
						deleting = false;
						putc(10, temp);
					}
				}
			} else if (!deleting) {
				putc(ch, temp);
			}
			ch = getc(fp);
		}
		remove("saves.txt");
		rename("temp.txt", "saves.txt");
		fclose(fp);
		fclose(temp);

		fp = fopen("saves.txt", "a+");
		fprintf(fp, "%s", saveName);
		fprintf(fp, "\n%i", (int) seed);
		fprintf(fp, "\nlook around\n\n");
		fclose(fp);
	}
	fp = fopen("saves.txt", "r");
	i = 0;
	while (getline(&line, &len, fp) != -1) {
		line[strcspn(line, "\n")] = 0;
		if (!strcmp(line, saveName)) {
			break;
		} else {
			i++;
		}
	}
	inputPointer = i + 4;
	fclose(fp);
}

void saveInput(char* input) {
	bool written = false;

	FILE *fp = fopen("saves.txt", "r");
	FILE *temp = fopen("temp.txt", "w");
	if (!strcmp(input, "\n")) inputPointer--;
	int index = 0;
	char ch = getc(fp);

	while (ch != EOF) {
		if (ch == 10) {
			index++;
			if (index != inputPointer) {
				putc(ch, temp);
			} else {
				if (strcmp(input, "quit\n")) {
					fprintf(temp, "%s\n", input);
					written = true;
				}
			}
		} else {
			putc(ch, temp);
		}
		ch = getc(fp);
	}
	remove("saves.txt");
	rename("temp.txt", "saves.txt");
	fclose(fp);
	fclose(temp);

	if (written) inputPointer++;
}

bool load() {
	bool success, foundSave = false;

	char saveName[5];
	printf("\nEnter save name :\n>>> ");
	fgets(saveName, 5, stdin);

	char *line;
	size_t len = 24;
	int i = 0;
	FILE *savesP = fopen("saves.txt", "a+");
	while (!foundSave && getline(&line, &len, savesP) != -1) {
		if (!strcmp(line, saveName)) {
			foundSave = true;
			i += 2;
		}
		i++;
	}
	if (foundSave) {
		success = true;
		bool endOfSave = false;

		printf("Loading save....\n");
		sleep(2);
		system("clear");
		printf("\n");

		getline(&line, &len, savesP);
		seed = atoi(line);
		srand(seed);
		while (!endOfSave) {
			getline(&line, &len, savesP);
			if (!strcmp(line, "\n")) {
				endOfSave = true;
			} else {
				printf("\n>>> %s", line);
				parse(line, savesP);
				i++;
				printf("\n");
			}
		}
		inputPointer = i;
		printf("~~~\nGame succesfully loaded.\n~~~\n");
	} else {
		printf("Save name not found.\n");
	}

	fclose(savesP);
	return success;
}