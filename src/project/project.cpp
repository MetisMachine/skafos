#include <algorithm>
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

#define project_directory(name) (name == "")?   \
  FileManager::cwd() : (name == ".") ?          \
  FileManager::resolve_path(name) :             \
  FileManager::cwd() + "/" + name

void Project::init(string name, string org_name, string tpl, bool master) {
  string directory = project_directory(name);
  Env::instance()->load_defaults();
  string default_org_name = Env::instance()->get(METIS_DEFAULT_ORG);

  if(org_name.size() == 0) {
    org_name = default_org_name;
  }

  string project_name = directory.substr(directory.find_last_of("/"));
    
  if(exists(directory)) {
    string config_path = directory + "/metis.config.yml";

    if(FileManager::file_exists(config_path)) {
      console::error("A metis.config.yml file is present in this directory. Unable to initialize a project");
      exit(EXIT_FAILURE);
    }

    existing_init(name, org_name, master);
  } else {
    template_init(name, org_name, tpl, master);
  }
}

void Project::create_job(string name, string project_token){
  string err;
  Json job_json     = Json::parse(Request::create_job(name, project_token).body, err);
  string error_message = job_json["error"].string_value();
  if (error_message.size() > 0){
    console::error("There was an error creating job: " + name + "\n" + error_message);
  } else if(job_json["id"].string_value().size() > 0){
    console::success("Add the following to your project's metis.config.yml file: \n");
    YAML::Emitter out;
    out << YAML::Literal << "\n";
    out << YAML::BeginSeq;
    out << YAML::BeginMap;
    out << YAML::Key << "job_id" << YAML::Value << job_json["id"].string_value();
    out << YAML::Key << "name" << YAML::Value << name;
    out << YAML::Key << "entrypoint" << YAML::Value << "<change_me>";
    out << YAML::EndMap;
    out << YAML::EndSeq;

    cout << out.c_str() << "\n";
  }
}

void Project::template_init(string name, string org_name, string tpl, bool master) { 
  Template::update();
  
  string directory    = project_directory(name);
  string project_name = directory.substr(directory.find_last_of("/"));
    
  if(exists(directory)) {
    string config_path = directory + "metis.config.yml";
    
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
  Json json;
  if (org_name.size() == 0) {
    json = Json::parse(Request::create_project(proj).body, err);
  } else {
    console::info("Creating project " + project_name + " under organization " + org_name);
    json = Json::parse(Request::create_project(proj, org_name).body, err);
  }

  if(json["error"].string_value().size() > 0) {
    console::error("Unable to create your project: " + json["error"].string_value());
  } else {
    string token  = json["token"].string_value();
    string job_id = json["jobs"][0]["id"].string_value();
    string job_name = json["jobs"][0]["name"].string_value();

    config_template.setValue("token", token);
    config_template.setValue("name", proj);
    config_template.setValue("job_id", job_id);
    config_template.setValue("job_name", job_name);

    FileManager::write(template_path, config_template.render());
  }
}

void Project::existing_init(string name, string org_name, bool master) {
  const string source = "project_token: {{token}}\n" 
                        "name: {{name}}\n" 
                        "jobs:\n" 
                        "  - job_id: {{job_id}}\n" 
                        "    language: python\n" 
                        "    name: Main\n" 
                        "    entrypoint: \"main.py\"\n";
  
  string err;
  string directory    = project_directory(name);
  string config_path  = directory + "/metis.config.yml";
  string project_name = directory.substr(directory.find_last_of("/"));
  string proj         = replace(project_name, "/", "");
  Json json;
  if (org_name.size() == 0) {
    json = Json::parse(Request::create_project(proj).body, err);
  } else {
    console::info("Creating project " + project_name + " under organization " + org_name);
    json = Json::parse(Request::create_project(proj, org_name).body, err);
  }

  string token        = json["token"].string_value();
  string job_id       = json["jobs"][0]["id"].string_value();
  string job_name     = json["jobs"][0]["name"].string_value();

  Jinja::Template config_template(source);

  config_template.setValue("token", token);
  config_template.setValue("name", proj);
  config_template.setValue("job_id", job_id);
  config_template.setValue("job_name", job_name);  

  FileManager::write(config_path, config_template.render());
}

void Project::remote_add(string project_token){
  if(project_token.compare(".") == 0){
    project_token = PROJECT_TOKEN;
  }

  std::string selected_org;
  std::string err;

  RestClient::Response req = Request::my_organizations();
  Json json = Json::parse(req.body, err);
  auto list = json.array_items();

  string list_size = std::to_string(list.size());
  
  if(stoi(list_size) == 1){
    selected_org = list[0]["id"].string_value();
  } else{
    cout << "Please select the organization for the project." << endl;
    int opt_iter = 1;
    for(auto i : list) {
      auto j = Json(i);
      std::string org_id = j["display_name"].string_value();
    
      org_id.erase(std::remove(org_id.begin(), org_id.end(), '"'), org_id.end());
      cout << std::to_string(opt_iter) << ". "+ org_id << endl;
      opt_iter++;
    }
    string opt_select;
    cout << "Enter your choice and press return: ";
    while(!(cin >> opt_select) && std::stoi(opt_select) - 1 < list.size()) {
      cin.clear();
      cin.ignore(1, '\n');
      cout << "Invalid choice. Valid numbers are [1-" << list_size << "]. Try again: ";
    }

    int index = std::stoi(opt_select) - 1;
    selected_org = list[index]["id"].string_value();
  }
    console::info("To add a new remote, copy the git remote add skafos command below and run it on the terminal.\n");
    console::info("$ git remote add skafos " + VASI_URL + selected_org + "/" + project_token + "\n");
    console::info("You can then push changes for deployment using the git push skafos <branch_name> command.");
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
