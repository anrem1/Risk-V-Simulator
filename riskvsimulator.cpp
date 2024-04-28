#include <algorithm>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <iostream>
#include <bits/stdc++.h>
using namespace std;
class program
{
public:
    void run();
	program(string , string, unsigned int );
private:
map<string, string> RegisterHash = {
    {"zero", "x0"}, {"ra", "x1"}, {"sp", "x2"}, {"gp", "x3"}, {"tp", "x4"},
    {"t0", "x5"}, {"t1", "x6"}, {"t2", "x7"}, {"s0", "x8"}, {"s1", "x9"},
    {"a0", "x10"}, {"a1", "x11"}, {"a2", "x12"}, {"a3", "x13"}, {"a4", "x14"},
    {"a5", "x15"}, {"a6", "x16"}, {"a7", "x17"}, {"s2", "x18"}, {"s3", "x19"},
    {"s4", "x20"}, {"s5", "x21"}, {"s6", "x22"}, {"s7", "x23"}, {"s8", "x24"},
    {"s9", "x25"}, {"s10", "x26"}, {"s11", "x27"}, {"t3", "x28"}, {"t4", "x29"},
    {"t5", "x30"}, {"t6", "x31"}
};
	vector<string> RegistersN = { "zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2", "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6" };
	void getoperation();
    string decimalToBinary(int Decimal);
    string decimalToHexa(int n); 
	void dooperation();
	void dooo(vector<string> );
    bool populate();
	int HaveR(string);
	void errorr(string); 
	// string decimalToHexa(int outt);
	// string decimalToBinary(int outt);
	unsigned int PC;
	unsigned int startingPC;
	map<unsigned int, int> Memory;
    void setPC(unsigned int);
	unsigned int getPC();
	void DoR(string, int);
	int Reg[32];
	vector<string> rawoperations;
	vector<vector<string>> alloperations;
	map<string, unsigned int> labels;
	string file;
	string data;
	bool checkregister(string);
	//int bintodec(string binary);
    //	bool errorfinder(); and put it in the dooperation
    bool isValidImmediate(const string& , int& );
    void AND(vector<string> );
    void ANDI(vector<string> );
    void ADD(vector<string> );
    void ADDI(vector<string> );
    void AUIPC(vector<string> );
    void BEQ(vector<string> );
    void BGE(vector<string> );
    void BGEU(vector<string> );
    void BLT(vector<string> );
    void BLTU(vector<string> );
    void BNE(vector<string> );
    void JAL(vector<string> );
    void JALR(vector<string> );
    void LB(vector<string> );
    void LBU(vector<string> );
    void LH(vector<string> );
    void LHU(vector<string> );
    void LUI(vector<string> );
    void LW(vector<string> );
    void OR(vector<string> );
    void ORI(vector<string> );
    void SB(vector<string> );
    void SH(vector<string> );
    void SLL(vector<string> );
    void SLLI(vector<string> );
    void SLT(vector<string> );
    void SLTI(vector<string> );
    //bouns
    void MUL(vector<string> );
	void MULH(vector<string> );
	void MULHU(vector<string> );
	void MULHSU(vector<string> );
	void DIV(vector<string> );
	void DIVU(vector<string> );
	void REM(vector<string> );
	void REMU(vector<string> );
    //
    void SLTU(vector<string> );
    void SLTIU(vector<string> );
    void SRA(vector<string> );
    void SRAI(vector<string> );
    void SRL(vector<string> );
    void SRLI(vector<string> );
    void SUB(vector<string> );
    void SW(vector<string> );
    void XOR(vector<string> );
    void XORI(vector<string> );
};
// Constructor for program class
program::program(string filee, string dataa, unsigned int sPC) {
    file = filee; // Assigning file name
    data = dataa; // Assigning data
    Memory.clear(); // Clearing Memory
    rawoperations.clear(); // Clearing raw operations
    alloperations.clear(); // Clearing all operations
    // Initializing register values to 0
    for (int i = 0; i < 32; i++) {
        Reg[i] = 0;
    }
    PC = sPC; // Setting program counter
    startingPC = sPC; // Setting starting program counter
    getoperation(); // Calling getoperation function
}

