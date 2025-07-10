//InstructionParser.hpp
#pragma once

#include <stack>
#include <string>
#include <vector>
#include <unordered_map>

namespace InstructionParser
{

    struct Program {
        std::unordered_map<std::string, int> variables;
        std::unordered_map<std::string, size_t> labels;
        size_t current_line = 0;
        std::stack<size_t> call_stack;
    };

    enum class Type { 
        SET, 
        GPIO, 
        DELAY, 
        GOTO, 
        LABEL, 
        IF, 
        READ, 
        PRINT,
        ADD,
        SUB,
        CALL,
        RETURN
        };

    struct Instruction {
        Type type;
        size_t lineNumber;
        std::vector<std::string> args;
    };

    struct ResultOfLine {
        bool isSuccessful = true;

        bool isGOTO = false;
        bool isCALL = false;
        bool isRETURN = false;

        size_t lineNumberGOTO = 0;
    };

    Instruction ParseInstructionLine(const std::vector<std::string>& line);
    std::vector<Instruction> ParseInstructionLines(const std::vector<std::vector<std::string>>& lines);
    ResultOfLine InterpretLine(const Instruction& inst, Program& program);
}