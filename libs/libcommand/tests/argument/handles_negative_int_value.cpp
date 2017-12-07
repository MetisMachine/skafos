#include <iostream>

#include "argument.h"

using namespace std;
using namespace command;

#define VALUE "-1234567890"

typedef int ArgumentType;

ArgumentType test;

void _function(ArgumentType value) {
    test = value;
}

int main() {
    Argument<ArgumentType> argument("Argument as negative int", _function);

    if (argument.understand(VALUE)) {
        argument.handle();
    }
    else {
        cout << "Argument class do not understand negative int values\n";
        return 1;
    }

    if (test == std::stoi(VALUE)) {
        cout << "Argument class handles negative int values\n";
        return 0;
    }

    cout << "Argument class do not handle negative int values\n";

    return 1;
}
