#ifndef __COMMAND_EXCEPTION_MISSING_OPTION_VALUE_H
#define __COMMAND_EXCEPTION_MISSING_OPTION_VALUE_H

#include <stdexcept>
#include <string>

namespace command {

/**
 * Exception thrown used when Option should have value, but no one has been set
 */
class MissingOptionValue : public std::invalid_argument {
public:
    /** \inheritdoc */
    explicit MissingOptionValue(const std::string& what_arg) :
        std::invalid_argument(what_arg) { }

    /** \inheritdoc */
    explicit MissingOptionValue(const char* what_arg) :
        std::invalid_argument(what_arg) { }
};

}

#endif /* __COMMAND_EXCEPTION_MISSING_OPTION_VALUE_H */
