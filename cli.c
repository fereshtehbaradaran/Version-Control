#include "Header.h"

void printDescription(char name[], char description[], char example[]) {

	printColored(name, YELLOW);
	printColored(description, LIGHT_BLUE);
	printColored(example, BLUE);
	printf("\n");
}

void printGuid() {

	printDescription("\ninit\n", "To make this directory under control, use", " \"VC init\" ");
	printDescription("\nstatus\n", "To sea changes and other information, use", " \"VC status\" ");
	printDescription("\nselect\n", "To select a file, use", " \"VC select \"File Name\" \" ");
	printColored("*Use \"-all\" to select all\n", PURPLE);
	printDescription("\nunselect\n", "To unselect a file, use", " \"VC unselect \"File Name\" \" ");
	printColored("*Use \"-all\" to unselect all\n", PURPLE);
	printDescription("\ncommit\n", "To commit changes, use", " \"VC commit \"commit description\" \" ");
	printColored("*Commit Description is required\n", PURPLE);
	printDescription("\nlog\n", "To see list of logs, use", " \"VC log\" ");
	printDescription("\nreset\n", "To reset changes, use", " \"VC reset \"commit ID\" \" ");
	printColored("*If you don't commit before reset, you may loose some changes\n", PURPLE);
	printDescription("\nstash\n", "To reset changes, use", " \"VC stash \"commit ID\" \" ");
	printColored("*You can return to your current position by using \"stash pop\" \n", PURPLE);
}

void shortGuid() {

	printColored("Use this commands:\n", LIGHT_BLUE);
	printColored("init\nstatus\nselect[fileName]\nunselect[fileName]\ncommit[commitDescription]\nlog\nreset[commitId]\nstash[commitId]\n", YELLOW);

}

int run(int argc, char const* argv[]) {

	if (argc == 1) {
		printColored("Nothing to do!", RED);
		printf("\n\n");
		shortGuid();
		printColored("*use \"-help\" for more information\n", PURPLE);
		return 0;
	}
	else if (argc == 2 && !strcmp(argv[1], "-help")) {
		printGuid();
		return 1;
	}
	else if (!strcmp(argv[1], "init")) {

		if (argc == 2) {
			init();
			doToAll(1);
			commit("First version");
			doToAll(0);
			return 1;
		}
		printColored("SOMTHING IS WRONG!", YELLOW);
		printf("\n");
		printColored("Use it like this:\t\" VC init\" ", LIGHT_BLUE);
		printf("\n");
		return 0;
	}
	else if (!strcmp(argv[1], "status")) {

		if (argc == 2) {
			printColored("Selected files:\n", ORANGE);
			printSelectedItem();
			printf("\n");
			changeConsoleColor(GRAY);
			checkChanges();
			changeConsoleColor(WHITE);
			printColored("Changed files:", ORANGE);
			printf("\n");
			printChangedItems();
			printf("\n");
			return 1;
		}
		printColored("SOMTHING IS WRONG!", YELLOW);
		printf("\n");
		printColored("Use it like this:\t\" VC status\" ", LIGHT_BLUE);
		printf("\n");
		return 0;
	}
	else if (!strcmp(argv[1], "select")) {

		if (argc == 3) {
			if (!strcmp(argv[2], "-all")) {
				doToAll(1);
				return 1;
			}
			if (!mySelect(argv[2], 1)) {
				printf("CAN'T FIND THE FILE!\n");
				return 0;
			}
			return 1;
		}
		printColored("SOMTHING IS WRONG!", YELLOW);
		printf("\n");
		printColored("Use it like this:\t\" VC select \"file name\" \"", LIGHT_BLUE);
		printf("\n");
		return 0;
	}

	if (!strcmp(argv[1], "unselect")) {

		if (argc == 3) {
			if (!strcmp(argv[2], "-all")) {
				doToAll(0);
				return 1;
			}
			if (!mySelect(argv[2], 0)) {
				printf("CAN'T FIND THE FILE!\n");
				return 0;
			}
			return 1;
		}
		printColored("SOMTHING IS WRONG!", YELLOW);
		printf("\n");
		printColored("Use it like this:\t\" VC unselect \"file name\" \"", LIGHT_BLUE);
		printf("\n");
		return 0;
	}

	if (!strcmp(argv[1], "commit")) {

		if (argc == 3) {
			commit(argv[2]);
			doToAll(0);
			MarkAll(0);
			return 1;
		}
		if (argc == 2) {
			printColored("Commit Description is required!", YELLOW);
			printf("\n");
			return 0;
		}
		printColored("SOMTHING IS WRONG!", YELLOW);
		printf("\n");
		printColored("Use it like this:\t\" VC commit \"commit description\" \"", LIGHT_BLUE);
		printf("\n");
	}

	if (!strcmp(argv[1], "log")) {

		if (argc == 2) {
			printLogs();
			return 1;
		}
		printColored("SOMTHING IS WRONG!", YELLOW);
		printf("\n");
		printColored("Use it like this:\t\" VC log \"", LIGHT_BLUE);
		printf("\n");
		return 0;

	}

	if (!strcmp(argv[1], "reset")) {

		if (argc == 3) {
			reset(atoi(argv[2]));
			return 0;
		}
		else {
			printColored("SOMTHING IS WRONG!", YELLOW);
			printf("\n");
			printColored("Use it like this:\t\" VC reset \"commit ID\" \"", LIGHT_BLUE);
			printf("\n");
			return 0;
		}
	}

	if (!strcmp(argv[1], "stash")) {

		if (argc == 3) {
			if (!strcmp(argv[2], "pop")) {
				reset(0);
				return 0;
			}
			else {
				stash(atoi(argv[2]));
				return 0;
			}
		}
		else {
			printColored("SOMTHING IS WRONG!", YELLOW);
			printf("\n");
			printColored("Use it like this:\t\" VC stash \"commit ID\" \"", LIGHT_BLUE);
			printf("\n");
			printColored("* You  can use \"stash pop\" each time you stash", LIGHT_BLUE);
			printf("\n");
			return 0;
		}

	}

	printColored("SOMTHING IS WRONG!", RED);
	printf("\n\n");
	shortGuid();
	printColored("*use \"-help\" for more information\n", PURPLE);
	return 0;

}
