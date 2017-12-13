#ifndef __SKAFOS_CLI_LOGS_H__
#define __SKAFOS_CLI_LOGS_H__

#include "common.h"

class Logs {
  public:
    static void print(std::string project, long num, bool tail);
};

#endif // !__SKAFOS__CLI_LOGS_H__
