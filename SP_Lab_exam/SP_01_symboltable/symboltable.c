#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char symbol[30];
    char type[10];
    int length;
    int address;
} SymbolEntry;

void initializeSymbolTable(SymbolEntry *symbolTable, int size) {
    for (int i = 0; i < size; i++) {
        strcpy(symbolTable[i].symbol, "");
        strcpy(symbolTable[i].type, "");
        symbolTable[i].length = 0;
        symbolTable[i].address = 0;
    }
}

void printSymbolTable(SymbolEntry *symbolTable, int symbolCount) {
    printf("\nSymbol Table:\n");
    printf("Symbol    Type      Length    Address\n");

    for (int i = 0; i < symbolCount; i++) {
        printf("%-10s %-10s %-8d %-8d\n", symbolTable[i].symbol, symbolTable[i].type, symbolTable[i].length, symbolTable[i].address);
    }
}

int main() {
    FILE *inputFile;
    char fileName[100];

    printf("Enter the name of the input file: ");
    scanf("%s", fileName);

    inputFile = fopen(fileName, "r");

    if (inputFile == NULL) {
        fprintf(stderr, "Error: Unable to open file '%s'.\n", fileName);
        return 1;
    }

    SymbolEntry symbolTable[100];
    int symbolCount = 0;

    // Initialize the symbolTable array
    initializeSymbolTable(symbolTable, sizeof(symbolTable) / sizeof(symbolTable[0]));

    char line[256];
    while (fgets(line, sizeof(line), inputFile)) {
        char *type = strtok(line, " ");
        char *symbol = strtok(NULL, " ;");

        if ((strcmp(type, "int") == 0 || strcmp(type, "char") == 0 || strcmp(type, "float") == 0 || strcmp(type, "double") == 0) &&
            strcmp(symbol, "main") != 0) {

            char *token = strtok(symbol, ",");
            while (token != NULL) {
                strcpy(symbolTable[symbolCount].symbol, token);
                strcpy(symbolTable[symbolCount].type, type);

                int arraySize = 0;
                if (strstr(token, "[") != NULL) {
                    sscanf(token, "%*[^[][%d]", &arraySize);
                    if (strcmp(type, "char") == 0)
                        symbolTable[symbolCount].length = arraySize * sizeof(char);
                    else if (strcmp(type, "int") == 0)
                        symbolTable[symbolCount].length = arraySize * sizeof(int);
                    else if (strcmp(type, "float") == 0)
                        symbolTable[symbolCount].length = arraySize * sizeof(float);
                    else if (strcmp(type, "double") == 0)
                        symbolTable[symbolCount].length = arraySize * sizeof(double);
                } else {
                    if (strcmp(type, "char") == 0)
                        symbolTable[symbolCount].length = sizeof(char);
                    else if (strcmp(type, "int") == 0)
                        symbolTable[symbolCount].length = sizeof(int);
                    else if (strcmp(type, "float") == 0)
                        symbolTable[symbolCount].length = sizeof(float);
                    else if (strcmp(type, "double") == 0)
                        symbolTable[symbolCount].length = sizeof(double);
                }

                symbolTable[symbolCount].address = (symbolCount > 0) ? (symbolTable[symbolCount - 1].address + symbolTable[symbolCount - 1].length) : 0;

                symbolCount++;
                token = strtok(NULL, ",");
            }
        }
    }

    fclose(inputFile);

    printSymbolTable(symbolTable, symbolCount);

    return 0;
}
