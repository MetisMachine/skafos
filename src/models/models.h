#ifndef __CLI_MODELS__
#define __CLI_MODELS__

#include "common.h"
#include "project_env/project_env.h"
#include "request/request.h"
#include "env/env.h"
#include "file/file.h"
#include <curl/curl.h>

using namespace std;
using namespace json11;

class Models {
  public:
    static void list(std::string project_token, std::map<string, string> params);
    static void download(std::string project_token, std::map<string,string> params, std::string output_path);

  private:
    static string tags_to_string(Json model_tags);
};

#endif