#ifndef __CLI_TEMPLATES__
#define __CLI_TEMPLATES__

#include "common.h"

class Template {
public:
  static void update();
  static void search(std::string name);

private:
  static int clone();
  static int pull();
};

#endif
