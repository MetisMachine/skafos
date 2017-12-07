#ifndef __COMMAND_EXCEPTION_OPTION_FAILED_CONVERSION_H
#define __COMMAND_EXCEPTION_OPTION_FAILED_CONVERSION_H

#include <stdexcept>
#include <string>

namespace command {

/**
 * Exception thrown used when Option's value failed conversion to specific type
 *
 * e.g.:
 *  "a" -> int
 */
class OptionFailedConversion : public std::invalid_argument {
public:
    /** \inheritdoc */
    explicit OptionFailedConversion(const std::string& what_arg) :
        std::invalid_argument(what_arg) { }

    /** \inheritdoc */
    explicit OptionFailedConversion(const char* what_arg) :
        std::invalid_argument(what_arg) { }
};

}

#endif /* __COMMAND_EXCEPTION_OPTION_FAILED_CONVERSION_H */
