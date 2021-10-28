#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

char *line;

void save(char commands[20][24]) {
	char saveName[4];
	char yesOrNo[2];
	size_t len = 32;
	int i, overwriteIndex;
	bool foundName, overwrite = false;
	FILE *fp;

	printf("\nEnter your name (3 chars)\n>>> ");
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
		fprintf(fp, "\n%s\n", saveName);
		for (int i=0; i<20; i++) {
			fprintf(fp, "%s", commands[i]);
		}
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

		fp = fopen("saves.txt", "a");
		fprintf(fp, "\n%s\n", saveName);
		for (int i=0; i<20; i++) {
			fprintf(fp, "%s", commands[i]);
		}
		fclose(temp);
	}
}

void load() {
	;
}