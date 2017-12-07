#ifndef __COMMAND_MULTIVALUE_H
#define __COMMAND_MULTIVALUE_H

#include <iostream>
#include <vector>

#include "parameter.h"

namespace command {
    /**
     * Multiple Value Parameter decorator. Allows Parameters to understand
     * many values.
     *
     * Example:
     *  - separator: ","
     *  - multiValue: "0,1,2,3,4,5,6,7,8,9"
     *
     * Example usage:
     *  - ./myprog ARGUMENT,ARGUMENT,ARGUMENT,ARGUMENT
     *  - ./myprog OPTION_NAME=VALUE,VALUE,VALUE,VALUE
     */
    class MultiValue : public Parameter {
    protected:
        /**
         * Parameter which will be treated as containing multiple values
         */
        Parameter * parameter;

        std::vector<std::string> values;

        std::string separator;

    public:
        /**
         * Default constructor.
         *
         * @param parameter Parameter which will be treated as containing multiple values
         */
        MultiValue(std::string separator, Parameter * parameter)
            : Parameter(parameter->describe()), parameter(parameter),
                separator(separator) {
        }

        /**
         * Default destructor. Releases allocated memory
         */
        virtual ~MultiValue() {
            delete parameter;
        }

        /**
         * Wrapper method around passed Parameter::handle().
         *
         * \inheritdoc
         */
        virtual void handle() {
            for (std::string value : values) {
                parameter->understand(value);
                parameter->handle();
            }
        }

        /**
         * Wrapper method around passed Parameter::understand()
         *
         * @param argv command line value against which test will be made
         *
         * \inheritdoc
         */
        virtual bool understand(const std::string & value) {
            size_t start = 0;
            size_t pos = 0;
            bool _understand = true;
            std::string prefix = "";

            start = parameter->valuePosition(value);

            if (start > value.size()) {
                return false;
            }

            if (start > 0) {
                prefix = value.substr(0, ++start);// always count: "="
            }

            do {
                pos = value.find(separator, start);
                values.push_back(prefix + value.substr(start, pos-start));
                _understand &= parameter->understand(values.back());
                start = pos + 1;

                if (!_understand) {
                    values.clear();
                    break;
                }
            } while ((pos != std::string::npos) && (start < value.size()));
            return _understand;
        }

        /**
         * Wrapper method around passed Parameter::isRequired().
         * Indicates if current Parameter is required.
         *
         * @return true if Parameter is required, false otherwise
         */
        virtual bool isRequired() {
            return parameter->isRequired();
        };

        /**
         * Wrapper method around passed Parameter::isUsed().
         *
         * \inheritdoc
         */
        virtual bool isUsed() {
            return parameter->isUsed();
        };

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

#endif /* __COMMAND_MULTIVALUE_H */
