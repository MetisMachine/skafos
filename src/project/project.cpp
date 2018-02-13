#include "project.h"
#include "templates/templates.h"
#include "file/file.h"
#include "env/env.h"
#include "request/request.h"
#include "jinja/jinja.h"
#include "jinja/stringhelper.h"
#include "yaml-cpp/yaml.h"

using namespace std;
using namespace json11;

void Project::init(string name, string tpl) {
  string directory = (name == "")? 
    FileManager::cwd() : (name == ".") ? 
      FileManager::resolve_path(name) : 
      FileManager::cwd() + "/" + name;

  string project_name = directory.substr(directory.find_last_of("/"));
    
  if(exists(directory)) {
    console::error("Not an empty directory, we don't want to overwrite any current projects");
    exit(EXIT_FAILURE);
  }

  FileManager::create_path(0755, directory);

  TemplateDetails tpl_details = Template::find(tpl);
  if(tpl_details.repo.length() < 1) {
    console::error("Unable to find template: " + tpl);

    exit(EXIT_FAILURE);
  }

  string version = "master"; //(tpl_details.version.length() > 0)? tpl_details.version : "master";

  Template::download(tpl_details, version);
  
  console::info("Setting up template in project...");

  string cache_path = ENV_PATHS.cache + "/" + tpl + ".zip";
 
  FileManager::unzip(cache_path, directory);

  console::info("Creating configuration file");

  string template_path            = directory + "/metis.config.yml";
  Jinja::Template config_template = FileManager::read(template_path);
  
  string err;
  string proj   = replace(project_name, "/", "");
  Json json     = Json::parse(Request::create_project(proj).body, err);
  string token  = json["token"].string_value();
  string project_task_id = json["project_tasks"][0]["id"].string_value();
  string project_task_name = json["project_tasks"][0]["name"].string_value();

  config_template.setValue("token", token);
  config_template.setValue("name", proj);
  config_template.setValue("project_task_id", project_task_id);
  config_template.setValue("project_task_name", project_task_name);

  FileManager::write(template_path, config_template.render());
}

void Project::kill(string kill_id, string task_type){
  string directory = (kill_id == "")? 
    FileManager::cwd() : (kill_id == ".") ? 
      FileManager::resolve_path(kill_id) : 
      FileManager::cwd() + "/" + kill_id;

  if(task_type == "."){
    YAML::Node config;
    string tpl_path = directory + "/metis.config.yml";
    try {
      config = YAML::LoadFile(tpl_path);
    } catch(...) {
      console::error("Unable to find metis.config.yml, are you in a project directory?");
      exit(EXIT_FAILURE);
    }
    kill_id = config["token"].as<string>();
  }
  string err;
  Json json = Json::parse(Request::kill_task(kill_id, task_type).body, err);
  bool kill_message_sent = json["kill_message_sent"].bool_value();
  if (kill_id.compare("all") == 0){
      kill_id = "project";
    }

  if (kill_message_sent){
    console::success("Successfully added the " + kill_id + " to the kill task queue.");
  } else{
    console::error("Unable to add the " + kill_id + " to the kill task queue.");
  }
}

bool Project::exists(std::string directory) {
  if(FileManager::dir_exists(directory)) {
      list<string>files = FileManager::dir_list(directory);

      if(files.size() > 0) {
        return true;
      }
  }

  return false;
}
