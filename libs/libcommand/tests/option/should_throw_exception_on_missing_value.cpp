#include <iostream>
#include <vector>

#include "option.h"

#define NAME "test"
#define BAD_OPTION "test="

using namespace std;
using namespace command;

void _function(int) { }

int main() {
    Option<int> option(NAME, "Option should throw exception on missing value", _function);

    try {
        if (option.understand(BAD_OPTION)) {
            std::cout << option.describe() << " but instead it understanded it\n";
        }
    }
    catch(std::invalid_argument e) {
        std::cout << option.describe() << " and it was thrown correctly\n";
        return 0;
    }

    std::cout << option.describe() << " but nothing happened\n";
    return 1;
}
