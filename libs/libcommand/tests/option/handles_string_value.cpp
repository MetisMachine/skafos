#include <cstring>
#include <iostream>

#include "option.h"

using namespace std;
using namespace command;

#define NAME "test"
#define VALUE "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"

#define OPTION NAME "=" VALUE

typedef std::string OptionType;

OptionType test;

void _function(OptionType value) {
    test = value;
}

int main() {
    Option<OptionType> option(NAME, "Option with string value", _function);

    if (option.understand(OPTION)) {
        option.handle();
    }
    else {
        cout << option.describe() << " do not understand string values\n";
        return 1;
    }

    int cmp = strcmp(test.c_str(), VALUE);

    if (cmp == 0) {
        cout << option.describe() << " handles string values\n";
        return 0;
    }

    cout << "Option class do not handle string values\n";

    return 1;
}
