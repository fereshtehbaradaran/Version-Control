#include "Header.h"

int init() {

	char command[BUFFER_MAX_SIZE];
	char fileName[FILE_NAME_MAX_SIZE];
	char path[BUFFER_MAX_SIZE];
	FILE* outPut;
	FILE* listOfFiles;

	system("if not exist .versionControl mkdir .versionControl");
	system("attrib +h .versionControl");

	outPut = _popen("dir /B", "r");

	int len = findCurrentPath(path);
	sprintf(path + len, "\\.versionControl");

	sprintf(command, "%s\\logs.bin", path);
	FILE* logs = fopen(command, "w+b");
	fclose(logs);

	sprintf(command, "%s\\listOfFiles.bin", path);
	listOfFiles = fopen(command, "w+b");
	fseek(listOfFiles, 0, SEEK_SET);

	while (fgets(fileName, FILE_NAME_MAX_SIZE, outPut)) {

		fileName[strlen(fileName) - 1] = '\0';

		struct fileInfo file;
		strcpy(file.name, fileName);
		file.isSelected = 0;
		file.isChanged = 0;
		fwrite(&file, sizeof(struct fileInfo), 1, listOfFiles);

		sprintf(command, "cd %s && if not exist \"%s\" mkdir \"%s\"", path, fileName, fileName);
		system(command);
		copyFile(fileName);
	}

	fclose(listOfFiles);
	_pclose(outPut);

	sprintf(command, "copy \"C:\\Users\\fereshteh\\programs\\C\\version control\\diff_python\\applyChanges.py\" \"%s\\applyChanges.py\"", path);
	outPut = _popen(command, "r");
	_pclose(outPut);

	sprintf(command, "copy \"C:\\Users\\fereshteh\\programs\\C\\version control\\diff_python\\findChanges.py\" \"%s\\findChanges.py\"", path);
	outPut = _popen(command, "r");
	_pclose(outPut);

	return 0;
}

int commit(char massage[]) {

	char command[BUFFER_MAX_SIZE];
	char path[BUFFER_MAX_SIZE];
	findCurrentPath(path);
	sprintf(command, "%s\\.versionConntrol\\numberOfCommits.bin", path);

	int numberOfCommits = getNumberOfCommits();

	sprintf(command, "%s\\.versionControl\\listOfFiles.bin", path);
	FILE* listOfFiles = fopen(command, "r+b");
	struct fileInfo fileInformation;

	numberOfCommits++;
	while (fread(&fileInformation, sizeof(struct fileInfo), 1, listOfFiles)) {

		if (fileInformation.isSelected) {
			saveChanges(fileInformation.name, numberOfCommits);
		}
		else {
			copyCommit(fileInformation.name, numberOfCommits);
		}

	}

	fclose(listOfFiles);

	increaseNumberOfCommits(&numberOfCommits);


	struct commitInfo log;
	strcpy(log.massage, massage);
	log.numberOfCommit = numberOfCommits;
	log.toShow = 1;

	sprintf(command, "%s\\.versionControl\\logs.bin", path);
	FILE* logs = fopen(command, "r+b");

	fseek(logs, 0, SEEK_END);
	fwrite(&log, sizeof(struct commitInfo), 1, logs);
	fclose(logs);

	return 1;

}

int printLogs() {

	char path[BUFFER_MAX_SIZE];
	findCurrentPath(path);
	strcat(path, "\\.versionControl\\logs.bin");

	struct commitInfo commit;

	FILE* logs = fopen(path, "r+b");

	while (fread(&commit, sizeof(struct commitInfo), 1, logs)) {

		if (commit.toShow) {
			printColored("\nID: ", PURPLE);
			printf("%d\n", commit.numberOfCommit);
			printColored(commit.massage, LIGHT_BLUE);
			printf("\n");
		}

	}

}

int reset(int commitID) {

	char path[BUFFER_MAX_SIZE];
	findCurrentPath(path);
	strcat(path, "\\.versionControl\\listOfFiles.bin");

	struct fileInfo file;

	FILE* listOfFiles = fopen(path, "r+b");
	if (!listOfFiles) return 0;

	fseek(listOfFiles, 0, SEEK_SET);

	while (fread(&file, sizeof(struct fileInfo), 1, listOfFiles)) {

		applyChanges(file.name, file.name, commitID);
	}

	fclose(listOfFiles);



}

