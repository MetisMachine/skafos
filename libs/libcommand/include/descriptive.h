#ifndef __COMMAND_DESCRIPTIVE_H
#define __COMMAND_DESCRIPTIVE_H

#include <string>

namespace command {
    /**
     * Descriptive behaviour class.
     */
    class Descriptive {
        const std::string description;
    public:
        /**
         * Default constructor.
         *
         * @param description Description
         */
        Descriptive(const std::string& description)
            : description(description) {
        }

        virtual ~Descriptive() { }

        /**
         * Returns description of the current class.
         *
         * @return provided description for the class
         */
        const std::string & describe() {
            return description;
        }
    };
}

#endif /* __COMMAND_DESCRIPTIVE_H */
