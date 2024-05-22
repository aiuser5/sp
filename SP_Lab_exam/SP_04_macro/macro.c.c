#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINES 100
#define MAX_LENGTH 100

// Structure to represent a line in MDT
typedef struct {
    int index;
    char card[MAX_LENGTH];
} MDTLine;

// Structure to represent a line in MNT
typedef struct {
    int index;
    char name[MAX_LENGTH];
    int mdtIndex;
} MNTLine;

int main() {
    // variables
    char macroProcesses[MAX_LINES][MAX_LENGTH];
    char code[MAX_LINES][MAX_LENGTH];
    MDTLine mdt[MAX_LINES];
    MNTLine mnt[MAX_LINES];

    // Seed for random number generation
    srand(time(NULL));

    // Generate a random number for the first MDT index
    int initialMdtIndex = rand() % MAX_LINES + 1;  // Ensure it is within a valid range

    int mdtIndex = initialMdtIndex;
    int mntIndex = 1;

    // Input file name
    char fileName[MAX_LENGTH];
    printf("Enter the input file name: ");
    scanf("%s", fileName);

    // File handling
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Read Macro Processes from the file
    printf("\nMacro Processes:\n");
    int i = 0;
    while (fgets(macroProcesses[i], MAX_LENGTH, file) != NULL && strcmp(macroProcesses[i], ".code\n") != 0) {
        printf("%s", macroProcesses[i]);
        i++;
    }

    // Read Code from the file
    printf("\nCode:\n");
    int j = 0;
    while (fgets(code[j], MAX_LENGTH, file) != NULL && strcmp(code[j], "end\n") != 0) {
        printf("%s", code[j]);
        j++;
    }

    fclose(file);

    // Process Macro Processes and generate MNT and MDT
    for (int k = 0; k < i; k++) {
        if (strncmp(macroProcesses[k], "macro", 5) == 0) {
            // Process Macro Name
            sscanf(macroProcesses[k], "macro %s", mnt[mntIndex].name);
            mnt[mntIndex].index = mntIndex;
            mnt[mntIndex].mdtIndex = mdtIndex;

            // Process Macro Definitions
            k++;
            while (strncmp(macroProcesses[k], "mend", 4) != 0) {
                mdt[mdtIndex].index = mdtIndex;
                strcpy(mdt[mdtIndex].card, macroProcesses[k]);
                mdtIndex++;
                k++;
            }

            mdt[mdtIndex].index = mdtIndex;
            strcpy(mdt[mdtIndex].card, "mend\n"); // Include Mend in MDT
            mdtIndex++;

            mntIndex++;
        }
    }

    // Output MNT Table
    printf("\nMNT Table:\n");
    printf("index   | Name      | MDT index\n");
    for (int k = 1; k < mntIndex; k++) {
        printf("%d  | %s        | %d\n", mnt[k].index, mnt[k].name, mnt[k].mdtIndex);
    }

    // Output MDT Table
    printf("\nMDT Table:\n");
    printf("index   | card\n");
    for (int k = initialMdtIndex; k < mdtIndex; k++) {
        printf("%d  | %s", mdt[k].index, mdt[k].card);
    }
    
    return 0;
}
