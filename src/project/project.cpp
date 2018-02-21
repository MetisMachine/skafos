#include "project.h"
#include "templates/templates.h"
#include "file/file.h"
#include "env/env.h"
#include "request/request.h"
#include "jinja/jinja.h"
#include "jinja/stringhelper.h"
#include "yaml-cpp/yaml.h"
#include "project_env/project_env.h"

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

void Project::kill(string project_token){
  if(project_token.compare(".") == 0){
    project_token = PROJECT_TOKEN;
  }

  string err;
  Json json = Json::parse(Request::kill_project(project_token).body, err);
  string kill_message_sent = json["ok"]["intent"].string_value();
  if (kill_message_sent.compare("") != 0){
    console::success("Successfully added the project with token " + project_token + " to the kill task queue.");
  } else{
    string err_message = json["error"].string_value();
    console::error("Unable to add the project with token, " + project_token + ", to the kill task queue. " + err_message);
  }
}

void Project::kill(string project_tasks, string tasks){
  Json json;
  string err;
  string project_token = ".";

  if(project_tasks.find(",") == std::string::npos && tasks.find(",") == std::string::npos && tasks.compare("") != 0 && project_tasks.compare("") != 0){
    json = Json::parse(Request::kill_project_task(project_tasks, tasks).body, err);
  } else if(project_tasks.find(",") == std::string::npos && project_tasks.compare("") != 0){
    if(tasks.compare("") == 0){
      json = Json::parse(Request::kill_project_task(project_tasks).body, err);
    } else {
      json = Json::parse(Request::kill_project_task(project_tasks, tasks).body, err);
    } 
  } else if(tasks.find(",") == std::string::npos && tasks.compare("") != 0){

    if(project_tasks.compare("") == 0){
      json = Json::parse(Request::kill_task(tasks).body, err);
    } else{
      json = Json::parse(Request::kill_task(tasks, project_tasks).body, err);
    }
  } 
  if(project_tasks.find(",") != std::string::npos && tasks.find(",") != std::string::npos){
    console::error("Please provide a project token in your skafos kill command.");
  } else if (tasks.find(",") != std::string::npos && project_tasks.compare("") == 0){
    console::error("Please provide a project token in your skafos kill command.");
  } else if (project_tasks.find(",") != std::string::npos && tasks.compare("") == 0){
    console::error("Please provide a project token in your skafos kill command.");
  } 
  else {
    string kill_message_sent = json["ok"]["intent"].string_value();
    if (kill_message_sent.compare("") != 0){
      console::success("Successfully added to the kill task queue.");
    } else{
      string err_message = json["message"].string_value();
      console::error("Unable to add to the kill task queue. " + err_message);
    }
  }
}

void Project::kill(string project_token, string project_tasks, string tasks){
  string err;
  Json json = Json::parse(Request::kill_project(project_token, project_tasks, tasks).body, err);
  string kill_message_sent = json["ok"]["intent"].string_value();
  if (kill_message_sent.compare("") != 0){
    console::success("Successfully added the project with token " + project_token + " and associated tasks to the kill task queue.");
  } else{
    string err_message = json["error"].string_value();
    console::error("Unable to add the project with token, " + project_token + ", to the kill task queue. " + err_message);
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
