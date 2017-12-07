#include <iostream>
#include <command/command.h>
#include <command/option.h>

using namespace command;

/**
 * Just a simple example allowing to display help information if user will set
 * command line option: -h, e.g.:
 *
 *  ./a.out -h
 *
 */
int main(int argc, char *argv[]) {
    try {
        Command command(argc, argv, {
            new Option<void>("-h", "Help", [](void) {
                std::cout << "Help information\n";
            })
        });
    }
    catch(const std::exception & e) {
        return 1;
    }

    return 0;
}
