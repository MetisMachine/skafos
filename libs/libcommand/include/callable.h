#ifndef __COMMAND_CALLABLE_H
#define __COMMAND_CALLABLE_H

#include <string>
#include <functional>

namespace command {
    /**
     * Callable behaviour class.
     */
    template<typename ParameterType>
    class Callable {
    protected:
        /**
         * Function handling user Arguments
         */
        std::function<void(ParameterType)> func;

    public:
        /**
         * Default constructor.
         *
         * @param function Function that will be invoked
         */
        Callable(std::function<void(ParameterType)> function)
            : func(function) {
        }

        virtual ~Callable() { }

    protected:
        /**
         * Executes command binded with argument
         *
         * @param value Value passed to program argument
         */
        void call(ParameterType value) {
            this->func(value);
        }
    };

    /**
     * Template specialization of Callable behaviour class.
     * Allows passing functions with void argument
     */
    template<>
    class Callable<void> {
    protected:
        /**
         * Function handling user Arguments
         */
        std::function<void(void)> func;

    public:
        /**
         * Default constructor.
         *
         * @param function Function that will be invoked
         */
        Callable(std::function<void(void)> function)
            : func(function) {
        }

        virtual ~Callable() { }

    protected:
        /**
         * Executes command
         */
        void call() {
            this->func();
        }
    };
}

#endif /* __COMMAND_DESCRIPTIVE_H */
