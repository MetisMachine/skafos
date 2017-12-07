#ifndef __COMMAND_GROUPED_H
#define __COMMAND_GROUPED_H

#include <iostream>
#include <vector>
#include <string>

#include "parameter.h"
#include "exception/missingRequiredParameter.h"

namespace command {
    /**
     * Grouped Parameters decorator. Allows Parameters to understand be grouped
     * together.
     *
     * Example usage:
     *  - ./myprog [ARGUMENT OPTION] [ARGUMENT OPTION] ...
     */
    class Grouped : public Parameter {
    protected:
        /**
         * Parameters which will be treated as grouped together
         */
        std::vector<Parameter *> parameters;

        std::vector<std::string> values;

        std::vector<bool> understood;
        std::vector<bool> all_used;

    public:
        /**
         * Default constructor.
         *
         * @param params Parameters which will be treated as grouped together
         */
        Grouped(std::initializer_list<Parameter *> params, std::string description = "Grouped")
            : Parameter(description), parameters(params) {
            for (unsigned int i = 0; i < parameters.size(); i++) {
                understood.push_back(false);
                values.push_back("");
            }
        }

        /**
         * Default destructor. Releases allocated memory
         */
        virtual ~Grouped() {
            releaseMemory();
        }

        /**
         * Wrapper method around passed Parameter::handle().
         *
         * \inheritdoc
         */
        virtual void handle() {
//                 std::cout << this->describe() << " handles" << "\n";
//             if (!isUsed()) {
                for (unsigned int i = 0; i < parameters.size(); i++) {
                    Parameter *param = parameters[i];
                    if (understood[i] ||  param->hungryForValue()) {
//                     std::cout << param->describe() << "=" << values[i] << "\n";
                        param->handle();
                    }
                }
//             }
            // @TODO: Nested required
            for(Parameter *param : parameters) {
                if (param->isRequired() && !param->isUsed()) {
                    throw MissingRequiredParameter(param->describe() + " is required");
                }
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
//             std::cout << "[+] Grouped [" << this->describe() << "]\n";

            bool _understand = false;

//             std::cout << "GMD\n" ;
            for (unsigned int i = 0; i < parameters.size(); i++) {
                Parameter *param = parameters[i];

//                 std::cout << " > Grouped:Param [" << param->describe() << ", " << value << "]\n";
                if (!understood[i]) {
                    _understand = param->understand(value);
                    if (_understand) {
//                         std::cout << " > Understood [" << param->describe() << ", " << value << "]\n";
                        understood[i] = _understand;
                        values[i] = value;
                        break;
                    }
                }
            }

            bool all_understood = (understood.size() == parameters.size());
            for (bool u : understood) {
                all_understood &= u;
            }

//             std::cout << this->describe() << " understand: " << std::boolalpha << all_understood << "\n";

            return all_understood;
        }

        /**
         * Indicates if current Parameter has been already used
         *
         * @return true if current Parameter has been already used. False otherwise.
         */
//         virtual bool isUsed() {
//             if (!used) {
// //                 bool used 
//                 for(Parameter *param : parameters) {
//                     param->isUsed();
//                 }
//             }
//         }

        /**
         * \inheritdoc
         */
        virtual bool hungryForValue() {
            return true;
        }

        /**
         * Wrapper method around passed Parameter::valuePosition().
         *
         * \inheritdoc
         */
        virtual unsigned int valuePosition(const std::string &) {
            throw std::logic_error("Group does not have value part");
        }

        /**
         * Releases acquired memory
         */
        void releaseMemory() {
            for (Parameter * parameter : parameters) {
                if (parameter != NULL) {
                    delete parameter;
                }
            }
            parameters.clear();
            parameters.shrink_to_fit();
        }
    };
}

#endif /* __COMMAND_GROUPED_H */