// Function to populate operations
bool program::populate() {
    int length = rawoperations.size(); // Getting size of raw operations
    if (rawoperations.empty()) { // Checking if raw operations is empty
        errorr("Empty file"); // Outputting error message
        return false; //  false
    }
    for (int i = 0; i < length; i++) { // Looping through raw operations
        string j = rawoperations.at(i); // Getting current operation
        if (j.find(':') != string::npos) { // Checking if operation contains a label
            j.erase(remove_if(j.begin(), j.end(), static_cast<int(*)(int)>(isspace)), j.end()); // Removing white spaces
            j.erase(j.end() - 1, j.end()); // Removing last character
            transform(j.begin(), j.end(), j.begin(), ::tolower); // Converting to lowercase
            labels.insert(pair<string, unsigned int>(j, getPC() + (alloperations.size()*4))); // Inserting label into map
        } else { // If operation doesn't contain a label
            replace_if(j.begin(), j.end(), [](char c) { return c == '(' || c == ')' || c == ',' || c == ';'; }, ' ');
            transform(j.begin(), j.end(), j.begin(), ::tolower); // Converting to lowercase
            istringstream inst(j); // Creating string stream
            string token; // Token variable
            vector<string> operation; // Vector to store operation tokens
            while (inst >> token) { // Tokenizing operation string
                operation.push_back(token); // Pushing token into vector
            }
            if (operation.empty()) { // Checking if operation vector is empty
                continue; // Continue to next iteration
            }
            alloperations.push_back(operation); // Pushing operation into alloperations vector
        }
    }
    return true; //  true
}

// Function to read operations from instruction and data files
void program::getoperation() {
    ifstream input_file; // Input file stream
    string data_line; // String to store each line of data
    input_file.open(file); //  instruction file

    // Check if instruction file opened successfully
    if (!input_file.is_open()) {
        errorr("Failed to open instruction file"); // Outputting error message
        return; // 
    }

    // Reading each line from instruction file and adding it to rawoperations vector
    while (getline(input_file, data_line)) {
        rawoperations.push_back(data_line); // Adding each line to rawoperations vector
    }
    input_file.close(); // Closing instruction file

    // Populating operations vector
    if (!populate()) {
        return; //  if population fails
    }

    input_file.open(data); //  data file

    // Check if data file opened successfully
    if (!input_file.is_open()) {
        errorr("Failed to open data file"); // Outputting error message
        return; // 
    }

    // Reading each line from data file and inserting Memory location and data value into Memory map
    while (getline(input_file, data_line)) {
        if (data_line.empty()) { // Skipping empty lines
            continue;
        }
        int separator = data_line.find(','); // Finding separator (',')
        unsigned int Memory_location; // Variable to store Memory location
        int data_value; // Variable to store data value

        // getting Memory location and data value from line
        Memory_location = stoi(data_line.substr(0, separator));
        if (Memory_location % 4 != 0) {
            errorr("Memory should be divisble by 4");
        }
        data_value = stoi(data_line.substr(separator + 1, data_line.length()));

        // Inserting Memory location and data value into Memory map
        Memory.insert(pair<unsigned int, int>(Memory_location, data_value));
    }
    input_file.close(); // Closing data file

    dooperation(); // Performing operations
}

// Function to execute operations
void program::dooperation() {
    int operationsize = alloperations.size(); // Getting size of operations vector
    int INDEX = (PC - startingPC) / 4; // Calculating index based on current program counter
    run(); 

    // Looping through operations vector
    while (INDEX < operationsize) {
        vector<string> operation = alloperations.at(INDEX); // Getting current operation

        // Checking for control operations and breaking if encountered
        if (operation.at(0) == "ebreak" || operation.at(0) == "ecall" || operation.at(0) == "fence") {
            break;
        }

        dooo(operation); // Executing current operation
        run(); // Running program
        PC = PC + 4; // Incrementing program counter by 4
        INDEX = (PC - startingPC) / 4; // Updating index based on new program counter
    }
}

void program::run()
{
    cout << "Register Values:" << endl;
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << "| Register Name       | Register Number | Decimal Value | Binary Value      | Hexadecimal Value |" << endl;
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    for (int i = 0; i < 32; i++)
    {
        cout << "| " << setw(20) << left << RegistersN[i] << " | " << setw(15) << left << ("x" + to_string(i)) << " | " << setw(13) << left << Reg[i] << " | " << setw(17) << left << decimalToBinary(Reg[i]) << " | " << setw(16) << left << decimalToHexa(Reg[i]) << " |" << endl;
    }
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;

    cout << "Memory Values:" << endl;
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << "| Memory Address      | Decimal Value | Binary Value      | Hexadecimal Value |" << endl;
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    for (auto i : Memory)
        cout << "| " << setw(20) << left << i.first << " | " << setw(13) << left << i.second << " | " << setw(17) << left << decimalToBinary(i.second) << " | " << setw(16) << left << decimalToHexa(i.second) << " |" << endl;

    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << "Program Counter:" << endl << PC << endl;
}


