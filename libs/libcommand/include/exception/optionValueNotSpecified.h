#ifndef __COMMAND_EXCEPTION_OPTION_VALUE_NOT_SPECIFIED_H
#define __COMMAND_EXCEPTION_OPTION_VALUE_NOT_SPECIFIED_H

#include <stdexcept>
#include <string>

namespace command {

/**
 * Exception thrown when Option should have value, but no equal sign specifying
 * it has been set
 */
class OptionValueNotSpecified : public std::invalid_argument {
public:
    /** \inheritdoc */
    explicit OptionValueNotSpecified(const std::string& what_arg) :
        std::invalid_argument(what_arg) { }

    /** \inheritdoc */
    explicit OptionValueNotSpecified(const char* what_arg) :
        std::invalid_argument(what_arg) { }
};

}

#endif /* __COMMAND_EXCEPTION_OPTION_VALUE_NOT_SPECIFIED_H */
