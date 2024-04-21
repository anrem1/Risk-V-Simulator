#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <unordered_map>
using namespace std;

int pc; 
map<string, int> memory;
unordered_map<string, string> registers;

void initialize_regs() {
    cout << "Registers:" << endl;
    for (int i = 0; i <= 31; ++i) {
        string reg_name = "x" + to_string(i);
       registers[reg_name] = "";
    }
}

void print_state() {
    cout << "Program Counter (PC): " << pc << endl;
    cout << "Memory:" << endl;
    for (const auto& mem : memory) {
        cout << mem.first << ": " << mem.second << endl;
    }
    cout << "Registers:" << endl;
    for (const auto& reg : registers) {
        cout << reg.first << ": " << reg.second << endl;
    }
    cout << endl;
}


int main() {
    ifstream file("C:\\Users\\merna\\OneDrive\\Documents\\Year 2\\Sem 2\\Assembly\\prog1.txt");
    
    if (!file.is_open()) {
        cout << "Error opening file" << endl;
    }
     string mem_addr;
    if (getline(file, mem_addr)) {
        cout << "Memory Address: " << mem_addr << endl;
    } else {
        cout << "Empty file" << endl;
    }
    initialize_regs();
    memory[mem_addr] = 0;
    string line;
    while (getline(file, line)) {
     //   cout << line << endl;
        size_t pos = line.find(' ');
        string operation = line.substr(0, pos);
        cout << "Operation: " << operation << endl;
         if(operation == "add") {
        } else if(operation == "sub") {
        } else if(operation == "sll") {
        } else if(operation == "slt") {
        } else if(operation == "sltu") {
        } else if(operation == "xor") {
        } else if(operation == "addi") {
        } else if(operation == "slti") {
        } else if(operation == "sltiu") {
        } else if(operation == "xori") {
        } else if(operation == "ori") {
        } else if(operation == "andi") {
        } else if(operation == "slli") {
        }
        print_state();
    }
    file.close();
    return 0;
}

