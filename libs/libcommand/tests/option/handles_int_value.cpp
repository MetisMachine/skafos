#include <cstring>
#include <iostream>

#include "option.h"

using namespace std;
using namespace command;

#define NAME "test"
#define VALUE "1234567890"

#define OPTION NAME "=" VALUE

typedef int OptionType;

OptionType test;

void _function(OptionType value) {
    test = value;
}

int main() {
    Option<OptionType> option(NAME, "Option as int", _function);

    if (option.understand(OPTION)) {
        option.handle();
    }
    else {
        cout << option.describe() << " do not understand int values\n";
        return 1;
    }

    if (test == std::stoi(VALUE)) {
        cout << option.describe() << " handles int values\n";
        return 0;
    }

    cout << option.describe() << " do not handle int values\n";

    return 1;
}
