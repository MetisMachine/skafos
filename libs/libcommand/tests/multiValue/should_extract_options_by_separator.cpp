#include <iostream>
#include <vector>

#include "option.h"
#include "multiValue.h"

using namespace std;
using namespace command;

#define NAME "test"
#define VALUE "0,1,2,3,4,5,6,7,8,9"

#define OPTION NAME "=" VALUE

typedef int OptionType;

std::vector<OptionType> input;

void _function(OptionType value) {
    input.push_back(value);
    cout << "Catched value: " << value << "\n";
}

int main() {
    Parameter * option = new MultiValue(",", new Option<OptionType>(NAME, "Option as multiValue int", _function));

    try {
        if (option->understand(OPTION)) {
            option->handle();
        }
        else {
            cout << option->describe() << " should understand multiple int values\n";
            return 1;
        }
    }
    catch (...) {
        delete option;
        cout << option->describe() << " thrown unknown exception\n";
        return 1;
    }

    bool test = true;
    for (int i = 0; i < 10; i++) {
        test &= (input[i] == i);
        cout << i << ") input: " << input[i] << "\n";
    }

    if (test) {
        cout << option->describe() << " handles boolean (TRUE) values\n";
        delete option;
        return 0;
    }

    cout << option->describe() << " do not handle multiple int values\n";
    delete option;
    return 1;
}
