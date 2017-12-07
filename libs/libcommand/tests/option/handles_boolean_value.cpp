#include <iostream>

#include "option.h"

using namespace std;
using namespace command;

#define NAME "test"
#define FALSE "0"
#define TRUE "1"

#define OPTION1 NAME "=" FALSE
#define OPTION2 NAME "=" TRUE

typedef bool OptionType;

OptionType test;

void _function(OptionType value) {
    test = value;
}

int main() {
    Option<OptionType> option(NAME, "Option with boolean value", _function);

    if (option.understand(OPTION1)) {
        option.handle();
    }
    else {
        cout << option.describe() << " do not understand " << FALSE << " values\n";
        return 1;
    }

    if (test == (bool)std::stoi(FALSE)) {
        cout << option.describe() << " handles " << FALSE << " values\n";
    }

    Option<OptionType> option2(NAME, "Option with boolean value", _function);

    if (option2.understand(OPTION2)) {
        option2.handle();
    }
    else {
        cout << option.describe() << " do not understand " << TRUE << " values\n";
        return 1;
    }

    if (test == (bool)std::stoi(TRUE)) {
        cout << option.describe() << " handles " << TRUE << " values\n";
        return 0;
    }

    cout << "Option class do not handle boolean values\n" << test;

    return 1;
}
