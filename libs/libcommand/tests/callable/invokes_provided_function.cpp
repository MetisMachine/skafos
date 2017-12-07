#include <cstring>
#include <iostream>

#include "TestCallable.h"
#include "callable.h"

using namespace std;
using namespace command;

bool test = false;

void _function(bool val) {
    test = val;
};

int main() {
    TestCallable<bool> callable(_function);
    callable.callFunction(true);

    if (test == true) {
        cout << "Callable class calls provided function\n";
        return 0;
    }

    cout << "Callable class does not call provided function\n";

    return 1;
}

