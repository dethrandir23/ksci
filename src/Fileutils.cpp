//FileUtils.cpp
#include "../include/FileUtils.hpp"

#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <sstream>

namespace FileUtils
{

    std::vector<std::string> readLines(const std::string& filePath) {
        std::vector<std::string> lines;
        std::string line;

        std::ifstream file(filePath);

        while(getline(file, line)) {
            lines.push_back(line);
        }

        file.close();
        return lines;
    }

    std::vector<std::string> tokenizeLine(const std::string& line) {
        std::vector<std::string> tokens;
        std::string token;
        bool inQuotes = false;
        char quoteChar = 0;
        
        for (char c : line) {
            if ((c == '"' || c == '\'') && !inQuotes) {
                inQuotes = true;
                quoteChar = c;
                token += c;
            } else if (c == quoteChar && inQuotes) {
                inQuotes = false;
                token += c;
                tokens.push_back(token);
                token = "";
            } else if (c == ' ' && !inQuotes) {
                if (!token.empty()) {
                    tokens.push_back(token);
                    token = "";
                }
            } else {
                token += c;
            }
        }
        
        if (!token.empty()) {
            tokens.push_back(token);
        }
        
        return tokens;
    }


    std::vector<std::vector<std::string>> tokenizeLines(std::vector<std::string> lines) {
        std::vector<std::vector<std::string>> tokenizedLines;
        for (const auto& line : lines) {
            if (line.empty() || line[0] == '#') continue;
            tokenizedLines.push_back(tokenizeLine(line));
        }

        return tokenizedLines;
    }


} //namespace FileUtils