// Function to set the program counter
void program::setPC(unsigned int st) {
    // Checking if the new program counter value is within valid range
    if (st < startingPC) { 
        errorr("Wrong address"); // Outputting error message
        return; //
    } else {
        PC = st; // Setting program counter to the new value
    }
}

// Function to get the program counter
unsigned int program::getPC() {
    return PC; // program counter value
}

// Function to perform register operations
void program::DoR(string registerr, int valuee) {
    // Checking if the register format is valid
    if (!checkregister(registerr)) {
        errorr("Wrong Register format"); // Outputting error message
        return; //
    }
    // Checking if the register is x0 (zero register)
    if (registerr == "x0") {
        errorr("Cannot change the value of Register ZERO"); // Outputting error message
        return; //
    }
    int registernm;
    // getting register number
    if (registerr[0] == 'x') {
        registernm = stoi(registerr.substr(1));
    } else {
        auto it = RegisterHash.find(registerr);
        if (it != RegisterHash.end()) {
            registernm = stoi(it->second.substr(1));
        }
    }
    Reg[registernm] = valuee; // Setting register value
    return;
}

// Function to handle errors
void program::errorr(string errorrrr) {
    cout << errorrrr << endl; // Outputting error message
    exit(1); // Exiting program with error status
}

// Function to get the value of a register
int program::HaveR(const string stringg) {
    // Checking if the register format is valid
    if (!checkregister(stringg)) {
        errorr("Wrong Register format"); // Outputting error message
        return -1; // invalid value
    }
    int registernm;
    // getting register number
    if (stringg[0] == 'x') {
        registernm = stoi(stringg.substr(1));
    } else { // searching the map
        auto it = RegisterHash.find(stringg);
        if (it != RegisterHash.end()) {
            registernm = stoi(it->second.substr(1));
        }
    }
    return Reg[registernm]; // register value
}

// Function to check if a string represents a valid register
bool program::checkregister(string str)
{
    // Check if the string is found as a key in RegisterHash
    if (RegisterHash.find(str) != RegisterHash.end()) {
        return true;
    }
    
    // Check if the string is found as a value in RegisterHash
    for (const auto& pair : RegisterHash) {
        if (pair.second == str) {
            return true;
        }
    }

    // String not found in keys or values
    return false;
}

void program::dooo(vector<string> operationn) {
    string oper = operationn.at(0);
    
    if (oper == "add") {
        ADD(operationn);
    } else if (oper == "addi") {
        ADDI(operationn);
    } else if (oper == "and") {
        AND(operationn);
    } else if (oper == "andi") {
        ANDI(operationn);
    } else if (oper == "auipc") {
        AUIPC(operationn);
    } else if (oper == "beq") {
        BEQ(operationn);
    } else if (oper == "bge") {
        BGE(operationn);
    } else if (oper == "bgeu") {
        BGEU(operationn);
    } else if (oper == "blt") {
        BLT(operationn);
    } else if (oper == "bltu") {
        BLTU(operationn);
    } else if (oper == "bne") {
        BNE(operationn);
    } else if (oper == "jal") {
        JAL(operationn);
    } else if (oper == "jalr") {
        JALR(operationn);
    } else if (oper == "lb") {
        LB(operationn);
    } else if (oper == "lbu") {
        LBU(operationn);
    } else if (oper == "lh") {
        LH(operationn);
    } else if (oper == "lhu") {
        LHU(operationn);
    } else if (oper == "lui") {
        LUI(operationn);
    } else if (oper == "lw") {
        LW(operationn);
    } else if (oper == "or") {
        OR(operationn);
    } else if (oper == "ori") {
        ORI(operationn);
    } else if (oper == "sb") {
        SB(operationn);
    } else if (oper == "sh") {
        SH(operationn);
    } else if (oper == "sll") {
        SLL(operationn);
    } else if (oper == "slli") {
        SLLI(operationn);
    } else if (oper == "slt") {
        SLT(operationn);
    } else if (oper == "slti") {
        SLTI(operationn);
    } else if (oper == "mul") {
        MUL(operationn);
    } else if (oper == "mulh") {
        MULH(operationn);
    } else if (oper == "mulhu") {
        MULHU(operationn);
    } else if (oper == "mulhsu") {
        MULHSU(operationn);
    } else if (oper == "div") {
        DIV(operationn);
    } else if (oper == "divu") {
        DIVU(operationn);
    } else if (oper == "rem") {
        REM(operationn);
    } else if (oper == "remu") {
        REMU(operationn);
    } else if (oper == "sltu") {
        SLTU(operationn);
    } else if (oper == "sltiu") {
        SLTIU(operationn);
    } else if (oper == "sra") {
        SRA(operationn);
    } else if (oper == "srai") {
        SRAI(operationn);
    } else if (oper == "srl") {
        SRL(operationn);
    } else if (oper == "srli") {
        SRLI(operationn);
    } else if (oper == "sub") {
        SUB(operationn);
    } else if (oper == "sw") {
        SW(operationn);
    } else if (oper == "xor") {
        XOR(operationn);
    } else if (oper == "xori") {
        XORI(operationn);
    } else {
        // Handle unknown operation
        cout << "Unknown operation: " << oper << endl;
    }
}

