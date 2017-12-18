#ifndef __CLI_PROJECT__
#define __CLI_PROJECT__

#include "common.h"
#include "templates/templates.h"

class Project {
public:
  Project();
  
  void init();
private:
  void download_template(TemplateDetails details);
};

#endif
