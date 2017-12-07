#include <iostream>

#include "argument.h"

using namespace std;
using namespace command;

#define VALUE "-567890.1234"

typedef float ArgumentType;

ArgumentType test;

void _function(ArgumentType value) {
    test = value;
}

int main() {
    Argument<ArgumentType> argument("Argument as negative float", _function);

    if (argument.understand(VALUE)) {
        argument.handle();
    }
    else {
        cout << argument.describe() << " do not understand " << VALUE " value\n";
        return 1;
    }

    if (test == std::stof(VALUE)) {
        cout << argument.describe() << " handles negative float values\n";
        return 0;
    }

    cout << argument.describe() << " do not handle negative float values\n";

    return 1;
}
