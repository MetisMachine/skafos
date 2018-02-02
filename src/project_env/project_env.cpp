#include "project_env.h"
#include "file/file.h"
#include "yaml-cpp/yaml.h"

using namespace std;
using namespace json11;


ProjectDetails ProjectEnv::current() {
  return instance()->_current();
}

#pragma mark -  Private
ProjectEnv *ProjectEnv::instance_ = 0;

ProjectEnv *ProjectEnv::instance() {
  if (instance_ == 0) {
    instance_ = new ProjectEnv();
  }

  return instance_;
}

ProjectEnv::ProjectEnv() {}

ProjectDetails ProjectEnv::_current() {
  string path = FileManager::cwd() + "/metis.config.yml";

  return parse_project(path);
}

ProjectDetails ProjectEnv::parse_project(string path) {
  ProjectDetails details;
  ProjectTask prj_task_info;
  std::vector<ProjectTask> prj_tasks;
  std::string name = "";
  std::string entrypoint = "";
  std::string task_id = "";
  std::string token = "";

  YAML::Node prj;
  
  try {
    prj = YAML::LoadFile(path);
  } catch(...) {
    console::error("Unable to find metis.config.yml, are you in a project directory?");
    exit(EXIT_FAILURE);
  }

  if (prj["project_tasks"]){
    console::debug("Using multi task (new) config file"); 

    for (int j = 0; prj["project_tasks"][j] ; j++) {
      name       = prj["project_tasks"][j]["name"].as<string>();
      entrypoint = prj["project_tasks"][j]["entrypoint"].as<string>();
      task_id    = prj["project_tasks"][j]["project_task_id"].as<string>();
      
      prj_task_info.name = name;
      prj_task_info.entrypoint = entrypoint;
      prj_task_info.task_id = task_id;
      prj_tasks.push_back(prj_task_info);  
    }

    token = prj["project_token"].as<string>();
    
  } else {
    console::debug("Using single task (old) config file"); 

    token      = prj["project_token"].as<string>();
    name       = prj["name"].as<string>();
    entrypoint = prj["entrypoint"].as<string>();

    prj_task_info.name = name;
    prj_task_info.entrypoint = entrypoint;
    prj_task_info.task_id = task_id;
    prj_tasks.push_back(prj_task_info);
  
  }

  return details;
}
