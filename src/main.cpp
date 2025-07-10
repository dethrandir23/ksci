//main.cpp

#include "../libs/cliopatra/include/cliopatra/Cliopatra.h"
#include "../include/Program.hpp"

#include <string>
#include <iostream>

int main(int argc, char** argv) {

    Cliopatra cliopatra;

    cliopatra.addOption("r", "run", Cliopatra::Option::bool_o);
    cliopatra.addOption("n", "name", Cliopatra::Option::string_o);

    auto parsedValue = cliopatra.parse(argc, argv);
    

    if(parsedValue.count("run")) {
        if(parsedValue.count("name")) {
            auto file = parsedValue["name"];
            std::string filePath = std::get<std::string>(file);
            Program program(filePath);
            program.run();
            //ENABLE FOR DEBUG: std::cout << "DEBUG: program ran!" << std::endl;
        }
    }


    return 0;
}