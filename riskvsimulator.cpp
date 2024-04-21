#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <unordered_map>
#include <sstream>
using namespace std;

int pc; 
map<int, string> memory;
map<string, string> registers;

void initialize_regs() {
    for (int i = 0; i <= 31; ++i) {
        string reg_name = "x" + to_string(i);
       registers[reg_name] = "";
    }
    registers["x0"] = "0";
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

void store_values(istringstream& iss, string& rd, string& rs1, string& rs2) {
    iss >> rd >> ws; 
    rd.erase(rd.end() - 1); 
    if (getline(iss, rs1, ',')) {
        iss >> ws; 
    }
    if (getline(iss, rs2)) {
        iss >> ws; 
    }  
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
    memory[stoi(mem_addr)] = "0";
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string operation, rd, rs1, rs2;
        iss >> operation;
        
        registers["x2"] = "12";			// example
        registers["x3"] = "10";
        
        
        if(operation == "add") {
         store_values(iss, rd, rs1, rs2);
         registers[rd] = to_string(stoi(registers[rs1]) + stoi(registers[rs2]));
         pc += 4;
        } else if(operation == "sub") {
        store_values(iss, rd, rs1, rs2);
       	registers[rd] = to_string(stoi(registers[rs1]) - stoi(registers[rs2]));
         pc += 4;
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

