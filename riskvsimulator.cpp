#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <unordered_map>
#include <sstream>
using namespace std;
class Program
{
public:
	Program(string file, string data);
private:
	void getoperation();
	void sPC(unsigned int);
	unsigned int gPC();
	void doR(string, int);
	int haveR(string); 
	unsigned int PC;
	map<unsigned int, int> Memory; // we have to make a map for memeory that will be printed (int, int) ( we assumed that deal with registers as int, ok?)
	int Registers[32]; // u said map<string, string> registers, will, i think it is better to make integers so it would be easier to deal with them
	string file;
	string data;
	vector<string> rawoperations; // to be the input of any function and then we can retrive whats needed
	vector<vector<string>> alloperations; // we may need this to save the file data in it
	vector<string> Utype = { "LUI", "AUIPC"};
	vector<string> OffsetItype = { "JALR", "LB", "LH", "LW", "LBU", "LHU"};
	vector<string> Itype = { "ADDI", "SLTI", "SLTIU", "XORI", "ORI", "ANDI", "SLLI", "SRLI", "SRAI"};
    vector<string> RegistersN = { "ZERO", "RA", "SP", "GP", "TP", "T0", "T1", "T2", "S0", "S1", "A0", "A1", "A2", "A3", "A4", "A5", "A6", "A7", "S2", "S3", "S4", "S5", "S6", "S7", "S8", "S9", "S10", "S11", "T3", "T4", "T5", "T6" };
	vector<string> Stype = { "SW", "SB", "SH" };
	vector<string> Rtype = { "ADD", "SUB", "SLL", "SLT", "SLTU", "XOR", "SRL", "SRA", "OR", "AND", "MUL", "MULH", "MULHU", "MULHSU","DIV","DIVU","REM","REMU"};
	vector<string> SBtype = { "BEQ", "BNE", "BLT", "BGE", "BLTU", "BGEU"};
	vector<unsigned char> getBytes(int);
	int getInt(vector<unsigned char>);
	void LUI(vector<string> operation);
	void AUIPC(vector<string> operation);
	void JAL(vector<string> operation);
	void JALR(vector<string> operation);
	void BEQ(vector<string> operation);
	void BNE(vector<string> operation);
	void BLT(vector<string> operation);
	void BGE(vector<string> operation);
	void BLTU(vector<string> operation);
	void BGEU(vector<string> operation);
	void LB(vector<string> operation);
	void LH(vector<string> operation);
	void SB(vector<string> operation);
	void SH(vector<string> operation);
	void SW(vector<string> operation);
	void SLTI(vector<string> operation);
	void SLTIU(vector<string> operation);
	void ADDI(vector<string> operation);
	void ORI(vector<string> operation);
	void XORI(vector<string> operation);
	void ANDI(vector<string> operation);
	void SLLI(vector<string> operation);
	void SRLI(vector<string> operation);
	void SRAI(vector<string> operation);
	void ADD(vector<string> operation);
	void SUB(vector<string> operation);
	void SLL(vector<string> operation);
	void XOR(vector<string> operation);
	void SRL(vector<string> operation);
	void SRA(vector<string> operation);
	void OR(vector<string> operation);
	void AND(vector<string> operation);
	void LW(vector<string> operation);
	void SLT(vector<string> operation);
	void SLTU(vector<string> operation);
	void LHU(vector<string> operation);
	void LBU(vector<string> operation);
	void MUL(vector<string> operation);
	void MULH(vector<string> operation);
	void MULHU(vector<string> operation);
	void MULHSU(vector<string> operation);
	void DIV(vector<string> operation);
	void DIVU(vector<string> operation);
	void REM(vector<string> operation);
	void REMU(vector<string> operation);
};

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
    
    if (!file.is_open()) {// this should be a read function in the porgram class, that handles erorrs and handle cases when the operationrucuitons written in capitals, or there is additional spaces or commas 
        cout << "Error opening file" << endl; //  also the functoin should be put in the program class default constructor
    }// also it should give error and break when we find the four or three operationrucitons (i dont remember) that are mentioned in the pdf file
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
        // we  can initalize t I THINK U SHOULD CLEAR EVERYTHING (THE MEMORY AND THE REGISTERS) BEFORE STARTING THE PROGRAM, We can do this in the default constructor
        registers["x2"] = "2";			// examples
        registers["x4"] = "4";
        registers["x5"] = "5";
        registers["x7"] = "7";
        registers["x8"] = "8";
        registers["x10"] = "10";
        registers["x12"] = "12";
        registers["x13"] = "1";
        registers["x15"] = "8";
        registers["x16"] = "6";
        registers["x18"] = "18";
        registers["x19"] = "0";
        registers["x21"] = "2";
        registers["x23"] = "3";	
        registers["x25"] = "9";    
        registers["x27"] = "4";
        registers["x29"] = "3";
        registers["x31"] = "4";
        //we will do a function for each one so the code is more organized
        if (operation.at(0) == "add") // at(0) because it is a map and we have to accsess the first string in it)
	{
		add(operation);
	} else if .. // and so on
    ////////////////////////////////////////
        if(operation == "add") {
            store_values(iss, rd, rs1, rs2);
            registers[rd] = to_string(stoi(registers[rs1]) + stoi(registers[rs2]));
            // what about when using the new program class created we do ,
            //void program::add(vector<string> operation) { doR(operation.at(1), haveR(operation.at(2)) + haveR(operation.at(3)));}
}
         else if(operation == "sub") {
            store_values(iss, rd, rs1, rs2);
            registers[rd] = to_string(stoi(registers[rs1]) - stoi(registers[rs2]));
        } else if(operation == "sll") {
            store_values(iss, rd, rs1, rs2);
            registers[rd] =  to_string(stoi(registers[rs1]) << stoi(registers[rs2]));
        } else if(operation == "slli") {
            store_values(iss, rd, rs1, rs2);
            registers[rd] =  to_string(stoi(registers[rs1]) << stoi(rs2));
        } else if(operation == "slt") {
            store_values(iss, rd, rs1, rs2);
            if(stoi(registers[rs1]) < stoi(registers[rs2]))
                registers[rd] = "1";
            else
                registers[rd] = "0";
        } else if(operation == "sltu") {
            store_values(iss, rd, rs1, rs2);
            if(stoul(registers[rs1]) < stoul(registers[rs2])) {
                registers[rd] = "1";
            } else {
                registers[rd] = "0";
            }
        } else if(operation == "xor") {
            store_values(iss, rd, rs1, rs2);
            registers[rd] = to_string(stoul(registers[rs1]) ^ stoul(registers[rs2]));
        } else if(operation == "addi") {
            store_values(iss, rd, rs1, rs2);
            registers[rd] = to_string(stoi(registers[rs1]) + stoi(rs2));
        } else if(operation == "slti") {
            store_values(iss, rd, rs1, rs2);
            if(stoi(registers[rs1]) < stoi(rs2)) {
                registers[rd] = "1";
            } else {
                registers[rd] = "0";
            }
        } else if(operation == "sltiu") {
            store_values(iss, rd, rs1, rs2);
            if(stoul(registers[rs1]) < stoul(rs2)) {
                registers[rd] = "1";
            } else {
                registers[rd] = "0";
            }
        } else if(operation == "xori") {
            store_values(iss, rd, rs1, rs2);
            registers[rd] = to_string(stoul(registers[rs1]) ^ stoul(rs2));
        } else if(operation == "ori") {
            store_values(iss, rd, rs1, rs2);
            registers[rd] = to_string(stoul(registers[rs1]) | stoul(rs2));
        } else if(operation == "andi") {
            store_values(iss, rd, rs1, rs2);
            registers[rd] = to_string(stoul(registers[rs1]) & stoul(rs2));
        }
        print_state();
    }
    file.close();
    return 0;
}
