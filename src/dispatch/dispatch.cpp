#include "dispatch.h"
#include <iostream> 
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <typeinfo>
#include <typeindex>
#include <cassert>
#include <algorithm>
#include "common.h"
#include "auth/auth.h"
#include "version.h"
#include "logs/logs.h"
#include "env/env.h"
#include "env_var/env_var.h"
#include "templates/templates.h"
#include "project/project.h"
#include "project_env/project_env.h"
#include "data/data.h"
#include "organization/organization.h"

using namespace std;

typedef void (*VoidFunctionType)(void);

struct FunctionCaller {

    map<string,pair<VoidFunctionType,type_index>> m;

    template<typename T>
    void insert(string string_name, T func){
        auto tt = type_index(typeid(func));
        
        m.insert(make_pair(string_name, make_pair((VoidFunctionType)func,tt)));
    }

    template<typename T,typename... Args>
    T callFunction(std::string string_name, Args... args){
        auto iter       = m.find(string_name);
        auto mapVal     = iter->second;
        auto typeCasted = (T(*)(Args ...))(mapVal.first); 

        assert(mapVal.second == type_index(typeid(typeCasted)));

        return typeCasted(forward<Args>(args)...);
    }
};

struct command setup_cmd              = {"setup", {}, false, true};
struct command init_cmd               = {"init", {"--template", "--master"}, true, true};
struct command auth_cmd               = {"auth", {}, false, false};
struct command templates_cmd          = {"templates", {"--update", "--search"}, true, true};
struct command env_cmd                = {"env", {"--set"}, true, true};
struct command create_cmd             = {"create", {"--project"}, true, true};
struct command logs_cmd               = {"logs", {"-n", "--tail"}, true, true};
struct command fetch_cmd              = {"fetch", {"--table"}, true, true};
struct command kill_deployment_cmd    = {"kill", {"--deployments", "--job_ids"}, true, true};
struct command remote_cmd             = {"remote", {"--default"}, true, true};
struct command organizations_cmd      = {"orgs", {}, true, true};

struct command command_list[11]  = {
  setup_cmd, 
  init_cmd, 
  auth_cmd, 
  templates_cmd, 
  create_cmd,
  logs_cmd, 
  env_cmd,
  fetch_cmd,
  kill_deployment_cmd,
  remote_cmd,
  organizations_cmd
};


template <typename T>
bool contains(std::vector<T> & vectorOfElements, const T & element)
{
	// Find the iterator if element in list
	auto it = std::find(vectorOfElements.begin(), vectorOfElements.end(), element);
	//return if iterator points to end or not. It points to end then it means element
	// does not exists in list
	return it != vectorOfElements.end();
}

int Dispatch::name_match(string arg) {
  for (int j = 0; j < sizeof(command_list)/sizeof(command_list[0]) ; j++) {
    if (arg.compare(command_list[j].name) == 0) {
      return j; 
    }
  }

  return -1; 
}

int Dispatch::flag_match(int argc, char **argv, string flag) {
    for (int l = 0; l < argc; l++){
      if(flag.compare(argv[l]) == 0){
        return l;
      }
    }
  return -1;
}

map<string, int> find_flags(int argc, char **argv, int cmd_index) {
  map<string, int> foundFlags;

  for(int k = 0; k < command_list[cmd_index].flags.size(); k++) {
    string key      = command_list[cmd_index].flags[k];
    foundFlags[key] = Dispatch::flag_match(argc, argv, command_list[cmd_index].flags[k]);;
  }

  return foundFlags;
}

void setup() {
  Env::instance()->setup();
}

void init(int argc, char **argv, int cmd_index) {
  map<string, int> init_flags = find_flags(argc, argv, cmd_index);
  string name                 = ".";
  string tpl                  = "base";
  bool master                 = false;
  
  if(argc > 2) {
    if(string(argv[2]).compare("--template") != 0) {
      name = string(argv[2]);
    }
  }

  if(init_flags.find("--template")->second != -1) {
    int tpl_index = init_flags.find("--template")->second;
    
    if(tpl_index + 1 < argc) {
      tpl = argv[tpl_index + 1];
    }
  }

  if(init_flags.find("--master")->second != -1) {
    master = true;
  }

  Project::init(name, tpl, master);
}

void auth() {
  Auth::authenticate();
}

void templates(int argc, char **argv, int cmd_index) {
  map<string, int> template_flags = find_flags(argc, argv, cmd_index);

  if(template_flags.find("--update")->second != -1) {
    Template::update();
  }

  if(template_flags.find("--search")->second != -1){
    int search_index = template_flags.find("--search")->second;

    if(search_index + 1 < argc) {
       Template::search(argv[search_index + 1]);
    } else {
       console::error("A <search_term> is required.");
    }
  }  
}

void envvar(int argc, char **argv, int cmd_index) {
  map<string, int> flags = find_flags(argc, argv, cmd_index);

  if(argc == 3) {
    string key = string(argv[2]);
    
    EnvVar::get(key);

    exit(EXIT_SUCCESS);
  }

  if(argc > 3) {
    string key = string(argv[2]);

    if(string(argv[3]) == "--delete") {
      EnvVar::del(key);

      exit(EXIT_SUCCESS);
    }

    if(string(argv[3]) == "--set") {
      string val = string(argv[4]);

      EnvVar::set(key, val);

      exit(EXIT_SUCCESS);
    }
  }

  EnvVar::list();
  exit(EXIT_SUCCESS);
}

