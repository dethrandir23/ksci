//InstructionParser.cpp
#include "../include/InstructionParser.hpp"
#include <ostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <iostream>
#include <chrono>
#include <thread>


#define STRING_TO_ENUM(NAME) if (str == #NAME) return Type::NAME

namespace InstructionParser
{

    Type stringToType(const std::string& str) {

        // ENABLE FOR DEBUG: std::cerr << "DEBUG: Trying to parse type: " << str << std::endl;
        STRING_TO_ENUM(SET);
        STRING_TO_ENUM(GPIO);
        STRING_TO_ENUM(DELAY);
        STRING_TO_ENUM(GOTO);
        STRING_TO_ENUM(LABEL);
        STRING_TO_ENUM(IF);
        STRING_TO_ENUM(READ);
        STRING_TO_ENUM(PRINT);
        STRING_TO_ENUM(ADD);
        STRING_TO_ENUM(SUB);
        STRING_TO_ENUM(CALL);
        STRING_TO_ENUM(RETURN);
        throw std::invalid_argument("Invalid");
    }
    
    Instruction ParseInstructionLine(const std::vector<std::string>& line) {
        Instruction inst;
        inst.type = stringToType(line[0]);

        for (auto token = line.begin() + 1; token != line.end(); ++token) {
            const std::string& arg = *token;
            inst.args.push_back(arg);
        }

        return inst;
    }

    std::vector<Instruction> ParseInstructionLines(const std::vector<std::vector<std::string>>& lines) {
        std::vector<Instruction> instructedLines;

        for (size_t i = 0; i < lines.size(); i++) {
            Instruction inst = ParseInstructionLine(lines[i]);
            inst.lineNumber = i;
            instructedLines.push_back(inst);
        }
        return instructedLines;
    }


