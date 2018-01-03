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
#include "templates/templates.h"
#include "project/project.h"

using namespace std;

struct command setupCmd = {"setup", {}, false};
struct command initCmd = {"init", {"--template"}, true};
struct command authCmd = {"auth", {}, false};
struct command templatesCmd = {"templates", {"--update", "--search"}, true};
struct command logsCmd = {"logs", {"-n", "--tail"}, true};
struct command commandList[5] = {setupCmd, initCmd, authCmd, templatesCmd, logsCmd};

int Dispatch::nameMatch(string arg){
  for (int j = 0; j < sizeof(commandList)/sizeof(commandList[0]) ; j++){
    if (arg.compare(commandList[j].name) == 0){
      return j; 
    }
  }
  return -1; 
}

int Dispatch::flagMatch(int argc, char **argv, string flag){
    for (int l = 0; l < argc; l++){
      if(flag.compare(argv[l]) == 0){
        return l;
      }
    }
  return -1;
}

std::map<std::string, int> findFlags(int argc, char **argv, int cmdIndex){
  std::map<std::string, int> foundFlags;
  for(int k = 0; k < commandList[cmdIndex].flags.size(); k++){
    string key = commandList[cmdIndex].flags[k];
    int value = Dispatch::flagMatch(argc, argv, commandList[cmdIndex].flags[k]);
    foundFlags[key] = value;
  }
  return foundFlags;
}

void setup() {
  Env::instance()->setup();
}

void init(int argc, char **argv, int cmdIndex){
  std::map<std::string, int> initFlags = findFlags(argc, argv, cmdIndex);
  string name = ".";
  string tpl = "base";
  if(argc > 2){
    if(string(argv[2]).compare("--template") != 0){
      name = string(argv[2]);
    }
  }
  if(initFlags.find("--template")->second != -1){
    int tplIndex = initFlags.find("--template")->second;
    if(tplIndex+1 < argc){
      tpl = argv[tplIndex+1];
    }
  }
  Project::init(name, tpl);
}

void auth(){
  Auth::authenticate();
}

void templates(int argc, char **argv, int cmdIndex){
  std::map<std::string, int> templateFlags = findFlags(argc, argv, cmdIndex);
  if(templateFlags.find("--update")->second != -1){
    Template::update();
  }
  if(templateFlags.find("--search")->second != -1){
    int searchIndex = templateFlags.find("--search")->second;
    if(searchIndex+1 < argc){
       Template::search(argv[searchIndex+1]);
    } else{
       cout << "A <search_term> is required." << endl;
    }
  }  
}

void logs(int argc, char **argv, int cmdIndex){
  std::map<std::string, int> logFlags = findFlags(argc, argv, cmdIndex);
  string project  = "";
	long numlines   = 0;
	bool follow     = false;
  if (string(argv[2]).compare("--tail") != 0 || string(argv[2]).compare("-n") != 0){
    project = string(argv[2]);
  } else {
		cout << "A project token is required" << endl;
	}
  if(logFlags.find("-n")->second != -1){
    int numIndex = logFlags.find("-n")->second;
    if(numIndex+1 < argc){
      numlines = long(argv[numIndex+1]);
    }
  }
  if(logFlags.find("--tail")->second != -1){
    follow = true;
  }
  Logs::print(project, numlines, follow);
}

typedef void (*voidFunctionType)(void);

struct FunctionCaller{

    std::map<std::string,std::pair<voidFunctionType,std::type_index>> m;

    template<typename T>
    void insert(std::string string_name, T func){
        auto tt = std::type_index(typeid(func));
        m.insert(std::make_pair(string_name,
                        std::make_pair((voidFunctionType)func,tt)));
    }

    template<typename T,typename... Args>
    T callFunction(std::string string_name, Args... args){
        auto iter = m.find(string_name);
  
        auto mapVal = iter->second;
typedef void (*voidFunctionType)(void);

struct FunctionCaller{

    std::map<std::string,std::pair<voidFunctionType,std::type_index>> m;

    template<typename T>
    void insert(std::string string_name, T func){
        auto tt = std::type_index(typeid(func));
        m.insert(std::make_pair(string_name,
                        std::make_pair((voidFunctionType)func,tt)));
    }

    template<typename T,typename... Args>
    T callFunction(std::string string_name, Args... args){
        auto iter = m.find(string_name);
  
        auto mapVal = iter->second;
        
        auto typeCasted = (T(*)(Args ...))(mapVal.first); 
        assert(mapVal.second == std::type_index(typeid(typeCasted)));
        return typeCasted(std::forward<Args>(args)...);
    }
};

