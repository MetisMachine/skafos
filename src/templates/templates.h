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
  static void create_cache_dir();  
  static void search(std::string name);
  static TemplateDetails find(std::string name);
  static void download(TemplateDetails details, std::string version);  
private:
  static std::list<TemplateDetails> all();
  static int clone();
  static int pull();
  static TemplateDetails parse_template(std::string path);
};

#endif
