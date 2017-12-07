# Command

C++ library for handling command line arguments.

[![Build Status](https://travis-ci.org/quayle/command.svg?branch=master)](https://travis-ci.org/quayle/command)

## Installation

### debian package

    $ wget https://github.com/quayle/command/releases/download/v0.3-deb/command_0.3_all.deb
    $ sudo dpkg -i command_0.3_all.deb

### from sources

You will need to have autotools installed (automake, autoconf, ...)

    $ ./autogen.sh
    $ ./configure
    $ make
    $ sudo make install

## Configuration

You need to enable c++11 support in your compiler. You can achieve that in
g++ and clang++ by adding `-std=c++11` compilation flag.

As this is header-only library, you don't need any additional steps.

## Usage

example.cpp:

    #include <iostream>
    #include <string>
    #include <command/command.h>
    #include <command/option.h>
    #include <command/argument.h>

    using namespace command;

    class MyClass {
        std::string _value = "Default";
    public:
        void setValue(std::string value) {
            this->_value = value;
        }
        std::string getValue() {
            return std::string("Value from MyClass: ") + this->_value;
        }
    };

    int main(int argc, char *argv[]) {
        MyClass myClass;
        try {
            Command command(argc, argv, {
                new Option<void>("-h", "Help", [](void) {
                    std::cout << "Help information\n";
                }),
                new Argument<std::string>("Value for MyClass",
                    std::bind(&MyClass::setValue, &myClass, std::placeholders::_1)
                )
            });
        }
        catch(const std::exception & e) {
            return 1;
        }

        std::cout << myClass.getValue() << std::endl;

        return 0;
    }

Now program can be compiled & run using following commands:

    $ g++ -std=c++11 example.cpp
    $ ./a.out -h
    Help information
    Value from MyClass: Default

    $ ./a.out someArg
    Value from MyClass: someArg

    $ ./a.out someArg -h
    Help information
    Value from MyClass: someArg

### Possible classes to use:

Arguments are non-named program parameters. They must have some description and
function handling when argument is passed:

    new Argument<bool>("Bool argument", [](bool value) { });

Options are named program parameters. Option need name (e.g.: "i"), description,
and function.

    new Option<int>("name", "Integer option", [](int value) { });

Options could also be set as containing no value. In that case they become just
a simple switches (some kind of mix between Argument and Option). They are used
just to invoke some function if specific name was passed:

    new Option<void>("v", "Verbose mode of program", [](void) { });

### Behaviours

Parameters (Options and Arguments) can also be wrapped in Behaviours.

Required behaviour - if specific parameter was not passed and is required,
exception is thrown (missingRequiredParameter):

    new Required(
        new Argument<bool>("Bool argument", [](bool value) { })
    );

MultiValue behaviour - given parameter can handle more than one value. Values are
separated by given separator. For each value passed function is invoked:

    new MultiValue(",",
        new Option<std::string>("input", "Input file", [](std::string value) { })
    )

More complex example:

    (...)
    void argument_function(bool a) {
        std::cout << "Argument: " << a << std::endl;
    }

    void option_function(std::string a) {
        std::cout << "Option function " << a << std::endl;
    }

    void void_function(void) {
        std::cout << "Void function " << std::endl;
    }

    Command command(argc, argv, {
        new Required(
            new MultiValue("-",
                new Argument<bool>("Input values", argument_function)
            )
        ),
        new MultiValue(",",
            new Option<std::string>("f", "Optional file", option_function)
        )
        new Option<void>("h", "Help", void_function)
    });
    (...)

Above code allows us to:

Parameters wrapped in Required class, have validator which checks if argument is
passed:

    $ ./a.out
    *Input values* is required

    $ ./a.out 1
    Argument: 1

    $ ./a.out 0
    Argument: 0

    $ ./command 0 f
    Option: f requires value to be specified after equal sign, but no equal sign was found

    $ ./command 0 f=
    Option: f failed value conversion to the required type

For MultiValue Parameters each value is passed to the given function:

    $ ./a.out 1-0-1
    Argument: 1
    Argument: 0
    Argument: 1

    $ ./command 0 f=one,two,three
    Argument: 0
    Option function one
    Option function two
    Option function three

## Documentation

Current documentation can be found at:
http://dlugolecki.net.pl/software/command/docs/

If for some reason it is unavailable, you can build it yourself. The only
requirement is to have [Doxygen](http://www.doxygen.org/) installed when `make` command is invoked.
