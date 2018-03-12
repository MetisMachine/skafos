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
  ProjectJob prj_job_info;
  std::vector<ProjectJob> jobs;
  std::string name = "";
  std::string entrypoint = "";
  std::string job_id = "";
  std::string token = "";

  YAML::Node prj;
  
  try {
    prj = YAML::LoadFile(path);
  } catch(...) {
    console::error("Unable to find metis.config.yml, are you in a project directory?");
    exit(EXIT_FAILURE);
  }

  if (prj["project_jobs"]){
    console::debug("Using multi task (new) config file"); 

    for (int j = 0; prj["project_jobs"][j] ; j++) {
      name       = prj["project_jobs"][j]["name"].as<string>();
      entrypoint = prj["project_jobs"][j]["entrypoint"].as<string>();
      job_id    = prj["project_jobs"][j]["job_id"].as<string>();
      
      prj_job_info.name = name;
      prj_job_info.entrypoint = entrypoint;
      prj_job_info.job_id = job_id;
      jobs.push_back(prj_job_info);  
    }

    token = prj["project_token"].as<string>();
    
  } else {
    console::debug("Using single task (old) config file"); 

    token      = prj["project_token"].as<string>();
    name       = prj["name"].as<string>();
    entrypoint = prj["entrypoint"].as<string>();

    prj_job_info.name = name;
    prj_job_info.entrypoint = entrypoint;
    prj_job_info.job_id = job_id;
    jobs.push_back(prj_job_info);
  
  }

  details.token         = token;
  details.jobs          = jobs; 

  return details;
}