int stash(int commitID) {

	char path[BUFFER_MAX_SIZE];
	findCurrentPath(path);
	strcat(path, "\\.versionControl\\listOfFiles.bin");

	struct fileInfo file;

	FILE* listOfFiles = fopen(path, "r+b");
	if (!listOfFiles) return 0;

	fseek(listOfFiles, 0, SEEK_SET);

	while (fread(&file, sizeof(struct fileInfo), 1, listOfFiles)) {

		saveChanges(file.name, 0);
		applyChanges(file.name, file.name, commitID);
	}

	fclose(listOfFiles);

	return 1;
}

/**
 * This function select or unselect a file
 *
 * @param fileName name of the file you want to select
 * @param toSelect(0/1) 0 to unselect and 1 to select
 * @return 1 when it work seccessfully
 */
int mySelect(char fileName[], int toSelect) {

	int isFind = 0;
	char path[BUFFER_MAX_SIZE];
	findCurrentPath(path);
	strcat(path, "\\.versionControl\\listOfFiles.bin");

	struct fileInfo files[MAX_NUMBER_OF_FILES];
	int numberOfFiles = 0;

	FILE* listOfFiles = fopen(path, "r+b");
	if (!listOfFiles) return 0;

	fseek(listOfFiles, 0, SEEK_SET);

	for (int i = 0; fread(&files[i], sizeof(struct fileInfo), 1, listOfFiles); i++) {

		if (!strcmp(files[i].name, fileName)) {
			isFind = 1;
			files[i].isSelected = toSelect;
		}

		numberOfFiles++;
	}

	fclose(listOfFiles);
	listOfFiles = fopen(path, "w+b");

	for (int i = 0; i < numberOfFiles; i++) {
		fwrite(&files[i], sizeof(struct fileInfo), 1, listOfFiles);
	}

	fclose(listOfFiles);

	if (!isFind) return 0;

	return 1;
}

/**
 * This function print selected items.
 *
 * @return 1 when it works successfully
 */
int printChangedItems() {

	int nothingIsSelected = 1;

	char path[BUFFER_MAX_SIZE];
	findCurrentPath(path);
	strcat(path, "\\.versionControl\\listOfFiles.bin");

	struct fileInfo file;

	FILE* listOfFiles = fopen(path, "r+b");
	if (!listOfFiles) return 0;

	fseek(listOfFiles, 0, SEEK_SET);

	while (fread(&file, sizeof(struct fileInfo), 1, listOfFiles)) {

		if (file.isChanged) {
			nothingIsSelected = 0;
			printColored(file.name, RED);
			printf("\n");
		}
	}

	fclose(listOfFiles);

	if (nothingIsSelected) {
		printColored("Nothing is changed...", LIGHT_BLUE);
		printf("\n");
	}

	return 1;
}

/**
 * This function mark or unmark all files
 *
 * @param toMark(0/1) 0 to unmark and 1 to mark
 * @return 1 when it work seccessfully
 */
void MarkAll(int toMark) {

	char fileName[FILE_NAME_MAX_SIZE];
	FILE* outPut;

	outPut = _popen("dir /B", "r");

	while (fgets(fileName, FILE_NAME_MAX_SIZE, outPut)) {
		fileName[strlen(fileName) - 1] = '\0';
		markChangedFiles(fileName, toMark);
	}
	_pclose(outPut);
}

/**
 * This function mark the file that is changed
 *
 * @param fileName name of the file that is changed
 * @param toMark(0/1) 0 to unmark and 1 to mark
 * @return 1 when it work seccessfully
 */
