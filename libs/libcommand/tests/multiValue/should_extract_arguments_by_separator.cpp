#include <iostream>
#include <vector>

#include "argument.h"
#include "multiValue.h"

using namespace std;
using namespace command;

#define VALUE "0,1,2,3,4,5,6,7,8,9"

typedef int ArgumentType;

std::vector<ArgumentType> input;

void _function(ArgumentType value) {
    input.push_back(value);
    cout << "Catched value: " << value << "\n";
}

int main() {
    Parameter * argument = new MultiValue(",", new Argument<ArgumentType>("Argument as multiValue int", _function));

    try {
        if (argument->understand(VALUE)) {
            argument->handle();
        }
        else {
            cout << argument->describe() << " should understand multiple int values\n";
            return 1;
        }
    }
    catch (...) {
        delete argument;
        cout << argument->describe() << " thrown unknown exception\n";
        return 1;
    }

    bool test = true;
    for (int i = 0; i < 10; i++) {
        test &= (input[i] == i);
        cout << i << ") input: " << input[i] << "\n";
    }

    if (test) {
        cout << argument->describe() << " handles boolean (TRUE) values\n";
        delete argument;
        return 0;
    }

    cout << argument->describe() << " do not handle multiple int values\n";
    delete argument;
    return 1;
}