void create(int argc, char **argv, int cmd_index) {
  map<string, int> create_flags = find_flags(argc, argv, cmd_index);
  string creation_name        = "";
  string kind                 = "";
  string project_token        = "";
  vector<string> kinds        = {"job"};

  if(argc < 4) {
    console::error("A type and name is required.");
    exit(EXIT_FAILURE);
  }

  kind = string(argv[2]);
  cout << "The kind = " << kind << "\n";
  if (!contains(kinds, kind)){
    console::error("Invalid Creation Type.");
    exit(EXIT_FAILURE);
  }

  creation_name = string(argv[3]);
  if (creation_name.size() < 1) {
    console::error("Invalid Name.");
    exit(EXIT_FAILURE);
  }

  if(create_flags.find("--project")->second != -1) {
    int project_index = create_flags.find("--project")->second;
    if(project_index + 1 < argc) {
      project_token = argv[project_index + 1];
    }
  } else {
    string config_path  = FileManager::cwd() + "/metis.config.yml";
    project_token      = (FileManager::file_exists(config_path))? ProjectEnv::current().token : "";
  }

  if(project_token.size() < 1) {
    console::error("A project token is required");
    exit(EXIT_FAILURE);
  }
  Project::create_job(creation_name, project_token);
}

void logs(int argc, char **argv, int cmd_index){
  std::map<std::string, int> logFlags = find_flags(argc, argv, cmd_index);

  string config_path  = FileManager::cwd() + "/metis.config.yml";
  string project      = (FileManager::file_exists(config_path))? ProjectEnv::current().token : "";
	int numlines        = 0;
	bool follow         = false;

  if(argc > 2) {
    if((string(argv[2]) != "--tail") && (string(argv[2]) != "-n")) {
      project = string(argv[2]);
    }

    if(project.size() < 1) {
      console::error("A project token is required");
      
      exit(EXIT_FAILURE);
    }
    
    if(logFlags.find("-n")->second != -1) {
      int numIndex = logFlags.find("-n")->second;

      if(numIndex+1 < argc) {
        numlines = atoi(argv[numIndex+1]);
      }
    }

    if(logFlags.find("--tail")->second != -1) {
      follow = true;
    }
  }

  if(project.size() < 1) {
    console::error("Unable to start logging, no project token available.");

    exit(EXIT_FAILURE);
  }
  
  console::info("Connecting to project logs...");
  Logs::print(project, numlines, follow);
}

void fetch_table(int argc, char **argv, int cmd_index) {
  map<string, int> flags = find_flags(argc, argv, cmd_index);

  if(flags.find("--table")->second != -1) {
    int index = flags.find("--table")->second;

    if(index + 1 < argc) {
      Data::fetch(argv[index + 1]);
    }
  }

  exit(EXIT_FAILURE);
}

void kill_deployment(int argc, char **argv, int cmd_index){
  map<string, int> flags = find_flags(argc, argv, cmd_index);
  string project_token        = ".";
  string jobs                 = "";
  string deployments          = "";

  if(argc == 3) {
    project_token = string(argv[2]);
    
    Project::kill(project_token);
  }
  
  if(argc > 3) {
    if(string(argv[2]).compare("--jobs") != 0 && string(argv[2]).compare("--deployments") != 0){
      project_token = string(argv[2]);
    } 
  }

  if(flags.find("--jobs")->second != -1){
    int job_index = flags.find("--jobs")->second;

    if(job_index + 1 < argc) {
      jobs = argv[job_index + 1];
    }
  }

  if(flags.find("--deployments")->second != -1){
    int deployment_index = flags.find("--deployments")->second;

    if(deployment_index + 1 < argc) {
      deployments = argv[deployment_index + 1];
    }
  }

  if(project_token.compare(".") != 0 && argc > 3){
    Project::kill(project_token, jobs, deployments);
  } 
  
  if(argc == 2){
    Project::kill(project_token);
  } 
  if(project_token.compare(".") == 0 && argc > 3) {
    Project::kill(jobs, deployments);
  }
}

void remote(int argc, char **argv, int cmd_index){
  string project_token = ".";

  if(argc == 4) {
    project_token = string(argv[3]);
  } 

  Project::remote_add(project_token);
}

void organizations(int argc, char **argv, int cmd_index) {
  switch(argc) {
    case 4:
      if (string(argv[3]) == "--default") {
        std::string name = std::string(argv[2]);
        Organization::set_default(name);
      } else {
        console::error("You must supply an organization name and the --default argument to switch default organizations.");
      }
      break;
    case 3:
      console::error("You must supply an organization name and the --default argument to switch default organizations.");
      break;
    case 2:
      console::info("All your organizations:\n");
      Organization::list();
      break;
  }
}


int Dispatch::dispatch(int argc, char **argv, int cmd_index) {
  FunctionCaller disp;

  disp.insert("setup",         setup);
  disp.insert("init",          init);
  disp.insert("auth",          auth);
  disp.insert("templates",     templates);
  disp.insert("create",        create);
  disp.insert("logs",          logs);
  disp.insert("env",           envvar);
  disp.insert("fetch",         fetch_table);
  disp.insert("kill",          kill_deployment);
  disp.insert("remote",        remote);
  disp.insert("orgs",          organizations);

  if(command_list[cmd_index].needs_auth) {
    VERIFY_AUTH();
  }

  if(command_list[cmd_index].flags.size() == 0 && command_list[cmd_index].has_args == false) {
    disp.callFunction<void>(command_list[cmd_index].name);
  } else {
    disp.callFunction<void>(command_list[cmd_index].name, argc, argv, cmd_index);
  }
  
  return EXIT_SUCCESS;
}