    ResultOfLine InterpretLine(const Instruction& inst, Program& program) {
        ResultOfLine result;
        result.isGOTO = false;
        result.isSuccessful = true;

        switch (inst.type) {

            //DEBUG SIMULATIONS FOR PC, NOT REAL.
            case Type::SET:
                if(inst.args.size() != 2) {
                    throw std::runtime_error("Error: SET requires 2 arguments at line " + std::to_string(inst.lineNumber));
                }
                program.variables[inst.args[0]] = std::stoi(inst.args[1]);
                break;
            case Type::GPIO:
                if(inst.args.size() != 2) {
                    throw std::runtime_error("Error: GPIO requires 2 arguments at line " + std::to_string(inst.lineNumber));
                }
                if(inst.args[1] == "ON") {
                    std::cout << "GPIO " + inst.args[0] + "is on now!" << std::endl;
                }
                else if(inst.args[1] == "OFF") {
                    std::cout << "GPIO " + inst.args[0] + "is off now!" << std::endl;
                }
                else {
                    result.isSuccessful = false;
                    throw std::runtime_error("Error: Unknown parameter at line " + std::to_string(inst.lineNumber) + ": " + inst.args[1] + "Is not an available parameter.");
                }
                break;
            case Type::DELAY: {

                auto getValue = [&](const std::string& arg) -> int {
                    if (program.variables.count(arg)) return program.variables[arg];
                    try { return std::stoi(arg); } 
                    catch (...) {
                        throw std::runtime_error("Error: Invalid operand '" + arg + "' at line " + std::to_string(inst.lineNumber));
                    }
                };

                int wait_time = getValue(inst.args[0]);
                try {
                    std::this_thread::sleep_for(std::chrono::milliseconds(wait_time));
                }catch (...) {
                    result.isSuccessful = false;
                    throw std::runtime_error("Error: Please enter an available value at line " + std::to_string(inst.lineNumber));
                }
                break;
            }
            case Type::GOTO:
                if(inst.args.empty()) {
                    throw std::runtime_error("GOTO requires label argument at line " + std::to_string(inst.lineNumber));
                }
                if(program.labels.count(inst.args[0])) {
                    result.isGOTO = true;
                    result.lineNumberGOTO = program.labels[inst.args[0]];
                } else {
                    throw std::runtime_error("Undefined label: " + inst.args[0] + " at line " + std::to_string(inst.lineNumber));
                }
                break;
            case Type::LABEL: //LABEL A FOR EXAMPLE
                break;
            case Type::IF: {
                if (inst.args.size() < 5) {
                    result.isSuccessful = false;
                    throw std::runtime_error("Error: IF requires 5 arguments at line " + std::to_string(inst.lineNumber));
                }

                // take operator values. variable or constant
                auto getValue = [&](const std::string& arg) -> int {
                    if (program.variables.count(arg)) return program.variables[arg];
                    try { return std::stoi(arg); } 
                    catch (...) {
                        throw std::runtime_error("Error: Invalid operand '" + arg + "' at line " + std::to_string(inst.lineNumber));
                    }
                };

                int left = getValue(inst.args[0]);
                int right = getValue(inst.args[2]);
                std::string op = inst.args[1];
                std::string gotoLabel = inst.args[4]; // IF ... GOTO <label>

                //check operator
                bool conditionMet = false;
                if (op == "==") conditionMet = (left == right);
                else if (op == "!=") conditionMet = (left != right);
                else if (op == ">") conditionMet = (left > right);
                else if (op == "<") conditionMet = (left < right);
                else if (op == ">=") conditionMet = (left >= right);
                else if (op == "<=") conditionMet = (left <= right);
                else {
                    result.isSuccessful = false;
                    throw std::runtime_error("Error: Unknown operator '" + op + "' at line " + std::to_string(inst.lineNumber));
                }

                //GOTO if statement is true.
                if (conditionMet) {
                    if (program.labels.find(gotoLabel) != program.labels.end()) {
                        result.isGOTO = true;
                        result.lineNumberGOTO = program.labels[gotoLabel];
                    } else {
                        result.isSuccessful = false;
                        throw std::runtime_error("Error: Label '" + gotoLabel + "' not defined at line " + std::to_string(inst.lineNumber));
                    }
                }
                break;
            }
            case Type::READ:
                program.variables[inst.args[0]] = 987; //a random value to simulate rpi. at real usage, it should be: READ <GPIO> <VALUE>
                break;
            case Type::PRINT: {
                for (auto arg : inst.args) {
                    if (program.variables.count(arg)) {
                        std::cout << program.variables[arg];
                        std::cout.flush();
                    }
                    else if (arg == "ENDL") {
                        std::cout << std::endl;
                    }
                    else if ((arg.front() == '"' && arg.back() == '"') || 
                            (arg.front() == '\'' && arg.back() == '\'')) 
                    {
                        std::cout << arg.substr(1, arg.size() - 2);
                        std::cout.flush();
                    } else {
                        try { 
                            std::cout << std::stoi(arg);
                            std::cout.flush();
                        } catch (...) { 
                            std::cout << arg;
                            std::cout.flush();
                        }
                    }
                }
                break;
            }
            case Type::ADD: {
                if (inst.args.size() < 3) {
                    result.isSuccessful = false;
                    throw std::runtime_error("Error: ADD requires 3 arguments at line " + 
                                            std::to_string(inst.lineNumber));
                }

                auto getValue = [&](const std::string& arg) -> int {
                    if (program.variables.count(arg)) 
                        return program.variables[arg];
                    try { 
                        return std::stoi(arg); 
                    } 
                    catch (...) {
                        throw std::runtime_error("Error: Invalid operand '" + arg + 
                                                "' at line " + std::to_string(inst.lineNumber));
                    }
                };

                int operand1 = getValue(inst.args[1]);
                int operand2 = getValue(inst.args[2]);
                program.variables[inst.args[0]] = operand1 + operand2;
                break;
            }

            case Type::SUB: {
                if (inst.args.size() < 3) {
                    result.isSuccessful = false;
                    throw std::runtime_error("Error: SUB requires 3 arguments at line " + 
                                            std::to_string(inst.lineNumber));
                }

                auto getValue = [&](const std::string& arg) -> int {
                    if (program.variables.count(arg)) 
                        return program.variables[arg];
                    try { 
                        return std::stoi(arg); 
                    } 
                    catch (...) {
                        throw std::runtime_error("Error: Invalid operand '" + arg + "' at line " + std::to_string(inst.lineNumber));
                    }
                };

                int operand1 = getValue(inst.args[1]);
                int operand2 = getValue(inst.args[2]);
                program.variables[inst.args[0]] = operand1 - operand2;
                break;
            }
            case Type::CALL: // CALL SUBROUTINE
                if(inst.args.empty()) {
                    throw std::runtime_error("CALL requires label argument at line " + std::to_string(inst.lineNumber));
                }
                if(program.labels.count(inst.args[0])) {
                    program.call_stack.push(inst.lineNumber + 1);  // CALLBACK ADDRESS
                    result.isCALL = true;
                    result.isGOTO = true;
                    result.lineNumberGOTO = program.labels[inst.args[0]];
                } else {
                    throw std::runtime_error("Undefined label: " + inst.args[0] + " at line " + std::to_string(inst.lineNumber));
                }
                break;
            case Type::RETURN:
                if(program.call_stack.empty()) {
                    throw std::runtime_error("RETURN without CALL at line " + std::to_string(inst.lineNumber));
                }
                result.isRETURN = true;
                result.isGOTO = true;
                result.lineNumberGOTO = program.call_stack.top();
                program.call_stack.pop();
                break;
            default:
                result.isSuccessful=false;
                throw std::runtime_error("Error: Unknown type at line " + std::to_string(inst.lineNumber));
        }
        return result;
    }

}