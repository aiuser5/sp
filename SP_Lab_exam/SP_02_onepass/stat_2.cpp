#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

using namespace std;

class MOT {
public:
    string mnemonic;
    int opcode, operands, length;
    MOT(string mnemonic, int opcode, int operands, int length) {
        this->mnemonic = mnemonic;
        this->opcode = opcode;
        this->operands = operands;
        this->length = length;
    }
};

class MainTable {
public:
    int line;
    string label;
    string mnemonic;
    string operand;
    int location;
    string output;
    MainTable(int line, string label, string mnemonic, string operand, int location, string output) {
        this->line = line;
        this->label = label;
        this->mnemonic = mnemonic;
        this->operand = operand;
        this->location = location;
        this->output = output;
    }
};

vector<MOT> mot;
map<string, int> pot;
map<string, int> mot_table;
int lineNumber = 1;
int location_count = 0;
map<string, int> sym_table;
vector<MainTable> maintable;

void put_MOT() {
    ifstream file("MOT.txt");
    string line;
    if (!file.is_open()) {
        cerr << "Error: Unable to open MOT.txt\n";
        return;
    }
    while (getline(file, line)) {
        stringstream ss(line);
        string mnemonic;
        int opcode, operands, length;
        ss >> mnemonic >> opcode >> operands >> length;
        mot_table[mnemonic] = opcode;
        mot.push_back(MOT(mnemonic, opcode, operands, length));
    }
}

void take_input() {
    ifstream file("input.txt");
    string line;
    int ct = 2;
    while (getline(file, line)) {
        istringstream iss(line);
        string token;
        vector<string> temp;
        while (iss >> token) {
            temp.push_back(token);
        }
        if (temp.size() == 2) {
            maintable.push_back(MainTable(lineNumber, "", temp[0], temp[1], location_count, ""));
            location_count += ct;
        } else if (temp.size() == 3) {
            sym_table[temp[0]] = location_count;
            maintable.push_back(MainTable(lineNumber, temp[0], temp[1], temp[2], location_count, ""));
            location_count += ct;
        } else if (temp[0] == "STOP") {
            maintable.push_back(MainTable(lineNumber, "", temp[0], "", location_count, ""));
            ct = 1;
            location_count += ct;
            ct = 2;
        }
        lineNumber++;
    }
}

void printSymTable() {
    cout << "\nSymbol Table:\n";
    cout << "Symbol\tAddress\n";
    for (auto it : sym_table) {
        cout << it.first << "\t" << it.second << endl;
    }
}

void printMainTable() {
    cout << "\nMain Table:\n";
    cout << "Line\tLabel\tMnemonic\tOperand\tLocation\n";
    for (auto it : maintable) {
        cout << it.line << "\t" << it.label << "\t" << it.mnemonic << "\t\t" << it.operand << "\t" << it.location << "\t\t" << endl;
    }
}

int main() {
    put_MOT();
    take_input();
    printSymTable();
    printMainTable();
}