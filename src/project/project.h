#ifndef __CLI_PROJECT__
#define __CLI_PROJECT__

#include "common.h"
#include "templates/templates.h"

class Project {
public:
  static void init(std::string name, std::string tpl);
  static void kill(std::string kill_id, std::string task_type);

private:
  static bool exists(std::string directory);
};

#endif
