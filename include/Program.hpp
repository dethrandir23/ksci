// Program.hpp
#pragma once

#include <stdexcept>
#include <string>
#include <iostream>
#include <stack>

#include "../include/FileUtils.hpp"
#include "../include/InstructionParser.hpp"

class Program {
public:
    Program(const std::string& filePath) : filePath(filePath), error_code(0) {}

    void run() {
        try {
            auto lines = FileUtils::readLines(filePath);
            auto tokenizedLines = FileUtils::tokenizeLines(lines);
            
            auto instructedLines = InstructionParser::ParseInstructionLines(tokenizedLines);
            
            InstructionParser::Program program;
            findLabels(instructedLines, program);
            
            size_t pc = 0;
            std::stack<size_t> callStack; 
            
            while(pc < instructedLines.size()) {
                auto& inst = instructedLines[pc];

                //ENABLE FOR DEBUG: std::cout << "DEBUG: Executing line " << inst.lineNumber << ": " << std::endl;
                
                if(inst.type == InstructionParser::Type::LABEL) {
                    pc++;
                    continue;
                }
                
                auto result = InstructionParser::InterpretLine(inst, program);
                
                if(!result.isSuccessful) {
                    error_code = 1;
                    throw std::runtime_error("Execution error at line " + std::to_string(inst.lineNumber));
                }
                else if(result.isCALL) {
                    callStack.push(pc + 1);
                    pc = result.lineNumberGOTO;
                }
                else if(result.isRETURN) {
                    if(callStack.empty()) {
                        throw std::runtime_error("RETURN without CALL at line " + std::to_string(inst.lineNumber));
                    }
                    pc = callStack.top();
                    callStack.pop();
                }
                if(result.isGOTO) {
                    pc = result.lineNumberGOTO;
                } 
                else {
                    pc++;
                }

                //ENABLE FOR DEBUG: std::cout << "DEBUG: Next PC: " << pc << std::endl;
            }
        } 
        catch(const std::exception& e) {
            error_code = 1;
            std::cerr << "ERROR: " << e.what() << std::endl;
        }
        
        std::cout << std::endl << "Program exited with code: " << error_code << std::endl;
    }

private:
    std::string filePath;
    int error_code;
    
    void findLabels(const std::vector<InstructionParser::Instruction>& instructions, 
                InstructionParser::Program& program) {
        for(const auto& inst : instructions) {
            if(inst.type == InstructionParser::Type::LABEL) {
                if(!inst.args.empty()) {
                    // Save label and line number.
                    program.labels[inst.args[0]] = inst.lineNumber+1;
                }
            }
        }
    }
};