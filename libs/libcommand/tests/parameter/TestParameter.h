#include "parameter.h"


class TestParameter : public command::Parameter {
public:
    TestParameter(const std::string & description) : Parameter(description) { }

    virtual void handle() { }
    virtual bool understand(const std::string & ) { return false; }

    virtual unsigned int valuePosition(const std::string & value) {
        return 0;
    }
};