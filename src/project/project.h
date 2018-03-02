#ifndef __CLI_PROJECT__
#define __CLI_PROJECT__

#include "common.h"
#include "templates/templates.h"

class Project {
public:
  static void init(std::string name, std::string tpl, bool master);
  static void kill(std::string project_token);
  static void kill(std::string project_tasks, std::string tasks);
  static void kill(std::string project_token, std::string project_tasks, std::string tasks);
 
private:
  static bool exists(std::string directory);
};

#endif
