#include <iostream>

#include "argument.h"

using namespace std;
using namespace command;

#define FALSE "0"
#define TRUE "1"

typedef bool ArgumentType;

ArgumentType test;

void _function(ArgumentType value) {
    test = value;
}

int main() {
    Argument<ArgumentType> argument("Argument as boolean", _function);

    if (argument.understand(FALSE)) {
        argument.handle();
    }
    else {
        cout << "Argument class do not understand boolean (FALSE) values\n";
        return 1;
    }

    if (test == (bool)std::stoi(FALSE)) {
        cout << "Argument class handles boolean (FALSE) values\n";
    }

    Argument<ArgumentType> argument2("Argument as boolean", _function);
    if (argument2.understand(TRUE)) {
        argument2.handle();
    }
    else {
        cout << "Argument class do not understand boolean (TRUE) values\n";
        return 1;
    }

    if (test == (bool)std::stoi(TRUE)) {
        cout << "Argument class handles boolean (TRUE) values\n";
        return 0;
    }

    cout << "Argument class do not handle boolean values\n";

    return 1;
}
