#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <chrono>
#include <thread>

enum InstructionType {
    NOOP,
    ADDX,
};

struct Instruction {
    InstructionType instruction;
    int parameter;
};

class CPU {
public:
    CPU(const std::vector<Instruction> &instructions): instructions(instructions) {}
    void reset() {
        working_instructions = instructions;
        remaining_program = getProgramCycles();
        remaining_cycles = 0;
        reg_x = 1;
        current_instruction = {NOOP, 0};
    }
    int getRemainingInstructions() {
        return remaining_program;
    }
    void performCycle() {
        if(remaining_cycles == 0) {
            if(current_instruction.instruction == ADDX) {
                reg_x += current_instruction.parameter;
            }
            current_instruction = working_instructions.back();
            working_instructions.pop_back();
            remaining_cycles = getInstructionCycles(current_instruction);
        }
        remaining_cycles--;
        remaining_program--;
    }
    int getX() {
        return reg_x;
    }
private:
    int getProgramCycles() {
        int result = 0;
        for(auto &instruction : working_instructions) {
            result += getInstructionCycles(instruction);
        }
        return result;
    }
    int getInstructionCycles(const Instruction &instruction) {
        switch(instruction.instruction) {
            case NOOP:
                return 1;
            case ADDX:
                return 2;
        }
    }
    int reg_x;
    Instruction current_instruction;
    int remaining_cycles;
    int remaining_program;
    std::vector<Instruction> working_instructions;
    std::vector<Instruction> instructions;
};

CPU getCPU( std::ifstream &file ) {
    std::vector<Instruction> instructions{};
    std::string str;
    int parameter;
    while ( std::getline( file, str ) ) {
        if(str[0] == 'n') {
            instructions.push_back({NOOP, 0});
        } else {
            std::stringstream ss( str.substr(4) );
            ss >> parameter;
            instructions.push_back( {ADDX, parameter} );
        }
    }
    std::reverse(instructions.begin(), instructions.end());
    return CPU(instructions);
}

int part1(CPU &cpu) {
    int result = 0;
    cpu.reset();
    for(int i = 0; i < 20; i++) {
        cpu.performCycle();
    }
    result += 20 * cpu.getX();
    int cycles = 0;
    while(true) {
        cycles++;
        cpu.performCycle();
        if(cycles % 40 == 0) {
            result += (20+cycles) * cpu.getX();
            if(cpu.getRemainingInstructions() < 40) {
                break;
            }
        }
    }
    return result;
}

void part2(CPU &cpu) {
    cpu.reset();
    int x = 0;
    for(int i = 0; i < 240; i++) {
        cpu.performCycle();
        if(cpu.getX() >= x - 1 && cpu.getX() <= x + 1) {
            std::cout << '#';
        } else {
            std::cout << ' ';
        }
        x++;
        if(x == 40) {
            x = 0;
            std::cout << std::endl;
        }
    }
}

void visualize(CPU &cpu) {
    cpu.reset();
    int x = 0;
    std::cout << "\033[?25l";
    std::cout << "▓" << std::flush;
    for(int i = 0; i < 240; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(25));
        cpu.performCycle();
        if(cpu.getX() >= x - 1 && cpu.getX() <= x + 1) {
            std::cout << "\010█";
        } else {
            std::cout << "\010 ";
        }
        x++;
        if(x == 40) {
            x = 0;
            std::cout << std::endl;
        }
        std::cout << "▓" << std::flush;
    }
    std::cout << "\010\033[?25h" << std::flush;
}

int main() {
    std::ifstream input_file( "input" );
    auto cpu = getCPU( input_file );
    std::cout << "Part 1 result is \033[91;1m" << part1(cpu)
              << "\033[0m." << std::endl;
    std::cout << "The CRT provides the following image:" << std::endl;
    std::cout << "\033[91;1m";
    part2(cpu);
    std::cout << "\033[0m";
//    visualize(cpu);
}
