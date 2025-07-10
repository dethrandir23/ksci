//FileUtils.hpp
#pragma once

#include <string>
#include <vector>

namespace FileUtils
{

    std::vector<std::string> readLines(const std::string& filePath);

    std::vector<std::string> tokenizeLine(const std::string& line);

    std::vector<std::vector<std::string>> tokenizeLines(std::vector<std::string> lines);

} //namespace FileUtils