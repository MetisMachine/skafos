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


void auth(){
  Auth::authenticate();
}

