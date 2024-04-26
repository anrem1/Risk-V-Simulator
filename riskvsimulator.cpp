#include <algorithm>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <bits/stdc++.h>
using namespace std;
class program
{
public:
	program(string , string, unsigned int );
	void output();
private:
    vector<string> uformat = { "LUI", "AUIPC"};
	vector<string> OffsetIformat = { "JALR", "LB", "LH", "LW", "LBU", "LHU"};
	vector<string> Iformat = { "ADDI", "SLTI", "SLTIU", "XORI", "ORI", "ANDI", "SLLI", "SRLI", "SRAI"};
	vector<string> sformat = { "SW", "SB", "SH" };
	vector<string> sbformat = { "BEQ", "BNE", "BLT", "BGE", "BLTU", "BGEU"};
	vector<string> rformat = { "ADD", "SUB", "SLL", "SLT", "SLTU", "XOR", "SRL", "SRA", "OR", "AND", "MUL", "MULH", "MULHU", "MULHSU","DIV","DIVU","REM","REMU"};
	vector<string> RegistersN = { "ZERO", "RA", "SP", "GP", "TP", "T0", "T1", "T2", "S0", "S1", "A0", "A1", "A2", "A3", "A4", "A5", "A6", "A7", "S2", "S3", "S4", "S5", "S6", "S7", "S8", "S9", "S10", "S11", "T3", "T4", "T5", "T6" };
	void getoperation();
	void dooperation();
	void dooo(vector<string> instruction);
	void setPC(unsigned int);
	unsigned int getPC();
	void DoR(string, int);
	int HaveR(string);
	void errorr(string); 
	// string DecToHex(int outt);
	// string DecToBin(int outt);
	unsigned int PC;
	unsigned int startingPC;
	map<unsigned int, int> Memory;
	int Reg[32];
	vector<string> rawoperations;
	vector<vector<string>> alloperations;
	map<string, unsigned int> funcs;
	string file;
	string data;
	unsigned int returnindex(); 
    bool checkinteger(string);
	bool checkregister(string);
	//int bintodec(string binary);
    //	bool errorfinder(); and put it in the dooperation
    bool isValidImmediate(const string& immStr, int& imm);
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
program::program(string filee, string dataa, unsigned int startingPC)
{
	file = filee;
	data = dataa;
	Memory.clear();
	rawoperations.clear();
	alloperations.clear();
	getoperation();
	PC = startingPC;
	for (int i = 0; i < 32; i++) {
		Reg[i] = 0;
	}
}

void program::getoperation()
{
    ifstream input_file;
    string data_line;
    input_file.open(file);
    if (!input_file.is_open()) {
        errorr("Failed to open instruction file");
        return;
    }
    while (getline(input_file, data_line)) {
        rawoperations.push_back(data_line);
    }
    input_file.close();
    input_file.open(data);
    if (!input_file.is_open()) {
        errorr("Failed to open data file");
        return;
    }
    while (getline(input_file, data_line))
    {
        if (data_line.empty()) {
            continue;
        }
        int separator  = data_line.find(',');
        unsigned int memory_location;
        int data_value;

        memory_location = stoi(data_line.substr(0, separator ));
        data_value = stoi(data_line.substr(separator  + 1, data_line.length()));

        if (memory_location % 4 != 0 ||  memory_location >= PC && memory_location < PC + (alloperations.size() * 4)) {
            errorr("Error reading the file. ");
            return;
        }
        Memory.insert(pair<unsigned int, int>(memory_location, data_value));
    }
    input_file.close();
    dooperation();
}

void program::dooperation()
{
	int instructionlen = alloperations.size();
	if (funcs.find("start") != funcs.end()) {
		setPC(funcs.find("start")->second);
	}
	int INDEX = returnindex();
	output();
	while(INDEX < instructionlen) {
		vector<string> instruction = alloperations.at(INDEX);
		if (instruction.at(0) ==  "EBREAK" || instruction.at(0) == "ECALL"|| instruction.at(0) == "FENCE"){break;}
		dooo(instruction);
		output();
		PC = PC + 4;
		INDEX = returnindex();
	}
}
void program::output()
{
    cout << "Register Values:" << endl;
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << left << "                     "<< "Register Name" << "                     "<< "Register Number" << "                     "<< "Binary Value" << "                     "<< "Decimal Value" << "                     "<< "Hexadecimal Value" << endl;
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    for (int i = 0; i < 32; i++)
    {
        cout << left << "                     " << RegistersN[i] << "                     " << "x" + to_string(i) << "                     " << Reg[i] << endl;// << "                     " << DecToBin(Reg[i])  << "                     "<< DecToHex(Reg[i])
    }
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;

        cout << "Memory Values:" << endl;
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
        cout << left << "                     " << "Memory Address" << "                     " << "Binary Value" << "                     "<< "Decimal Value" <<"                     " << "Hexadecimal Value" << endl;
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
        for (auto i : Memory)
            cout << left << "                     " << i.first << "                     " << i.second << endl;// <<<< "                     "<< DecToBin(i.second) <<"                     " << DecToHex(i.second) 
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << "Program Counter:" << endl << PC << endl;
}

void program::setPC(unsigned int st)
{
	if (st < startingPC) { // or < startingPC - 4 not sure yet
		errorr("Wrong address");
		return;}	
        else {	PC = st;}
}

unsigned int program::getPC()
{
	return PC;
}

void program::DoR(string reg, int valuee)
{
    if (!checkregister(reg)) {
        errorr("Wrong Register Name");
        return;
    }
    int registernm;
    if (reg[0] == 'X') {
        stringstream ss(reg.substr(1, reg.length() - 1));
        if (!(ss >> registernm)) {
            errorr("Wrong Register Name");
            return;
        }
    } else {
        registernm = find(RegistersN.begin(), RegistersN.end(), reg) - RegistersN.begin();
    }

    if (registernm >= 32 || registernm < 0) {
        errorr("Wrong Register Name");
        return;
    }
    if (registernm != 0) {
        Reg[registernm] = valuee;
    } else {
        errorr("Cannot change the value of Register ZERO");
    }

    return;
}
void program::errorr(string error)
{
	cout << error << endl;
	exit(1);
}
int program::HaveR(string in) {
    if (!checkregister(in)) {
        errorr("Wrong Register Name");
        return 0;
    }
    int registernm;
    if (in[0] == 'X') {
        if (!checkinteger(in.substr(1))) {
            errorr("Wrong Register Num");
            return 0;
        }
        registernm = stoi(in.substr(1));
    } else {
        auto it = find(RegistersN.begin(), RegistersN.end(), in);
        if (it == RegistersN.end()) {
            errorr("Wrong Register Name");
            return 0;
        }
        registernm = distance(RegistersN.begin(), it);
    }

    if (registernm >= 32 || registernm < 0) {
        errorr("Wrong Register Num");
        return 0;
    }
    return Reg[registernm];
}
unsigned int program::returnindex()
{
	return (PC - startingPC) / 4;
}

bool program::checkinteger(string s) {
    if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;
    char * p;
    strtol(s.c_str(), &p, 10);
    return (*p == 0);
}
bool program::checkregister(string s)
{
	if (s.empty()) {
		return false;
	} else if (find(RegistersN.begin(), RegistersN.end(), s) != RegistersN.end()) {
		return true;
	} else if (s[0] == 'X') {
		if (checkinteger(s.substr(1, s.length() - 1))) {
			int number = stoi((s.substr(1, s.length() - 1)));
			if (number < 32 && number >= 0) {
				return true;
			}
		}
	}else {
        return false;
    }
    return false; // to ignore the warning of non-void function return value
    }

void program:: dooo(vector<string> operationn) {
    string oper = operationn.at(0);
    if (oper == "ADD") {
        ADD(operationn);
    } else if (oper == "ADDI") {
        ADDI(operationn);
    } else if (oper == "AND") {
        AND(operationn);
    } else if (oper == "ANDI") {
        ANDI(operationn);
    } else if (oper == "AUIPC") {
        AUIPC(operationn);
    } else if (oper == "BEQ") {
        BEQ(operationn);
    } else if (oper == "BGE") {
        BGE(operationn);
    } else if (oper == "BGEU") {
        BGEU(operationn);
    } else if (oper == "BLT") {
        BLT(operationn);
    } else if (oper == "BLTU") {
        BLTU(operationn);
    } else if (oper == "BNE") {
        BNE(operationn);
    } else if (oper == "JAL") {
        JAL(operationn);
    } else if (oper == "JALR") {
        JALR(operationn);
    } else if (oper == "LB") {
        LB(operationn);
    } else if (oper == "LBU") {
        LBU(operationn);
    } else if (oper == "LH") {
        LH(operationn);
    } else if (oper == "LHU") {
        LHU(operationn);
    } else if (oper == "LUI") {
        LUI(operationn);
    } else if (oper == "LW") {
        LW(operationn);
    } else if (oper == "OR") {
        OR(operationn);
    } else if (oper == "ORI") {
        ORI(operationn);
    } else if (oper == "SB") {
        SB(operationn);
    } else if (oper == "SH") {
        SH(operationn);
    } else if (oper == "SLL") {
        SLL(operationn);
    } else if (oper == "SLLI") {
        SLLI(operationn);
    } else if (oper == "SLT") {
        SLT(operationn);
    } else if (oper == "SLTI") {
        SLTI(operationn);
    } else if (oper == "SLTIU") {
        SLTIU(operationn);
    } else if (oper == "SLTU") {
        SLTU(operationn);
    } else if (oper == "SRA") {
        SRA(operationn);
    } else if (oper == "SRAI") {
        SRAI(operationn);
    } else if (oper == "SRL") {
        SRL(operationn);
    } else if (oper == "SRLI") {
        SRLI(operationn);
    } else if (oper == "SUB") {
        SUB(operationn);
    } else if (oper == "SW") {
        SW(operationn);
    } else if (oper == "XOR") {
        XOR(operationn);
    } else if (oper == "XORI") {
        XORI(operationn);
    } else {
       errorr("Wrong Operation");
    }
}

void program::LUI(vector<string> operation) {
    int immediate;
    if (!isValidImmediate(operation.at(2), immediate)) {
        errorr("Wrong immediate");
        return;
    }
    DoR(operation.at(1), immediate << 12);
}

void program::AUIPC(vector<string> operation) {
    int immediate;
    if (!isValidImmediate(operation.at(2), immediate)) {
        errorr("Wrong immediate");
        return;
    }
    DoR(operation.at(1), (immediate << 12) + PC);
}

void program::JALR(vector<string> operation) {
    int immediate;
    if (!isValidImmediate(operation.at(2), immediate)) {
        errorr("Wrong immediate");
        return;
    }
    DoR(operation.at(1), immediate + HaveR(operation.at(3)) + 4);
    setPC(immediate + HaveR(operation.at(3)) - 4);
}

void program::JAL(vector<string> operation) {
	DoR(operation.at(1), getPC() + 4);
	if (funcs.find(operation.at(2)) == funcs.end()) {
		errorr("Function doesn't exist");
	}
	else {
		setPC( (funcs.find(operation.at(2)) -> second)-4 );
	}
}

void program::BEQ(vector<string> operation) {
	if (funcs.find(operation.at(3)) == funcs.end()) {
		errorr("Function doesn't exist");
		return;
	}
	if (HaveR(operation.at(1)) == HaveR(operation.at(2)))
	{
		setPC((funcs.find(operation.at(3))->second) - 4);
	}
}

void program::BNE(vector<string> operation) {
	if (funcs.find(operation.at(3)) == funcs.end()) {
		errorr("Function doesn't exist");
		return;
	}
	if (HaveR(operation.at(1)) != HaveR(operation.at(2)))
	{
		setPC((funcs.find(operation.at(3))->second) - 4);
	}
}

void program::BLT(vector<string> operation) {
	if (funcs.find(operation.at(3)) == funcs.end()) {
		errorr("Function doesn't exist");
		return;
	}
	if (HaveR(operation.at(1)) < HaveR(operation.at(2)))
	{
		setPC((funcs.find(operation.at(3))->second) - 4);
	}
}

void program::BGE(vector<string> operation) {
	if (funcs.find(operation.at(3)) == funcs.end()) {
		errorr("Function doesn't exist");
		return;
	}
	if (HaveR(operation.at(1)) >= HaveR(operation.at(2)))
	{
		setPC((funcs.find(operation.at(3))->second) - 4);
	}
}

void program::SLTI(vector<string> operation) {
    int immediate;
    if (operation.size() < 4 || !isValidImmediate(operation.at(3), immediate)) {
        errorr("Wrong immediate");
        return;
    }
    if (HaveR(operation.at(2)) < immediate) {
        DoR(operation.at(1), 1);
    } else {
        DoR(operation.at(1), 0);
    }
}

void program::SLTIU(vector<string> operation) {
    int immediate;
    if (operation.size() < 4 || !isValidImmediate(operation.at(3), immediate)) {
        errorr("Wrong immediate");
        return;
    }
    if ((int)HaveR(operation.at(2)) < immediate) {
        DoR(operation.at(1), 1);
    } else {
        DoR(operation.at(1), 0);
    }
}

void program::ADDI(vector<string> operation) {
    int immediate;
    if (operation.size() < 4 || !isValidImmediate(operation.at(3), immediate)) {
        errorr("Wrong immediate");
        return;
    }
    DoR(operation.at(1), immediate + HaveR(operation.at(2)));
}

void program::ORI(vector<string> operation) {
    int immediate;
    if (operation.size() < 4 || !isValidImmediate(operation.at(3), immediate)) {
        errorr("Wrong immediate");
        return;
    }
    DoR(operation.at(1), immediate | HaveR(operation.at(2)));
}

void program::XORI(vector<string> operation) {
    int immediate;
    if (operation.size() < 4 || !isValidImmediate(operation.at(3), immediate)) {
        errorr("Wrong immediate");
        return;
    }
    DoR(operation.at(1), immediate ^ HaveR(operation.at(2)));
}

void program::ANDI(vector<string> operation) {
    int immediate;
    if (operation.size() < 4 || !isValidImmediate(operation.at(3), immediate)) {
        errorr("Wrong immediate");
        return;
    }
    DoR(operation.at(1), immediate & HaveR(operation.at(2)));
}

void program::SLLI(vector<string> operation) {
    int immediate;
    if (operation.size() < 4 || !isValidImmediate(operation.at(3), immediate)) {
        errorr("Wrong immediate");
        return;
    }
    DoR(operation.at(1), HaveR(operation.at(2)) << immediate);
}

void program::SRLI(vector<string> operation) {
    int immediate;
    if (operation.size() < 4 || !isValidImmediate(operation.at(3), immediate)) {
        errorr("Wrong immediate");
        return;
    }
    DoR(operation.at(1), (int)(HaveR(operation.at(2))) >> immediate);
}

void program::SRAI(vector<string> operation) {
    int immediate;
    if (operation.size() < 4 || !isValidImmediate(operation.at(3), immediate)) {
        errorr("Wrong immediate");
        return;
    }
    DoR(operation.at(1), HaveR(operation.at(2)) >> immediate);
}

bool program::isValidImmediate(const string& immediateStr, int& immediate) {
    try {
        immediate = stoi(immediateStr);
        return true;
    } catch (const invalid_argument&) {
        return false;
    } catch (const out_of_range&) {
        return false;
    }
}
void program::ADD(vector<string> operation) {
	DoR(operation.at(1), HaveR(operation.at(2)) + HaveR(operation.at(3)));
}

void program::SUB(vector<string> operation) {
	DoR(operation.at(1), HaveR(operation.at(2)) - HaveR(operation.at(3)));
}

void program::SLL(vector<string> operation) {
	DoR(operation.at(1), HaveR(operation.at(2)) << HaveR(operation.at(3)));
}

void program::SLT(vector<string> operation) {
	if (HaveR(operation.at(2)) < HaveR(operation.at(3))) {
		DoR(operation.at(1), 1);
	}
	else {
		DoR(operation.at(1), 0);
	}
}

void program::SLTU(vector<string> operation) {
	if ((int)HaveR(operation.at(2)) < (int)HaveR(operation.at(3))) {
		DoR(operation.at(1), 1);
	}
	else {
		DoR(operation.at(1), 0);
	}
}
void program::MUL(vector<string> operation) {
	int32_t finall = static_cast<int32_t>((int64_t(HaveR(operation.at(2)) * int64_t(HaveR(operation.at(3))))));
	DoR(operation.at(1),finall);
}

void program::MULH(vector<string> operation) {
	int32_t finall = static_cast<int32_t>((int64_t(HaveR(operation.at(2)) * int64_t(HaveR(operation.at(3))))) >> 32);
	DoR(operation.at(1), finall);
}

void program::MULHU(vector<string> operation) {
	uint32_t finall = static_cast<uint32_t>((uint64_t(HaveR(operation.at(2)) * uint64_t(HaveR(operation.at(3))))) >> 32);
	DoR(operation.at(1), finall);
}

void program::MULHSU(vector<string> operation) {
	int32_t finall = static_cast<int32_t>((int64_t(HaveR(operation.at(2)) * uint64_t(HaveR(operation.at(3))))) >> 32);
	DoR(operation.at(1), finall);
}

void program::DIV(vector<string> operation) {
	int finall = HaveR(operation.at(2)) / HaveR(operation.at(3));
	DoR(operation.at(1), finall);
}

void program::DIVU(vector<string> operation) {
	int finall = (int)HaveR(operation.at(2)) / (int)HaveR(operation.at(3));
	DoR(operation.at(1), finall);
}

void program::REM(vector<string> operation) {
	int finall = HaveR(operation.at(2)) % HaveR(operation.at(3));
	DoR(operation.at(1), finall);
}

void program::REMU(vector<string> operation) {
	int finall = (int)HaveR(operation.at(2)) % (int)HaveR(operation.at(3));
	DoR(operation.at(1), finall);
}
void program::XOR(vector<string> operation) {
	DoR(operation.at(1), HaveR(operation.at(2)) ^ HaveR(operation.at(3)));
}

void program::SRL(vector<string> operation) {
	DoR(operation.at(1), (int)HaveR(operation.at(2)) >> HaveR(operation.at(3)));
}

void program::SRA(vector<string> operation) {
	DoR(operation.at(1), HaveR(operation.at(2)) >> HaveR(operation.at(3)));
}

void program::OR(vector<string> operation) {
	DoR(operation.at(1), HaveR(operation.at(2)) | HaveR(operation.at(3)));
}

void program::AND(vector<string> operation) {
	DoR(operation.at(1), HaveR(operation.at(2)) & HaveR(operation.at(3)));
}

void program::BGEU(vector<string> operation) {}

void program::BLTU(vector<string> operation) {}

void program::LB(vector<string> operation) {}

void program::LBU(vector<string> operation) {}

void program::LH(vector<string> operation) {}

void program::LHU(vector<string> operation) {}

void program::LW(vector<string> operation) {}

void program::SB(vector<string> operation) {}

void program::SH(vector<string> operation) {}

void program::SW(vector<string> operation) {}
int main() {
	string alloperations = "instructions.txt";
	string data = "Data.txt";
    int startingPC;
    cout << "Enter the starting address: ";
    cin >> startingPC;
	program run(alloperations, data, startingPC);
	return 0;
}
