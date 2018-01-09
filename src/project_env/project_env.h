#ifndef __CLI_PROJECT_VAR_H__
#define __CLI_PROJECT_VAR_H__

#include "common.h"

#define PROJECT_TOKEN  ProjectEnv::current().token

struct ProjectDetails {
  std::string token;
  std::string name;
  std::string entrypoint;
};

class ProjectEnv {
public:
  static ProjectDetails current();

private:
  static ProjectEnv *instance();
  static ProjectEnv *instance_;
  ProjectEnv();

  ProjectDetails _current();
  ProjectDetails parse_project(std::string path);
};

#endif
