#include <cstring>
#include <iostream>

#include "descriptive.h"

using namespace std;
using namespace command;

#define STRING "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"

int main() {
    Descriptive descriptive(STRING);

    int cmp = strcmp(descriptive.describe().c_str(), STRING);

    if (cmp == 0) {
        cout << "Descriptive class holds data correctly\n";
        return 0;
    }

    cout << "Descriptive class changes provided description\n";

    return 1;
}