int markChangedFiles(char fileName[], int toMark) {

	int isFind = 0;
	char path[BUFFER_MAX_SIZE];
	findCurrentPath(path);
	strcat(path, "\\.versionControl\\listOfFiles.bin");

	struct fileInfo files[MAX_NUMBER_OF_FILES];
	int numberOfFiles = 0;

	FILE* listOfFiles = fopen(path, "r+b");
	if (!listOfFiles) return 0;

	fseek(listOfFiles, 0, SEEK_SET);

	for (int i = 0; fread(&files[i], sizeof(struct fileInfo), 1, listOfFiles); i++) {

		if (!strcmp(files[i].name, fileName)) {
			isFind = 1;
			files[i].isChanged = toMark;
		}

		numberOfFiles++;
	}

	fclose(listOfFiles);
	listOfFiles = fopen(path, "w+b");

	for (int i = 0; i < numberOfFiles; i++) {
		fwrite(&files[i], sizeof(struct fileInfo), 1, listOfFiles);
	}

	fclose(listOfFiles);

	if (!isFind) return 0;

	return 1;
}

/**
 * This function copy a file of commit
 *
 * @param name is the name of the file that is commited
 * @param commitID is the number of commit
 * @return doesn't have return value
 */
void copyCommit(char name[], int commitID) {

	FILE* outPut;

	char command[BUFFER_MAX_SIZE];
	char path[BUFFER_MAX_SIZE];
	int len = findCurrentPath(path);

	sprintf(command, "copy \"%s\\.versionControl\\%s\\%d\" \"%s\\.versionControl\\%s\\%d\"", path, name, commitID - 1, path, name, commitID);

	outPut = _popen(command, "r");
	_pclose(outPut);

}

/**
 * This function apply changes to the file
 *
 * @param fileName name of the file to change
 * @param commitID is the commit to return to
 * @return doesn't have a return value
 */
void applyChanges(char fileName[], char newFileName[], int commitID) {

	FILE* filesAdress;

	char command[BUFFER_MAX_SIZE];
	char path[BUFFER_MAX_SIZE];
	findCurrentPath(path);

	sprintf(command, "%s\\.versionControl\\address.txt", path);
	filesAdress = fopen(command, "w");

	sprintf(command, "%s\\.versionControl\\%s\\%s", path, fileName, fileName);

	for (int i = 0; command[i] != '\0'; i++) {
		fputc(command[i], filesAdress);
	}
	fputc('\n', filesAdress);

	sprintf(command, "%s\\.versionControl\\%s\\%d", path, fileName, commitID);

	for (int i = 0; command[i] != '\0'; i++) {
		fputc(command[i], filesAdress);
	}
	fputc('\n', filesAdress);

	sprintf(command, "%s\\%s", path, newFileName);

	for (int i = 0; command[i] != '\0'; i++) {
		fputc(command[i], filesAdress);
	}
	fputc('\n', filesAdress);

	fclose(filesAdress);

	sprintf(command, "python \".versionControl\\applyChanges.py\" <.versionControl\\address.txt");
	system(command);

	system("del \".versionControl\\address.txt\"");

}

/**
 * This function find changes and save them in a file
 *
 * @param fileName name of the file to find changes
 * @param commitID is the nth commit
 * @return doesn't have a return value
 */
void saveChanges(char fileName[], int commitID) {

	FILE* filesAdress;

	char command[BUFFER_MAX_SIZE];
	char path[BUFFER_MAX_SIZE];
	findCurrentPath(path);

	sprintf(command, "%s\\.versionControl\\address.txt", path);
	filesAdress = fopen(command, "w");

	sprintf(command, "%s\\.versionControl\\%s\\%s", path, fileName, fileName);

	for (int i = 0; command[i] != '\0'; i++) {
		fputc(command[i], filesAdress);
	}
	fputc('\n', filesAdress);

	sprintf(command, "%s\\%s", path, fileName);

	for (int i = 0; command[i] != '\0'; i++) {
		fputc(command[i], filesAdress);
	}
	fputc('\n', filesAdress);

	sprintf(command, "%s\\.versionControl\\%s\\%d", path, fileName, commitID);

	for (int i = 0; command[i] != '\0'; i++) {
		fputc(command[i], filesAdress);
	}
	fputc('\n', filesAdress);

	fclose(filesAdress);

	sprintf(command, "python \".versionControl\\findChanges.py\" <\".versionControl\\address.txt\"");
	system(command);

	system("del \".versionControl\\address.txt\"");
}

/**
 * This function print selected items.
 *
 * @return 1 when it works successfully
 */