// Function to check if a string represents a valid immediate value
bool program::isValidImmediate(const string& immediateStr, int& immediate) {
    try {
        immediate = stoi(immediateStr); // Converting string to integer
        return true; // Returning true if conversion succeeds
    } catch (const invalid_argument&) {
        return false; // Returning false if conversion fails due to invalid argument
    } catch (const out_of_range&) {
        return false; // Returning false if conversion fails due to out of range argument
    }
}
// Function to perform Load Upper Immediate (LUI) operation
void program::LUI(vector<string> operation) {
    int immediate;
    // Checking if the immediate value is valid
    if (!isValidImmediate(operation.at(2), immediate)) {
        errorr("Wrong immediate");
        return;
    }
    // Shifting immediate left by 12 bits and setting the register value
    DoR(operation.at(1), immediate << 12);
}

// Function to perform Add Upper Immediate to PC (AUIPC) operation
void program::AUIPC(vector<string> operation) {
    int immediate;
    // Checking if the immediate value is valid
    if (!isValidImmediate(operation.at(2), immediate)) {
        errorr("Wrong immediate");
        return;
    }
    // Shifting immediate left by 12 bits, adding PC and setting the register value
    DoR(operation.at(1), (immediate << 12) + PC);
}

// Function to perform Jump and Link Register (JALR) operation
void program::JALR(vector<string> operation) {
    int immediate;
    // Checking if the immediate value is valid
    if (!isValidImmediate(operation.at(3), immediate)) {
        errorr("Wrong immediate");
        return;
    }
    // Calculating new program counter value, setting register value, and updating program counter
    DoR(operation.at(1), immediate + HaveR(operation.at(2)) + 4);
    setPC(immediate + HaveR(operation.at(2)) - 4);
}

// Function to perform Jump and Link (JAL) operation
void program::JAL(vector<string> operation) {
    // Setting register value to current program counter + 4
    DoR(operation.at(1), getPC() + 4);
    // Checking if label exists in function map and setting program counter accordingly
    if (labels.find(operation.at(2)) == labels.end()) {
        errorr("the label is not found");
    } else {
        setPC((labels.find(operation.at(2))->second) - 4);
    }
}

// Function to perform Branch Equal (BEQ) operation
void program::BEQ(vector<string> operation) {
    // Checking if label exists in function map and setting program counter accordingly
    if (labels.find(operation.at(3)) == labels.end()) {
        errorr("the label is not found");
        return;
    }
    if (HaveR(operation.at(1)) == HaveR(operation.at(2))) {
        setPC((labels.find(operation.at(3))->second) - 4);
    }
}

// Function to perform Branch Not Equal (BNE) operation
void program::BNE(vector<string> operation) {
    // Checking if label exists in function map and setting program counter accordingly
    if (labels.find(operation.at(3)) == labels.end()) {
        errorr("the label is not found");
        return;
    }
    if (HaveR(operation.at(1)) != HaveR(operation.at(2))) {
        setPC((labels.find(operation.at(3))->second) - 4);
    }
}

// Function to perform Branch Less Than (BLT) operation
void program::BLT(vector<string> operation) {
    // Checking if label exists in function map and setting program counter accordingly
    if (labels.find(operation.at(3)) == labels.end()) {
        errorr("the label is not found");
        return;
    }
    if (HaveR(operation.at(1)) < HaveR(operation.at(2))) {
        setPC((labels.find(operation.at(3))->second) - 4);
    }
}

// Function to perform Branch Greater Than or Equal (BGE) operation
void program::BGE(vector<string> operation) {
    // Checking if label exists in function map and setting program counter accordingly
    if (labels.find(operation.at(3)) == labels.end()) {
        errorr("the label is not found");
        return;
    }
    if (HaveR(operation.at(1)) >= HaveR(operation.at(2))) {
        setPC((labels.find(operation.at(3))->second) - 4);
    }
}

