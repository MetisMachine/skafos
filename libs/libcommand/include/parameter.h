#ifndef __COMMAND_PARAMETER_H
#define __COMMAND_PARAMETER_H

#include <string>
#include <sstream>

#include "descriptive.h"
#include "callable.h"

namespace command {
    /**
     * Base class for all the Arguments and Options.
     *
     * Example:
     *  ./myprog ARGUMENT
     */
    class Parameter : public Descriptive {
    protected:
        /** Variable indicating if current Parameter was already used or not */
        bool used = false;

    public:
        typedef class Parameter Type;
        /**
         * Default constructor.
         *
         * @param description Description of current Argument
         */
        Parameter(const std::string & description)
            : Descriptive(description) {
        }

        virtual ~Parameter() { }

        /**
         * Method used for handling method calls linked with this Parameter
         */
        virtual void handle() = 0;

        /**
         * Method used for checking if the given user value understandable for
         * parameter.
         *
         * @return true if passed value is understandable by current Parameter.
         *      False otherwise.
         */
        virtual bool understand(const std::string & ) = 0;

        /**
         * Indicates if current Parameter is required
         *
         * @return false, as all Parameters are non-required by default. If you
         *      want to make Parameter as required, wrap it using Required class
         */
        virtual bool isRequired() {
            return false;
        };

        /**
         * Indicates if current Parameter has been already used
         *
         * @return true if current Parameter has been already used. False otherwise.
         */
        virtual bool isUsed() {
            return used;
        }

        /**
         */
        virtual bool hungryForValue() {
            return false;
        }

        /**
         * @return position where value starts in passed string
         */
        virtual unsigned int valuePosition(const std::string & ) = 0;
    };
}

#endif /* __COMMAND_PARAMETER_H */
