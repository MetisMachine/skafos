#ifndef __CLI_ENV_VAR_H__
#define __CLI_ENV_VAR_H__

#include "common.h"

class EnvVar {
public:
  static void list();
  static void get(std::string key);
  static void set(std::string key, std::string value);
  static void del(std::string key);

private:
  static void load_project();
};

#endif
