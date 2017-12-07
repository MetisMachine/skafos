#include "callable.h"

using namespace command;

template<typename ArgumentType>
class TestCallable : public Callable<ArgumentType> {
public:
    TestCallable(void (*function)(ArgumentType))
        : Callable<ArgumentType>(function) {
    }

    TestCallable(std::function<void(ArgumentType)> function)
        : Callable<ArgumentType>(function) {
    }

    void callFunction(ArgumentType test) {
        this->call(test);
    }
};

template<>
class TestCallable<void> : public Callable<void> {
public:
    TestCallable(void (*function)(void))
        : Callable<void>(function) {
    }

    TestCallable(std::function<void(void)> function)
        : Callable<void>(function) {
    }

    void callFunction() {
        this->call();
    }
};