// Function to perform Set Less Than Immediate (SLTI) operation
void program::SLTI(vector<string> operation) {
    int immediate;
    // Checking if the immediate value is valid
    if (operation.size() < 4 || !isValidImmediate(operation.at(3), immediate)) {
        errorr("Wrong immediate");
        return;
    }
    // Setting register value based on comparison with immediate
    if (HaveR(operation.at(2)) < immediate) {
        DoR(operation.at(1), 1);
    } else {
        DoR(operation.at(1), 0);
    }
}

// Function to perform Set Less Than Immediate Unsigned (SLTIU) operation
void program::SLTIU(vector<string> operation) {
    int immediate;
    // Checking if the immediate value is valid
    if (operation.size() < 4 || !isValidImmediate(operation.at(3), immediate)) {
        errorr("Wrong immediate");
        return;
    }
    // Setting register value based on comparison with immediate (unsigned)
    if ((int)HaveR(operation.at(2)) < immediate) {
        DoR(operation.at(1), 1);
    } else {
        DoR(operation.at(1), 0);
    }
}

// Function to perform Add Immediate (ADDI) operation
void program::ADDI(vector<string> operation) {
    int immediate;
    // Checking if the immediate value is valid
    if (operation.size() < 4 || !isValidImmediate(operation.at(3), immediate)) {
        errorr("Wrong immediate");
        return;
    }
    // Adding immediate value to register value and setting the result
    DoR(operation.at(1), immediate + HaveR(operation.at(2)));
}

// Function to perform Bitwise OR Immediate (ORI) operation
void program::ORI(vector<string> operation) {
    int immediate;
    // Checking if the immediate value is valid
    if (operation.size() < 4 || !isValidImmediate(operation.at(3), immediate)) {
        errorr("Wrong immediate");
        return;
    }
    // Performing bitwise OR operation with immediate value and setting the result
    DoR(operation.at(1), immediate | HaveR(operation.at(2)));
}

// Function to perform Bitwise XOR Immediate (XORI) operation
void program::XORI(vector<string> operation) {
    int immediate;
    // Checking if the immediate value is valid
    if (operation.size() < 4 || !isValidImmediate(operation.at(3), immediate)) {
        errorr("Wrong immediate");
        return;
    }
    // Performing bitwise XOR operation with immediate value and setting the result
    DoR(operation.at(1), immediate ^ HaveR(operation.at(2)));
}

// Function to perform Bitwise AND Immediate (ANDI) operation
void program::ANDI(vector<string> operation) {
    int immediate;
    // Checking if the immediate value is valid
    if (operation.size() < 4 || !isValidImmediate(operation.at(3), immediate)) {
        errorr("Wrong immediate");
        return;
    }
    // Performing bitwise AND operation with immediate value and setting the result
    DoR(operation.at(1), immediate & HaveR(operation.at(2)));
}

// Function to perform Shift Left Logical Immediate (SLLI) operation
void program::SLLI(vector<string> operation) {
    int immediate;
    // Checking if the immediate value is valid
    if (operation.size() < 4 || !isValidImmediate(operation.at(3), immediate)) {
        errorr("Wrong immediate");
        return;
    }
    // Performing shift left logical operation with immediate value and setting the result
    DoR(operation.at(1), HaveR(operation.at(2)) << immediate);
}

// Function to perform Shift Right Logical Immediate (SRLI) operation
void program::SRLI(vector<string> operation) {
    int immediate;
    // Checking if the immediate value is valid
    if (operation.size() < 4 || !isValidImmediate(operation.at(3), immediate)) {
        errorr("Wrong immediate");
        return;
    }
    // Performing shift right logical operation with immediate value and setting the result
    DoR(operation.at(1), (int)(HaveR(operation.at(2))) >> immediate);
}

// Function to perform Shift Right Arithmetic Immediate (SRAI) operation
void program::SRAI(vector<string> operation) {
    int immediate;
    // Checking if the immediate value is valid
    if (operation.size() < 4 || !isValidImmediate(operation.at(3), immediate)) {
        errorr("Wrong immediate");
        return;
    }
    // Performing shift right arithmetic operation with immediate value and setting the result
    DoR(operation.at(1), HaveR(operation.at(2)) >> immediate);
}

// Function to perform Addition operation
void program::ADD(vector<string> operation) {
    // Adding values of two registers and setting the result to the destination register
    DoR(operation.at(1), HaveR(operation.at(2)) + HaveR(operation.at(3)));
}

// Function to perform Subtraction operation
void program::SUB(vector<string> operation) {
    // Subtracting values of two registers and setting the result to the destination register
    DoR(operation.at(1), HaveR(operation.at(2)) - HaveR(operation.at(3)));
}

