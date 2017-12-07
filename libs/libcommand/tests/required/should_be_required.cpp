#include <iostream>

#include "required.h"
#include "argument.h"
#include "option.h"

using namespace std;
using namespace command;

#define NAME "test"
#define VALUE "1"

#define OPTION NAME "=" VALUE

typedef int Type;

void _function(Type val) { }

int main() {
    Parameter * requiredOption = new Required(new Option<Type>(NAME, "Required Option", _function));
    Parameter * requiredArgument = new Required(new Argument<Type>("Required Argument", _function));

    if (!requiredOption->isRequired()) {
        cout << requiredOption->describe() << " should be treated as required but it is not\n";
        delete requiredOption;
        return 1;
    }
    if (!requiredArgument->isRequired()) {
        cout << requiredArgument->describe() << " should be treated as required but it is not\n";
        delete requiredOption;
        return 1;
    }

    cout << "Required decorator correctly returns its property\n";

    return 0;
}
