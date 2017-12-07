#include <iostream>
#include <string>
#include <stdexcept>

#include "option.h"
#include "command.h"

using namespace command;

void option_function(std::string a) {
    std::cout << "Help function " << a << std::endl;
}

int main(int argc, char *argv[]) {
    try {
        Command command(argc, argv, {
            new Option<void>("void", "void Test", [](void) {
                std::cout << "VOID" << std::endl;
            }),
            new Option<char>("char", "char Test", [](char a) {
                std::cout << "char: " << a << std::endl;
            }),
            new Option<bool>("bool", "bool Test", [](bool b) {
                std::cout << "bool: " << b << std::endl;
            }),
            new Option<int>("int", "int Test", [](int i) {
                std::cout << "int: " << i << std::endl;
            }),
            new Option<unsigned int>("unsigned-int", "unsigned int Test", [](unsigned int i) {
                std::cout << "unsigned int: " << i << std::endl;
            }),
            new Option<float>("float", "float Test", [](float f) {
                std::cout << "float: " << f << std::endl;
            }),
            new Option<std::string>("std::string", "std::string Test", [](std::string s) {
                std::cout << "std::string: " << s << std::endl;
            })
        });
    }
    catch(const std::exception & e) {
        std::cout << e.what() << std::endl;
        return 1;
    }

    return 0;
}