// Function to perform Shift Left Logical (SLL) operation
void program::SLL(vector<string> operation) {
    // Shifting value of a register left by value of another register and setting the result to the destination register
    DoR(operation.at(1), HaveR(operation.at(2)) << HaveR(operation.at(3)));
}

// Function to perform Set Less Than (SLT) operation
void program::SLT(vector<string> operation) {
    // Comparing values of two registers and setting 1 to the destination register if first is less than second, otherwise setting 0
    if (HaveR(operation.at(2)) < HaveR(operation.at(3))) {
        DoR(operation.at(1), 1);
    } else {
        DoR(operation.at(1), 0);
    }
}

// Function to perform Set Less Than Unsigned (SLTU) operation
void program::SLTU(vector<string> operation) {
    // Comparing values of two registers (unsigned) and setting 1 to the destination register if first is less than second, otherwise setting 0
    if ((int)HaveR(operation.at(2)) < (int)HaveR(operation.at(3))) {
        DoR(operation.at(1), 1);
    } else {
        DoR(operation.at(1), 0);
    }
}

// Function to perform Multiplication (MUL) operation
void program::MUL(vector<string> operation) {
    // Multiplying values of two registers and setting the result to the destination register
    int32_t finall = static_cast<int32_t>((int64_t(HaveR(operation.at(2)) * int64_t(HaveR(operation.at(3))))));
    DoR(operation.at(1), finall);
}

// Function to perform Multiplication High (MULH) operation
void program::MULH(vector<string> operation) {
    // Multiplying values of two registers and getting the high bits of the result, then setting it to the destination register
    int32_t finall = static_cast<int32_t>((int64_t(HaveR(operation.at(2)) * int64_t(HaveR(operation.at(3))))) >> 32);
    DoR(operation.at(1), finall);
}

// Function to perform Multiplication High Unsigned (MULHU) operation
void program::MULHU(vector<string> operation) {
    // Multiplying values of two registers (unsigned) and getting the high bits of the result, then setting it to the destination register
    uint32_t finall = static_cast<uint32_t>((uint64_t(HaveR(operation.at(2)) * uint64_t(HaveR(operation.at(3))))) >> 32);
    DoR(operation.at(1), finall);
}

// Function to perform Multiplication High Signed/Unsigned (MULHSU) operation
void program::MULHSU(vector<string> operation) {
    // Multiplying values of two registers and getting the high bits of the result (with signed/unsigned interpretation), then setting it to the destination register
    int32_t finall = static_cast<int32_t>((int64_t(HaveR(operation.at(2)) * uint64_t(HaveR(operation.at(3))))) >> 32);
    DoR(operation.at(1), finall);
}

// Function to perform Division (DIV) operation
void program::DIV(vector<string> operation) {
    // Dividing values of two registers and setting the quotient to the destination register
    int finall = HaveR(operation.at(2)) / HaveR(operation.at(3));
    DoR(operation.at(1), finall);
}

// Function to perform Division Unsigned (DIVU) operation
void program::DIVU(vector<string> operation) {
    // Dividing values of two registers (unsigned) and setting the quotient to the destination register
    int finall = (int)HaveR(operation.at(2)) / (int)HaveR(operation.at(3));
    DoR(operation.at(1), finall);
}

// Function to perform Remainder (REM) operation
void program::REM(vector<string> operation) {
    // Computing remainder of division of values of two registers and setting it to the destination register
    int finall = HaveR(operation.at(2)) % HaveR(operation.at(3));
    DoR(operation.at(1), finall);
}

// Function to perform Remainder Unsigned (REMU) operation
void program::REMU(vector<string> operation) {
    // Computing remainder of division of values of two registers (unsigned) and setting it to the destination register
    int finall = (int)HaveR(operation.at(2)) % (int)HaveR(operation.at(3));
    DoR(operation.at(1), finall);
}

// Function to perform Bitwise XOR  operation
void program::XOR(vector<string> operation) {
    // Performing bitwise XOR operation on values of two registers and setting the result to the destination register
    DoR(operation.at(1), HaveR(operation.at(2)) ^ HaveR(operation.at(3)));
}

// Function to perform Shift Right Logical (SRL) operation
void program::SRL(vector<string> operation) {
    // Performing shift right logical operation on value of a register by value of another register and setting the result to the destination register
    DoR(operation.at(1), (int)HaveR(operation.at(2)) >> HaveR(operation.at(3)));
}

