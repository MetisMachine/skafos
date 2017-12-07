#include <cstring>
#include <iostream>

#include <functional> // std::bind, _1

#include "TestCallable.h"
#include "callable.h"

using namespace std;
using namespace command;

bool test = false;

class Class {
public:
    void method(void) {
        test = true;
    };
};

int main() {
    Class c;
    TestCallable<void> callable(std::bind(&Class::method, &c));
    callable.callFunction();

    if (test == true) {
        cout << "Callable class calls provided class member method\n";
        return 0;
    }

    cout << "Callable class does not call provided  member method\n";

    return 1;
}

