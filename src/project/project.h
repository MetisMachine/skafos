#ifndef __CLI_PROJECT__
#define __CLI_PROJECT__

#include "common.h"
#include "templates/templates.h"

class Project {
public:
  static void init(std::string name, std::string tpl, bool master);
  static void kill(std::string project_token);
  static void kill(std::string jobs, std::string deployments);
  static void kill(std::string project_token, std::string jobs, std::string deployments);
  static void kill_message(json11::Json kill_json, std::string kill_type, std::string kill_details);
  static void remote_add(std::string project_token);
 
private:
  static bool exists(std::string directory);
};

#endif
