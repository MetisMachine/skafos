#ifndef __COMMAND_EXCEPTION_MISSING_REQUIRED_PARAMETER_H
#define __COMMAND_EXCEPTION_MISSING_REQUIRED_PARAMETER_H

#include <stdexcept>
#include <string>

namespace command {

/**
 * Exception thrown when required argument was not set
 */
class MissingRequiredParameter : public std::logic_error {
public:
    /** \inheritdoc */
    explicit MissingRequiredParameter(const std::string& what_arg) :
        std::logic_error(what_arg) { }

    /** \inheritdoc */
    explicit MissingRequiredParameter(const char* what_arg) :
        std::logic_error(what_arg) { }
};

}

#endif /* __COMMAND_EXCEPTION_MISSING_REQUIRED_PARAMETER_H */