// Function to perform Shift Right Arithmetic (SRA) operation
void program::SRA(vector<string> operation) {
    // Performing shift right arithmetic operation on value of a register by value of another register and setting the result to the destination register
    DoR(operation.at(1), HaveR(operation.at(2)) >> HaveR(operation.at(3)));
}

// Function to perform Bitwise OR operation
void program::OR(vector<string> operation) {
    // Performing bitwise OR operation on values of two registers and setting the result to the destination register
    DoR(operation.at(1), HaveR(operation.at(2)) | HaveR(operation.at(3)));
}

// Function to perform Bitwise AND operation
void program::AND(vector<string> operation) {
    // Performing bitwise AND operation on values of two registers and setting the result to the destination register
    DoR(operation.at(1), HaveR(operation.at(2)) & HaveR(operation.at(3)));
}


void program::BGEU(vector<string> operation) {
    if (labels.find(operation.at(3)) == labels.end()) {
        errorr("Function doesn't exist");
        return;
    }
    if ((unsigned int)HaveR(operation.at(1)) >= (unsigned int)HaveR(operation.at(2))) {
        setPC((labels.find(operation.at(3))->second) - 4);
    }
}

void program::BLTU(vector<string> operation) {
    if (labels.find(operation.at(3)) == labels.end()) {
        errorr("Function doesn't exist");
        return;
    }
    if ((unsigned int)HaveR(operation.at(1)) < (unsigned int)HaveR(operation.at(2))) {
        setPC((labels.find(operation.at(3))->second) - 4);
        }
}

void program::LB(vector<string> operation) {
    int immediate;
    immediate = stoi(operation.at(2));
    if (!isValidImmediate(operation.at(2), immediate)) {
        errorr("Wrong immediate");
        return;
    }
    int baseValue = HaveR(operation.at(3));
    if (baseValue == -1) return; // Error handling in HaveR

    int address = baseValue + immediate;
    char value = static_cast<char>(Memory[address]); // Assuming Memory is a byte-addressable array
    DoR(operation.at(1), static_cast<int>(value)); // Sign-extend the loaded byte
}

void program::LH(vector<string> operation) {
    int offset;
    offset = stoi(operation.at(2));
    unsigned int totaloffset = offset + HaveR(operation.at(3));

    // Check if the address is aligned on a 4-byte boundary
    if (totaloffset % 4 == 3) {
        unsigned int firstpart = 0;
        unsigned int secpart = 0;

        // Retrieve the upper 16 bits from the lower Byte
        if (Memory.find(totaloffset - (totaloffset % 4)) == Memory.end()) {
            firstpart = 0;
        } else {
            int Byte = Memory.find(totaloffset - (totaloffset % 4))->second;
            firstpart = (Byte >> 24) & 0xFF;
        }

        // Retrieve the lower 16 bits from the higher Byte
        if (Memory.find(totaloffset + 1) == Memory.end()) {
            secpart = 0;
        } else {
            int Byte = Memory.find(totaloffset + 1)->second;
            secpart = (Byte >> 0) & 0xFF;
        }

        unsigned int number = firstpart + (secpart << 8);

        // Check if the number is negative (signed)
        if (number >> 15 == 1) {
            number += 0xFFFF0000;
        }

        DoR(operation.at(1), number);
    } else {
        if (Memory.find(totaloffset - (totaloffset % 4)) == Memory.end()) {
            DoR(operation.at(1), 0);
        } else {
            int Byte = Memory.find(totaloffset - (totaloffset % 4))->second;
            unsigned int firstpart = (Byte >> 8 * (totaloffset % 4)) & 0xFF;
            unsigned int secpart = (Byte >> (8 * ((totaloffset % 4) + 1))) & 0xFF;
            unsigned int number = firstpart + (secpart << 8);

            // Check if the number is negative (signed)
            if (number >> 15 == 1) {
                number += 0xFFFF0000;
            }

            DoR(operation.at(1), number);
        }
    }
}


void program::LHU(vector<string> operation) {
    int offset;
    offset = stoi(operation.at(2));

    unsigned int add = offset + HaveR(operation.at(3));

    // Check if the address is aligned on a 4-byte boundary
    if (add % 4 == 3) {
        unsigned int firstpart = 0;
        unsigned int secpart = 0;

        // Retrieve the upper 16 bits from the lower byte
        if (Memory.find(add - (add % 4)) == Memory.end()) {
            firstpart = 0;
        } else {
            int pyte = Memory.find(add - (add % 4))->second;
            firstpart = (pyte >> 24) & 0xFF;
        }

        // Retrieve the lower 16 bits from the higher pyte
        if (Memory.find(add + 1) == Memory.end()) {
            secpart = 0;
        } else {
            int pyte = Memory.find(add + 1)->second;
            secpart = (pyte >> 0) & 0xFF;
        }

        unsigned int number = firstpart + (secpart << 8);

        DoR(operation.at(1), number);
    } else {
        // Error: Address is not aligned on a 4-byte boundary
        errorr("Innumberid LHU operation: Address is not aligned on a 4-byte boundary");
    }
}

