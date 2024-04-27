# Risk-V-Simulator

## Authors
Saif Ahmed, Mohamed Mansour, Merna Hebishy  
900225535, 900222990, 900221976

## Description
Risk-V-Simulator is a program designed for simulating the execution of RISC-V assembly code. It provides a robust and versatile platform utilizing a well-structured class-based design. The simulator encapsulates core functionalities such as loading, parsing, and executing instructions, managing register values, memory contents, and program flow.

## Features
- Supports a wide range of RISC-V instructions including arithmetic, logical, memory access, control transfer, and bitwise operations.
- Additional support for bonus instructions such as multiplication, division, and remainder operations.
- Comprehensive error handling mechanisms ensuring the integrity of the simulation process with informative error messages.

## User Guide

### System Requirements
- **Operating System:** Compatible with Windows, macOS, and Linux.
- **Compiler:** Requires a C++ compiler such as g++, clang++, or Visual C++ compiler.

### Getting Started
1. **Download:** Obtain the program source code and necessary files.
2. **Compilation:** Compile the source code using your preferred C++ compiler.
3. **Terminal Access:** Open the terminal or command prompt.
4. **Navigate:** Use the `cd` command to navigate to the directory with the compiled executable.

### Command Line Arguments
The program requires three command-line arguments:
- `<instruction_file>`: Path to the file containing instructions.
- `<data_file>`: Path to the file containing data.
- `<starting_PC>`: Starting PC address for program execution.

### Running the Program
Execute the program with the following command:
./program <instruction_file> <data_file> <starting_PC>
Replace `program` with the name of the compiled executable and provide the paths to the instruction and data files along with the starting PC address.
