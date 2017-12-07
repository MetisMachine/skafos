#ifndef __COMMAND_COMMAND_H
#define __COMMAND_COMMAND_H

#include <string>
#include <vector>
#include <typeinfo>
#include <iostream>

#include "parameter.h"
#include "grouped.h"

namespace command {
    /**
     * Main class for handling user passed parameters from command line.
     */
    class Command : protected Grouped {
    public:
        /**
         * Default constructor.
         *
         * @param argc from the main function
         * @param argv from the main function
         * @param params initializer_list containing Parameter handlers
         *      responsible for correctly handle user data.
         */
        Command(unsigned int argc, char *argv[], std::initializer_list<Parameter *> params)
            : Grouped(params, "Command") {
            try {
                for (unsigned int i = 1; i < argc; i++) {
                    this->understand(argv[i]);
                }
                handle();
            }
            catch(const std::invalid_argument & exception) {
                releaseMemory();
                throw;
            }
            catch(const std::logic_error & exception) {
                releaseMemory();
                throw;
            }
        }

        /**
         * Destructor. Releases allocated memory.
         */
        ~Command() {
            releaseMemory();
        }
    };
}

#endif /* __COMMAND_COMMAND_H */