void program::LW(vector<string> operation) {
    int offset;
    offset = stoi(operation.at(2));
    int address = HaveR(operation.at(3)) + offset;
    if (!isValidImmediate(operation.at(2), offset)||(offset % 4 != 0)) {
            errorr("Wrong offset"); 
            return;}
    if (Memory.find(address) == Memory.end()) {
            DoR(operation.at(1), 0);
        } else {
            int number = Memory.find(address)->second;
            DoR(operation.at(1), number);
        }
}

void program::SB(vector<string> operation) {
    int offset;
    if (operation.size() < 3 || !isValidImmediate(operation.at(2), offset)) {
        errorr("Wrong immediate");
        return;
    }
    int address = HaveR(operation.at(3)) + offset;

    int value = HaveR(operation.at(1));
    Memory[address] = value & 0xFF;
}
void program::SH(vector<string> operation) {
    int offset;
    if (operation.size() < 3 || !isValidImmediate(operation.at(2), offset) || offset % 2 != 0) {
        errorr("Wrong immediate");
        return;
    }
    int address = HaveR(operation.at(3)) + offset;

    int value = HaveR(operation.at(1));
    Memory[address] = (value >> 8) & 0xFF;
    Memory[address + 1] = value & 0xFF;
}
void program::LBU(vector<string> operation) {

		int offset = stoi(operation.at(2));
		unsigned int totaloffset = offset + HaveR(operation.at(3));
         if (!isValidImmediate(operation.at(2), offset)) {
            errorr("Wrong offset");
        }
		if (Memory.find(totaloffset - (totaloffset % 4)) == Memory.end()) {
			DoR(operation.at(1), 0);
		}
		else {
			int smth = Memory.find(totaloffset - (totaloffset % 4))->second;
			unsigned int pyte = (smth >> (8 * (totaloffset % 4))) & 0xFF;
			DoR(operation.at(1), pyte);
		}
	}
	
void program:: SW(vector<string> operation) {
        int offset = stoi(operation.at(2));
        if (operation.size() < 3 || !isValidImmediate(operation.at(2), offset) ||offset%4 != 0 ) {
            errorr("Wrong immediate");
            return;
        }
        int address = HaveR(operation.at(3)) + offset;

        int value = HaveR(operation.at(1));
        Memory[address] = value & 0xFF;
    }
string program::decimalToHexa(int num) {
string binary = decimalToBinary(num);
    string hex = "";
    int value = 0;
    if (num == 0) return "0";


    for (int i = 0; i < 32; i += 4) {
        value = 8 * (binary[i] - '0') + 4 * (binary[i+1] - '0') + 
                2 * (binary[i+2] - '0') + (binary[i+3] - '0');
        if (value < 10) 
            hex += char('0' + value);
        else 
            hex += char('A' + (value - 10));
    }
 return hex;
}
string program::decimalToBinary(int num){
    if (num == 0) return "0";
    if (num == INT_MIN) return "10000000000000000000000000000000";

    bool isNegative = num < 0;
    unsigned int posNum = isNegative ? -num : num;
    
    string binary = "";
    while (posNum > 0) {
        binary = to_string(posNum % 2) + binary;
        posNum /= 2;
    }
        binary = string(32 - binary.length(), '0') + binary;
    
    if (isNegative) {
        for (char& c : binary) c = (c == '0' ? '1' : '0');
                bool carry = true;
        for (int i = binary.length() - 1; i >= 0 && carry; i--) {
            if (binary[i] == '1') {
                binary[i] = '0';
            } else {
                binary[i] = '1';
                carry = false;
            }
        }
    }
    return binary;
}

    

int main() {
	string alloperations = "instructions.txt";
	string data = "Data.txt";
    int sPC;
    cout << "Enter the starting PC: "<< endl;
    cin >> sPC;
    while (sPC<0||sPC%4!=0){
    if (sPC<0) cout << "Don't make the starting address a negative number"<< endl;
    if (sPC%4!=0)cout << "Starting address should be divisible by 4"<< endl;
    cout << "Enter the starting PC: "<< endl;
    cin >> sPC;
    }
    program run(alloperations, data, sPC);
	return 0;
}
