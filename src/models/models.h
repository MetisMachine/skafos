#ifndef __CLI_MODELS__
#define __CLI_MODELS__

#include "common.h"
#include "project_env/project_env.h"
#include "request/request.h"
#include "env/env.h"
#include "file/file.h"
#include <curl/curl.h>
#include "yaml-cpp/yaml.h"

using namespace std;
using namespace json11;

class Models {
  public:
    static void list(std::string project_token, std::map<string, string> params);
    static void download(std::string project_token, std::map<string,string> params, std::string output_path);

  private:
    static void download_to_file(std::string project_token, std::string display_name, std::string version, std::string output_path);
    static string resolve_download_path(std::string output_path, std::string output_file);
    static string tags_to_string(Json model_tags);
};

#endif