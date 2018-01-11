#ifndef __SKAFOS_CLI_LOGS_H__
#define __SKAFOS_CLI_LOGS_H__

#include "common.h"

class Logs {
public:
  static void print(std::string project, int num, bool tail);
};

#endif 
