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

void Project::init(string name, string tpl, bool master) {
  Template::update();
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

  string version = (!master && tpl_details.version.length() > 0)
    ? tpl_details.version 
    : "master";

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
  string job_id = json["jobs"][0]["id"].string_value();
  string job_name = json["jobs"][0]["name"].string_value();

  config_template.setValue("token", token);
  config_template.setValue("name", proj);
  config_template.setValue("job_id", job_id);
  config_template.setValue("job_name", job_name);

  FileManager::write(template_path, config_template.render());
}

void Project::kill(string project_token){
  if(project_token.compare(".") == 0){
    project_token = PROJECT_TOKEN;
  }
  string kill_type = "project";
  string kill_details = project_token;
  string err;
  Json json = Json::parse(Request::kill_project(project_token).body, err);
  Project::kill_message(json, kill_type, kill_details);
}

void Project::kill(string jobs, string deployments){
  Json json;
  string kill_type = "job";
  string kill_details = "";
  string err;
  string project_token = ".";

  if(jobs.find(",") == std::string::npos && deployments.find(",") == std::string::npos && deployments.compare("") != 0 && jobs.compare("") != 0){
    json = Json::parse(Request::kill_job(jobs, deployments).body, err);  
  } else if(jobs.find(",") == std::string::npos && jobs.compare("") != 0){
    if(deployments.compare("") == 0){
      json = Json::parse(Request::kill_job(jobs).body, err);
    } else {
      json = Json::parse(Request::kill_job(jobs, deployments).body, err);
    } 
  } else if(deployments.find(",") == std::string::npos && deployments.compare("") != 0){

    if(jobs.compare("") == 0){
      json = Json::parse(Request::kill_deployment(deployments).body, err);
      kill_type = "deployment";
    } else{
      json = Json::parse(Request::kill_deployment(deployments, jobs).body, err);
      kill_type = "deployment";
    }
  } 
  if(jobs.find(",") != std::string::npos && deployments.find(",") != std::string::npos){
    console::error("Please provide a project token in your skafos kill command.");
  } else if (deployments.find(",") != std::string::npos && jobs.compare("") == 0){
    console::error("Please provide a project token in your skafos kill command.");
  } else if (jobs.find(",") != std::string::npos && deployments.compare("") == 0){
    console::error("Please provide a project token in your skafos kill command.");
  } 
  else {
    Project::kill_message(json, kill_type, kill_details);
  }
}

void Project::kill(string project_token, string jobs, string deployments){
  string err;
  string kill_type = "outside directory";
  string kill_details = project_token;
  Json json = Json::parse(Request::kill_project(project_token, jobs, deployments).body, err);
  Project::kill_message(json, kill_type, kill_details);
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

 void Project::kill_message(json11::Json kill_json, string kill_type, string kill_details){
    string kill_message_sent = kill_json["intent"].string_value();
    if (kill_message_sent.compare("") != 0){
      if (kill_type.compare("job") == 0 || kill_type.compare("deployment") == 0){
        console::success("Successfully instructed the platform to kill the specified " +  kill_type + ".");
      } else if (kill_type.compare("project") == 0){
        console::success("Successfully instructed the platform to kill the " + kill_type + " with token " + kill_details + ".");
      } else{
        console::success("Successfully instructed the platform to kill the specified jobs for the project with token " + kill_details + ".");
      }
    } else{
      string err_message = kill_json["message"].string_value();
      if (err_message.compare("") == 0){
        err_message = kill_json["error"].string_value();
      }
      if (kill_type.compare("job") == 0 || kill_type.compare("deployment") == 0){
        console::error("Unable to intstruct the platform to kill the specified " + kill_type + "." + err_message);
      } else if (kill_type.compare("project") == 0){
        console::error("Unable to instruct the platform to kill the " + kill_type + " with token " + kill_details + ". " + err_message);
      } else {
        console::error("Unable to instruct the platform to kill the specified jobs for the project with token " + kill_details + ". " + err_message);
      }
    }
}
