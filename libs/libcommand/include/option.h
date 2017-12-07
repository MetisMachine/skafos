#ifndef __COMMAND_OPTION_H
#define __COMMAND_OPTION_H

#include <string>
#include <sstream>
#include <stdexcept>

#include "parameter.h"
#include "exception/missingOptionValue.h"
#include "exception/optionFailedConversion.h"
#include "exception/optionValueNotSpecified.h"

namespace command {
    /**
     * Class responsible for handling commandline options.
     * Options are named parameters of program.
     *
     * Example:
     *  - ./myprog OptionName=OptionValue
     *  - ./myprog -f=/some/file
     *  - ./myprog --level=15
     */
    template<typename ParameterType>
    class Option
        : public Parameter, public Callable<ParameterType>  {
    public:
        typedef std::string OptionName;
    protected:
        /**
         * Current Option name
         */
        const OptionName name;

        /**
         * Current Option value
         */
        ParameterType value;

    public:
        /**
         * Default constructor.
         *
         * @param name Name of the current Option
         * @param description Description of current Option
         * @param function Function used to handle current Option.
         */
        Option(const std::string & name, const std::string & description, std::function<void(ParameterType)> function)
            : Parameter(description), Callable<ParameterType>(function), name(name) {
        }

        /**
         *
         */
        virtual ~Option() { }

        /**
         * \inheritdoc
         */
        virtual void handle() {
            this->call(value);
            used = true;
        }

        /**
         * Method used for checking if Option understands given user value.
         * If so current Option is flagged as used and no more checks against
         * it will be done in future.
         *
         * Passed value should be in form of:
         *      OptionName=OptionValue
         *
         * If no equal sign is after OptionName part,
         * std::invalid_argument exception with appropriate message is thrown
         *
         * If conversion of OptionValue part to ParameterType failed,
         * std::invalid_argument exception with appropriate message is thrown
         *
         * @param argv command line value against which test will be made.
         *  User value should be in format: OptionName=OptionValue.
         *
         * @return If passed argv succesfully detected OptionName part as a
         *  current option and its OptionValue part has been succesfully
         *  converted to ParameterType, returns true and Option is set as used one.
         *  Otherwise returns false and can be used to check against next value.
         *
         * @throw MissingOptionValue when OptionValue part is missing after
         *  equal sign
         * @throw OptionFailedConversion when OptionValue part failed conversion
         *  to ParameterType
         */
        virtual bool understand(const std::string & argv) {
            if (this->hasName(argv)) {
                std::size_t pos = this->valuePosition(argv);

                if (pos != name.size()) {
                    throw MissingOptionValue("Option: " + name + " requires value but no one has been provided");
                }

                std::stringstream ss;
                ss << std::fixed << argv.substr(pos + 1);
                ss >> value;

                if (ss.fail()) {
                    throw OptionFailedConversion("Option: " + name + " failed value conversion to the required type");
                }

                return true;
            }
            return false;
        }

        /**
         * \inheritdoc
         */
        virtual unsigned int valuePosition(const std::string & value) {
            std::size_t pos = value.find("=");

            if ((this->hasName(value)) && (pos == std::string::npos)) {
                throw OptionValueNotSpecified("Option: " + name + " requires value to be specified after equal sign, but no equal sign was found");
            }

            return pos;
        }

    protected:
        bool hasName(const std::string & argv) {
            return argv.find(name) == 0;
        }
    };

    /**
     * Template class responsible for handling commandline options.
     * Options are non-required, named parameters of program.
     * This template specialization allows Options to work like switches.
     * It means that just named parameter is needed to invoke command. No value
     * is used.
     *
     * Example:
     *  ./myprog OptionName
     *  ./myprog -h
     *  ./myprog --help
     */
    template<>
    class Option<void>
        : public Parameter, public Callable<void>  {
    public:
        typedef std::string OptionName;
    protected:
        /**
         * Current Option name
         */
        const OptionName name;
    public:
        /**
         * Default constructor.
         *
         * @param name Name of the current Option
         * @param description Description of current Option
         * @param function Function used to handle current Option.
         */
        Option(const std::string & name, const std::string & description, std::function<void(void)> function)
            : Parameter(description), Callable<void>(function), name(name) {
        }

        /**
         *
         */
        virtual void handle() {
            this->call();
            used = true;
        }

        /**
         * Method used for checking if Option understands given user value.
         * If so, current Option is flagged as used and no more checks against
         * it will be done in future.
         *
         * Passed value should be in form of:
         *      OptionName
         *
         * @param argv command line value against which test will be made.
         *  User value should be in format: OptionName.
         *
         * @return If passed argv succesfully detected OptionName returns true
         *  and Option is set as used one. Otherwise returns false and can be
         *  used to check against next value.
         */
        virtual bool understand(const std::string & argv) {
            if (argv == name) {
                return true;
            }
            return false;
        }

        /**
         * \inheritdoc
         */
        virtual unsigned int valuePosition(const std::string & ) {
            throw new std::invalid_argument(this->describe() + " is void Option, so it does not have value part");
        }
    };
}

#endif /* __COMMAND_OPTION_H */
