#ifndef __COMMAND_ARGUMENT_H
#define __COMMAND_ARGUMENT_H

#include <string>
#include <sstream>
#include <iostream>
#include <functional>

#include "parameter.h"
#include "callable.h"

namespace command {
    /**
     * Class responsible for handling commandline arguments.
     * Arguments are non-named parameters of program.
     *
     * Example:
     *  - ./myprog ARGUMENT
     *  - ./myprog /path/to/file
     *  - ./myprog "some argument"
     */
    template<typename ParameterType>
    class Argument : public Parameter, public Callable<ParameterType> {
    protected:
        ParameterType value;
    public:
        typedef class Argument Type;

        /**
         * Default constructor.
         *
         * @param description Description of current Argument
         * @param function Function used to handle current Argument.
         */
        Argument(const std::string & description, std::function<void(ParameterType)> function)
            : Parameter(description), Callable<ParameterType>(function) {
        }

        /**
         *
         */
        virtual ~Argument() { }

        /**
         * \inheritdoc
         */
        virtual void handle() {
            this->call(value);
            this->used = true;
        }

        /**
         * Method used for checking if Argument understands user value.
         * If so current Argument is flagged as used and no more checks against
         * it will be done in future.
         *
         * \attention If conversion from passed value to ParameterType is
         * impossible, it is ignored. It means that it is not understanded by
         * Argument.
         *
         * @param argv command line value against which test will be made.
         *
         * @return If passed argv is succesfully converted to ParameterType,
         *  returns true and Argument is set as used one. If there was an error
         *  during conversion, method returns false and can be used to check
         *  against next value.
         */
        virtual bool understand(const std::string & argv) {
            std::stringstream ss;

            ss << std::fixed << argv;
            ss >> value;

            if (!ss.fail()) {
                return true;
            }

            return false;
        }

        /**
         * \inheritdoc
         */
        virtual unsigned int valuePosition(const std::string & ) {
            return 0;
        }
    };
}

#endif /* __COMMAND_ARGUMENT_H */
