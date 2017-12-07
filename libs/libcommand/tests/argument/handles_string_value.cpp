#include <cstring>
#include <iostream>

#include "argument.h"

using namespace std;
using namespace command;

#define VALUE "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"

typedef std::string ArgumentType;

ArgumentType test;

void _function(ArgumentType value) {
    test = value;
}

int main() {
    Argument<ArgumentType> argument("Argument as string", _function);

    if (argument.understand(VALUE)) {
        argument.handle();
    }
    else {
        cout << "Argument class do not understand string values\n";
        return 1;
    }

    int cmp = strcmp(test.c_str(), VALUE);

    if (cmp == 0) {
        cout << "Argument class handles string values\n";
        return 0;
    }

    cout << "Argument class do not handle string values\n";

    return 1;
}
