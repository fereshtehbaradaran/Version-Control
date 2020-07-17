#pragma once

#ifndef DO_ONCE

#define DO_ONCE

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<Windows.h>

#define WHITE 15
#define YELLOW 14
#define LIGHT_BLUE 11
#define RED 12
#define GREEN 10
#define PURPLE 13
#define BLUE_BLOCK 18
#define GRAY 8
#define BLUE 3
#define ORANGE 6


#define STRING_MAX_SIZE 100
#define FILE_NAME_MAX_SIZE 100
#define BUFFER_MAX_SIZE 1000
#define MAX_NUMBER_OF_FILES 100

struct fileInfo {

	char name[STRING_MAX_SIZE];
	char isSelected;
	char isChanged;

};

struct commitInfo {
	char massage[BUFFER_MAX_SIZE];
	int numberOfCommit;
	int toShow;
};

int init();
void copyFile(const char name[]);
int findCurrentPath(char path[]);
int mySelect(char fileName[], int toSelect);
void doToAll(int toSelect);
int printSelectedItem();
void saveChanges(char fileName[], int commitID);
void applyChanges(char fileName[], char newFile[], int commitID);
int reset(int commitID);
int stash(int commitID);
int commit(char massage[]);
void copyCommit(char name[], int commitID);
int printLogs();
int checkChanges();
int markChangedFiles(char fileName[], int toMark);
void MarkAll(int toMark);
int printChangedItems();

void changeConsoleColor(int colorCode);
void printColored(char* content, int colorCode);

#endif



