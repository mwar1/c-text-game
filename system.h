#ifndef SYSTEM_H
#define SYSTEM_H

void createNewSave();
void saveInput(char*);
bool load();

extern FILE *readFileP;
extern int inputPointer;

#endif