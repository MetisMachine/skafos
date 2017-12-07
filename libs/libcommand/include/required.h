#ifndef __COMMAND_REQUIRED_H
#define __COMMAND_REQUIRED_H

#include "parameter.h"

namespace command {
    /**
     * Required Parameter decorator. Makes passed Parameters treated as required
     */
    class Required : public Parameter {
    protected:
        /**
         * Parameter which will be treated as required
         */
        Parameter * parameter;

    public:
        /**
         * Default constructor.
         *
         * @param parameter Parameter which will be treated as required
         */
        Required(Parameter * parameter)
            : Parameter(parameter->describe()), parameter(parameter) {
        }

        /**
         * Default destructor. Releases allocated memory
         */
        virtual ~Required() {
            delete parameter;
        }

        /**
         * Wrapper method around passed Parameter::handle().
         *
         * \inheritdoc
         */
        virtual void handle() {
            parameter->handle();
        }

        /**
         * Wrapper method around passed Parameter::understand()
         *
         * @param argv command line value against which test will be made
         *
         * \inheritdoc
         */
        virtual bool understand(const std::string & value) {
            return parameter->understand(value);
        }

        /**
         * Indicates if current Parameter is required
         *
         * @return true, as all Parameters wrapped in Required class are set as
         *      required. In order to make them non-required do not use
         *      Required class
         */
        virtual bool isRequired() {
            return true;
        };

        /**
         * Wrapper method around passed Parameter::isUsed().
         *
         * \inheritdoc
         */
        virtual bool isUsed() {
            return parameter->isUsed();
        }

        /**
         * Wrapper method around passed Parameter::valuePosition().
         *
         * \inheritdoc
         */
        virtual unsigned int valuePosition(const std::string & value) {
            return parameter->valuePosition(value);
        }
    };
}

#endif /* __COMMAND_REQUIRED_H */
