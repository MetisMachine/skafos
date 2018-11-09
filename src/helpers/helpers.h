#ifndef __CLI_HELPERS__
#define __CLI_HELPERS__

#include "common.h"

const std::vector<int> SUCCESS_CODES = {200, 201, 202, 203, 204};

class Helpers {
public:
  static bool success(int status_code);
};

#endif
