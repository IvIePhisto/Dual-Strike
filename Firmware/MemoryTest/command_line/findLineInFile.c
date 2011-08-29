
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

int findLineInFile(char* path, char* lineToFind) {
	int lineLength = strlen(lineToFind);
	char* line = malloc(lineLength + 1);
	FILE* file = fopen(path, "r");
	int returnValue = 1;
	
	if(file == NULL) {
        fprintf(stderr, "Error opening file \"%s\".\n", path, strerror(errno));
        return 1;
	}
	
	while(1) {
		char* readLine = fgets(line, lineLength + 1, file);
		
		if(readLine == NULL)
			break;
		
		if(strcmp(line, lineToFind) == 0) {
			returnValue = 0;
			break;
		}
	}
	
	fclose(file);
	free(line);
	
	return returnValue;
}

static void printUsage(char *pname) {
    fprintf(stderr,	"usage: %s ([-h|--help] | <file> <line to find>)\n", pname);
}

static int testArgs(int argc, char **argv) {
	int currentIndex;
	
	if(argc == 1
	|| (argc == 2
	    && (strcmp(argv[currentIndex], "-h") == 0
		 || strcmp(argv[currentIndex], "--help") == 0))
	|| argc == 3)
		return 0;

	return 1;
}

int main(int argc, char **argv) {
    if(testArgs(argc, argv)) {
        printUsage(argv[0]);
        return 1;
    }
	
    if(argc > 1 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)) {
        printUsage(argv[0]);
        return 0;
    }

    return findLineInFile(argv[1], argv[2]);
}



