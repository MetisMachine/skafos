#include <iostream>
#include <string>
#include <functional>

#include "option.h"
#include "argument.h"
#include "required.h"
#include "multiValue.h"
#include "grouped.h"
#include "command.h"

using namespace command;

/**
 * @file
 * @brief Simple example explaining how to bind functions to command-line parameters
 */

void argument_function(bool a) {
    std::cout << "Argument: " << a << std::endl;
}

void option_function(std::string a) {
    std::cout << "Help function " << a << std::endl;
}

void void_function(void) {
    std::cout << "Void function " << std::endl;
}

int main(int argc, char **argv)
{
    try {
        Command command(argc, argv, {
            // function with bool parameter
            new Argument<bool>("Input values", argument_function),

            // function with std::string parameter
            new Option<std::string>("f", "Optional file", option_function),

            // function without parameter
            new Option<void>("h", "Help", void_function)
        });

        /* Code is initialized.
         *
         * You can run your main program e.g. here
         */
    }
    catch(const std::exception & e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
