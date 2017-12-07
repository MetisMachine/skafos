#include <cstring>
#include <iostream>

#include "TestParameter.h"

using namespace std;
using namespace command;

#define DESC "Simple parameter"

int main() {
    TestParameter parameter(DESC);

    if (parameter.isRequired()) {
        cout << parameter.describe() << " is required, but it should not\n";
        return 1;
    }

    cout << parameter.describe() << " is not required by default\n";

    return 0;
}

