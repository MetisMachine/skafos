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

void argument_function(bool a) {
    std::cout << "Argument: " << a << std::endl;
}

void option_function(std::string a) {
    std::cout << "Help function " << a << std::endl;
}

void void_function(void) {
    std::cout << "Void function " << std::endl;
}

class ExampleClass {
public:
    void _argument(bool a) {
        argument_function(a);
    }
    void _option(std::string a) {
        option_function(a);
    }
    void _void(void) {
        void_function();
    }
};

int main(int argc, char *argv[]) {
    ExampleClass c;

    try {
        Command command(argc, argv, {
            new Grouped({
                new Required(
                    new MultiValue("-",
                        new Argument<bool>("Input values", std::bind(&ExampleClass::_argument, &c, std::placeholders::_1))
                    )
                ),
                new MultiValue(",",
                    new Option<std::string>("f", "Optional file", std::bind(&ExampleClass::_option, &c, std::placeholders::_1))
                )
            }),
            new Option<void>("h", "Help", std::bind(&ExampleClass::_void, &c)),

            // just a pure method calling
            new Option<void>("v", "version", void_function)
        });

        /* ExampleClass is initialized.
         * You can run your main program now
         */
    }
    catch(const std::exception & e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
