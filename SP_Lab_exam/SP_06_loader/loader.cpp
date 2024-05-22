// a.c size 149 bytes 
// b.c size 157 bytes 

// 149 + 157 = 309

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;


void readFileSize(string filename, int& size) {
    ifstream file(filename, ios::binary | ios::ate);
    if (file.is_open()) {
        size = file.tellg();
    }
}

int main() {
    int memorySize;
    cout << "Enter the size of memory: ";
    cin >> memorySize;

    int sizeA, sizeB;
    string contentA, contentB;
    readFileSize("a.c", sizeA);
    readFileSize("b.c", sizeB);

    int totalSize = sizeA + sizeB;
    cout << "Total file size: " << totalSize << " bytes" << endl;

    if (totalSize > memorySize) {
        cout << "Memory Space give is :" << memorySize <<endl;
        cout <<  "Insufficient memory."  << endl;
    }
    else{
        cout << "Memory is being allocated" << endl;
    }
    return 0;
}
