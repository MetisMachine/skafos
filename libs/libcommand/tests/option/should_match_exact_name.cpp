#include <iostream>
#include <vector>

#include "option.h"

#define NAME "test"

using namespace std;
using namespace command;

void _function(void) { }

int main() {
    std::vector<std::string> badOptions;
    badOptions.push_back("--test");
    badOptions.push_back("-test");
    badOptions.push_back("tes");
    badOptions.push_back("te");
    badOptions.push_back("t");

    Option<void> option(NAME, "Option should match only exact name", _function);
    for (std::string bad : badOptions) {
        if (option.understand(bad)) {
            std::cout << option.describe() << " but '" << NAME << "' was matched as same to '" << bad << "'\n";
        }
    }

    if (option.understand(NAME)) {
        std::cout << option.describe() << " and it understands it correctly\n";
        return 0;
    }

    std::cout << option.describe() << " but no name was found\n";

    return 1;
}
