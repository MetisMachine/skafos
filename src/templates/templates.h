#ifndef __CLI_TEMPLATES__
#define __CLI_TEMPLATES__

#include "common.h"

struct TemplateDetails {
  std::string name;
  std::string author;
  std::string description;
  std::string tags;
  std::string repo;
  std::string version;
};

class Template {
public:
  static void update();
  static void search(std::string name);

private:
  static int clone();
  static int pull();
  static TemplateDetails parse_template(std::string path);
};

#endif
