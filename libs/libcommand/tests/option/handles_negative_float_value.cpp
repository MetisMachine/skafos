#include <cstring>
#include <iostream>

#include "option.h"

using namespace std;
using namespace command;

#define NAME "test"
#define VALUE "-567890.1234"

#define OPTION NAME "=" VALUE

typedef float OptionType;

OptionType test;

void _function(OptionType value) {
    test = value;
}

int main() {
    Option<OptionType> option(NAME, "Option with negative float value", _function);

    if (option.understand(OPTION)) {
        option.handle();
    }
    else {
        cout << option.describe() << " do not understand " << VALUE << " value\n";
        return 1;
    }

    if (test == std::stof(VALUE)) {
        cout << option.describe() << " handles " << VALUE << " value\n";
        return 0;
    }

    cout << "Option class do not handle negative float values\n";

    return 1;
}
