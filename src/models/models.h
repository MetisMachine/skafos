#ifndef __CLI_MODELS__
#define __CLI_MODELS__

#include "common.h"
#include "project_env/project_env.h"
#include "request/request.h"

using namespace std;
using namespace json11;

class Models {
  public:
    static void list(std::string project_token, std::map<string, string> params);
};

#endif