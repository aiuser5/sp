#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100
#define MAX_LEN 50

// Structure to store the Mnemonic Opcode Table (MOT)
typedef struct {
    char mnemonic[MAX_LEN];
    int opcode;
    int operands;
    int length;
} MOT;

// Structure to store the Main Table entries
typedef struct {
    int line;
    char label[MAX_LEN];
    char mnemonic[MAX_LEN];
    char operand[MAX_LEN];
    int location;
} MainTable;

// Structure to store the Symbol Table entries
typedef struct {
    char symbol[MAX_LEN];
    int address;
} Symbol;

// Global arrays to store the MOT, Main Table, and Symbol Table entries
MOT mot[MAX_SIZE];
MainTable maintable[MAX_SIZE];
Symbol sym_table[MAX_SIZE];

// Global counters for the number of entries in each table
int mot_size = 0;
int maintable_size = 0;
int sym_table_size = 0;

// Function to read the Mnemonic Opcode Table (MOT) from the file "MOT.txt"
void put_MOT() {
    FILE *file = fopen("MOT.txt", "r");
    if (!file) {
        fprintf(stderr, "Error: Unable to open MOT.txt\n");
        return;
    }

    // Read each line and populate the MOT array
    while (fscanf(file, "%s %d %d %d", mot[mot_size].mnemonic, &mot[mot_size].opcode, &mot[mot_size].operands, &mot[mot_size].length) != EOF) {
        mot_size++;
    }
    fclose(file);
}

// Function to read the input instructions from the file "input.txt"
void take_input() {
    FILE *file = fopen("input.txt", "r");
    if (!file) {
        fprintf(stderr, "Error: Unable to open input.txt\n");
        return;
    }

    int lineNumber = 1;
    int location_count = 0;
    char line[MAX_LEN];

    // Read each line from the input file
    while (fgets(line, sizeof(line), file)) {
        char temp[3][MAX_LEN] = {""};
        int token_count = sscanf(line, "%s %s %s", temp[0], temp[1], temp[2]);

        // Depending on the number of tokens, populate the Main Table and Symbol Table
        if (token_count == 2) {
            strcpy(maintable[maintable_size].mnemonic, temp[0]);
            strcpy(maintable[maintable_size].operand, temp[1]);
            maintable[maintable_size].line = lineNumber;
            maintable[maintable_size].location = location_count;
            maintable_size++;
            location_count += 2;
        } else if (token_count == 3) {
            strcpy(sym_table[sym_table_size].symbol, temp[0]);
            sym_table[sym_table_size].address = location_count;
            sym_table_size++;

            strcpy(maintable[maintable_size].label, temp[0]);
            strcpy(maintable[maintable_size].mnemonic, temp[1]);
            strcpy(maintable[maintable_size].operand, temp[2]);
            maintable[maintable_size].line = lineNumber;
            maintable[maintable_size].location = location_count;
            maintable_size++;
            location_count += 2;
        } else if (strcmp(temp[0], "STOP") == 0) {
            strcpy(maintable[maintable_size].mnemonic, "STOP");
            maintable[maintable_size].line = lineNumber;
            maintable[maintable_size].location = location_count;
            maintable_size++;
            location_count += 1;
        }
        lineNumber++;
    }
    fclose(file);
}

// Function to print the Symbol Table
void printSymTable() {
    printf("\nSymbol Table:\n");
    printf("Symbol\tAddress\n");
    for (int i = 0; i < sym_table_size; i++) {
        printf("%s\t%d\n", sym_table[i].symbol, sym_table[i].address);
    }
}

// Function to print the Main Table
void printMainTable() {
    printf("\nMain Table:\n");
    printf("Line\tLabel\tMnemonic\tOperand\tLocation\n");
    for (int i = 0; i < maintable_size; i++) {
        printf("%d\t%s\t%s\t\t%s\t%d\n", maintable[i].line, maintable[i].label, maintable[i].mnemonic, maintable[i].operand, maintable[i].location);
    }
}

// Main function to execute the assembler simulation
int main() {
    put_MOT();
    take_input();
    printSymTable();
    printMainTable();
    return 0;
}
