// ala 10
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES 100
#define MAX_LENGTH 256
#define MAX_ARGS 10

typedef struct {
    char name[MAX_LENGTH]; //macro name
    char args[MAX_ARGS][MAX_LENGTH]; // fomal paramter --> &a,&b
    int argCount;
} MNTEntry;

MNTEntry mnt[MAX_LINES];
int mntCount = 0;
char ala[MAX_ARGS][MAX_LENGTH];

void readDefinitionFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open definition file");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LENGTH];
    while (fgets(line, MAX_LENGTH, file)) {
        if (strstr(line, "Macro")) {
            sscanf(line, "Macro %s", mnt[mntCount].name);
            char *token = strtok(line, " ");
            token = strtok(NULL, " "); // Skip "Macro"
            token = strtok(NULL, ","); // First argument

            mnt[mntCount].argCount = 0;
            while (token != NULL) {
                strcpy(mnt[mntCount].args[mnt[mntCount].argCount++], token);
                token = strtok(NULL, ",\n");
            }
            mntCount++;
        }
    }

    fclose(file);
}

void printALA(int macroIndex) {
    for (int j = 0; j < mnt[macroIndex].argCount; j++) {
        printf("%-20s %-20s %-20d %-20s\n", mnt[macroIndex].name, mnt[macroIndex].args[j], j + 1, ala[j]);
    }
}

void processLine(char *line) {
    char *token = strtok(line, " \n");
    for (int i = 0; i < mntCount; i++) {
        if (strcmp(token, mnt[i].name) == 0) {
            // Parse the arguments
            token = strtok(NULL, ",\n");
            int argIndex = 0;
            while (token != NULL) {
                strcpy(ala[argIndex++], token);
                token = strtok(NULL, ",\n");
            }
            // Print ALA for the current macro invocation
            printALA(i);
            return;
        }
    }
}

void readCodeFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open code file");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LENGTH];
    while (fgets(line, MAX_LENGTH, file)) {
        processLine(line);
    }

    fclose(file);
}

int main() {
    const char *definitionFile = "def.txt";
    const char *codeFile = "code.txt";

    readDefinitionFile(definitionFile);
    printf("Argument List Array (ALA):\n");
    printf("%-20s %-20s %-20s %-20s\n", "Macro Name", "Formal Argument", "Positional Argument", "Actual Argument");
    readCodeFile(codeFile);

    return 0;
}
