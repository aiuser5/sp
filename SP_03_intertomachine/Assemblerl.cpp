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

    MOT(string mnemonic, int opcode, int operands , int length) {
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
map<string, int> mot_length;
int lineNumber = 1;
int location_count = 0;
map<string, int> sym_table;
vector<MainTable> maintable;


void printMOT() {
    cout << "\nMOT Table:\n";
    cout << "Mnemonic\tOpcode\tOperands\tLength\n";
    for (auto it : mot) {
        cout << it.mnemonic << "\t\t" << it.opcode << "\t" << it.operands << "\t" << it.length << "\n";
    }
    cout << "\n";
}

void printPOT() {
    cout << "\nPOT Table:\n";
    cout << "Pseudo-Opcode\tNo. Of Operands\n";
    for (auto it : pot) {
        cout << it.first << "\t\t" << it.second << "\n";
    }
    cout << "\n";
}

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
        mot_length[mnemonic] = length;
        mot.push_back(MOT(mnemonic, opcode, operands, length));
    }
    printMOT();
}

void put_POT() {
    ifstream file("POT.txt");
    string line;
    if (!file.is_open()) {
        cerr << "Error: Unable to open POT.txt\n";
        return;
    }
    while (getline(file, line)) {
        stringstream ss(line);
        string opcode;
        int operands;
        ss >> opcode >> operands;
        pot[opcode] = operands;
    }
    printPOT();
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
            int length = mot_length[temp[0]];
            location_count += length;      
        } else if (temp.size() == 3) {
            sym_table[temp[0]] = location_count;
            maintable.push_back(MainTable(lineNumber, temp[0], temp[1], temp[2], location_count, ""));
            int length = mot_length[temp[0]];
            location_count += length;
        } else if (temp[0] == "STOP") {
            maintable.push_back(MainTable(lineNumber, "", temp[0], "", location_count, ""));
            int length = mot_length[temp[0]];
            location_count += length;
        }
        lineNumber++;
    }
}

void secondPass() {
    for (auto &i : maintable) {
        int opcode = mot_table[i.mnemonic];
        int location = sym_table[i.operand];

        i.output = to_string(opcode) + "_" + to_string(location);
    }
}

void printMainTable() {
    cout << "\nMain Table:\n";
    cout << "Line\tLabel\tMnemonic\tOperand\tLocation\tOutput\n";
    for (auto it : maintable) {
        cout << it.line << "\t" << it.label << "\t" << it.mnemonic << "\t\t" << it.operand << "\t" << it.location << "\t\t" << it.output << endl;
    }
    cout << "\n";
}

void printSymTable() {
    cout << "\nSymbol Table:\n";
    cout << "Symbol\tAddress\n";
    for (auto it : sym_table) {
        cout << it.first << "\t" << it.second << endl;
    }
    cout << "\n";
}

int main() {
    put_MOT();
    put_POT();
    take_input();
    printSymTable();
    secondPass();
    printMainTable();
    return 0;
}
