#include "dispatch.h"
#include <iostream> 
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <typeinfo>
#include <typeindex>
#include <cassert>
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

struct command setup_cmd        = {"setup", {}, false, true};
struct command init_cmd         = {"init", {"--template"}, true, true};
struct command auth_cmd         = {"auth", {}, false, false};
struct command templates_cmd    = {"templates", {"--update", "--search"}, true, true};
struct command env_cmd          = {"env", {"--set"}, true, true};
struct command logs_cmd         = {"logs", {"-n", "--tail"}, true, true};
struct command fetch_cmd        = {"fetch", {"--table"}, true, true};
struct command kill_task_cmd    = {"kill", {"--task"}, true, true};

struct command command_list[8]  = {
  setup_cmd, 
  init_cmd, 
  auth_cmd, 
  templates_cmd, 
  logs_cmd, 
  env_cmd,
  fetch_cmd,
  kill_task_cmd
};

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

  Project::init(name, tpl);
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
  
  console::info("Connecting to task logs...");
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

void kill_task(int argc, char **argv, int cmd_index){
  map<string, int> flags = find_flags(argc, argv, cmd_index);
  string kill_id                = ".";
  string task_type              = "";

  if(flags.find("--all")->second != -1) {
    task_type = "all";
    int all_index = flags.find("--all")->second;

    if(all_index + 1 < argc) {
      kill_id = argv[all_index + 1];
    }
  } else if(flags.find("--project_task")->second != -1) {
    task_type = "project_task";
    int proj_task_index = flags.find("--project_task")->second;

    if(proj_task_index + 1 < argc) {
      kill_id = argv[proj_task_index + 1];
    }
  } else {
    task_type = "task";
    int task_index = flags.find("--task")->second;

    if(task_index + 1 < argc) {
      kill_id = argv[task_index + 1];
    }
  }

  Project::kill(kill_id, task_type);
}


int Dispatch::dispatch(int argc, char **argv, int cmd_index) {
  FunctionCaller disp;

  disp.insert("setup",      setup);
  disp.insert("init",       init);
  disp.insert("auth",       auth);
  disp.insert("templates",  templates);
  disp.insert("logs",       logs);
  disp.insert("env",        envvar);
  disp.insert("fetch",      fetch_table);
  disp.insert("kill",       kill_task);

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