int printSelectedItem() {

	int nothingIsSelected = 1;

	char path[BUFFER_MAX_SIZE];
	findCurrentPath(path);
	strcat(path, "\\.versionControl\\listOfFiles.bin");

	struct fileInfo file;

	FILE* listOfFiles = fopen(path, "r+b");
	if (!listOfFiles) return 0;

	fseek(listOfFiles, 0, SEEK_SET);

	while (fread(&file, sizeof(struct fileInfo), 1, listOfFiles)) {

		if (file.isSelected) {
			nothingIsSelected = 0;
			printColored(file.name, GREEN);
			printf("\n");
		}
	}

	if (nothingIsSelected) {
		printColored("Nothing is selected...", RED);
		printf("\n");
	}

	return 1;
}

/**
 * This function select or unselect all files
 *
 * @param toSelect(0/1) 0 to unselect all and 1 to select all
 * @return doesn't have return value
 */
void doToAll(int toSelect) {

	char fileName[FILE_NAME_MAX_SIZE];
	FILE* outPut;

	outPut = _popen("dir /B", "r");

	while (fgets(fileName, FILE_NAME_MAX_SIZE, outPut)) {
		fileName[strlen(fileName) - 1] = '\0';
		mySelect(fileName, toSelect);
	}
	_pclose(outPut);
}

/**
 * This function find the current path
 *
 * @param path an empty arry
 * @return len of path
 */
int findCurrentPath(char path[]) {

	FILE* outPut = _popen("cd", "r");

	int c;
	int i = 0;

	for (i = 0; (c = fgetc(outPut)) != EOF; i++) {
		path[i] = c;
	}
	path[--i] = '\0';

	return i;
}

/**
 * This function copy a file.
 *
 * @param name is the name of the file to copy
 * @return doesn't have any return value
 */
void copyFile(const char name[]) {

	char command[BUFFER_MAX_SIZE];
	char path[BUFFER_MAX_SIZE];
	FILE* outPut;
	findCurrentPath(path);

	sprintf(command, "copy \"%s\\%s\" \"%s\\.versionControl\\%s\"", path, name, path, name);
	outPut = _popen(command, "r");
	_pclose(outPut);
}

void increaseNumberOfCommits(int* n) {

	FILE* commits = fopen(".versionControl\\numberOfCommits.txt", "w");
	fwrite(n, sizeof(int), 1, commits);
	fclose(commits);

}

int getNumberOfCommits() {

	int n = 0;
	FILE* commits = fopen(".versionControl\\numberOfCommits.txt", "r");
	if (!commits) {
		commits = fopen(".versionControl\\numberOfCommits.txt", "w");
		fwrite(&n, sizeof(int), 1, commits);
		fclose(commits);
		return n;
	}
	fread(&n, sizeof(int), 1, commits);
	fclose(commits);
	return n;

}

int checkChanges() {

	int numberOfCommits = getNumberOfCommits();
	char command[BUFFER_MAX_SIZE];
	char path[BUFFER_MAX_SIZE];
	findCurrentPath(path);
	strcat(path, "\\.versionControl\\listOfFiles.bin");

	struct fileInfo file;
	int numberOfFiles = 0;

	FILE* listOfFiles = fopen(path, "r+b");
	if (!listOfFiles) return 0;

	fseek(listOfFiles, 0, SEEK_SET);

	for (int i = 0; fread(&file, sizeof(struct fileInfo), 1, listOfFiles); i++) {

		char name[FILE_NAME_MAX_SIZE];
		sprintf(name, ".versionControl\\%s\\lastVersion%s", file.name, file.name);
		applyChanges(file.name, name, numberOfCommits);
		sprintf(command, "fc \"%s\" \"%s\" ", file.name, name);
		int n = system(command);

		if (n != 0) {
			markChangedFiles(file.name, 1);
		}

	}

	fclose(listOfFiles);
	return 1;
}


void changeConsoleColor(int colorCode) {
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, colorCode);
}


void printColored(char* content, int colorCode) {
	changeConsoleColor(WHITE);
	changeConsoleColor(colorCode);
	printf("%s", content);
	changeConsoleColor(WHITE);
}
