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
 * @brief Simple example explaining how to bind class methods to command-line parameters
 */

class ExampleClass {
public:
    void _argument(bool a) {
        std::cout << "Argument: " << a << std::endl;
    }
    void _option(std::string a) {
        std::cout << "Help function " << a << std::endl;
    }
    void _void(void) {
        std::cout << "Void function " << std::endl;
    }
};

int main(int argc, char **argv)
{
    ExampleClass c;

    try {
        Command command(argc, argv, {
            // class method with bool parameter
            new Argument<bool>("Input values", std::bind(&ExampleClass::_argument, &c, std::placeholders::_1)),

            // class method std::string parameter
            new Option<std::string>("f", "Optional file", std::bind(&ExampleClass::_option, &c, std::placeholders::_1)),

            // class method without parameter
            new Option<void>("h", "Help", std::bind(&ExampleClass::_void, &c))
        });

        /* Code is initialized.
         *
         * You can run your main program for example here
         */
    }
    catch(const std::exception & e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
