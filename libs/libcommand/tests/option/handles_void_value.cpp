#include <iostream>

#include "option.h"

using namespace std;
using namespace command;

#define NAME "test"

typedef void OptionType;

bool test = false;

void _function() {
    test = true;
}

int main() {
    Option<OptionType> option(NAME, "Option with void value", _function);

    if (option.understand(NAME)) {
        option.handle();
    }
    else {
        cout << option.describe() << " do not understand void values\n";
        return 1;
    }

    if (test) {
        cout << option.describe() << " handles void values\n";
        return 0;
    }

    cout << option.describe() << " do not handle void values\n" << endl;

    return 1;
}
