#include <iostream>
#include <docopt.h>
#include "usage.h"
#include "common.h"
#include "auth/auth.h"
#include "logs/logs.h"
#include "env/env.h"
#include "templates/templates.h"
#include "project/project.h"

using namespace std;

int main(int argc, char **argv) {
  Env::instance()->load_credentials();

  string title = (
    string("\nSkafos version: ") + 
    VERSION + 
    string("\nMetis Machine https://metismachine.com\n")
  );

  map<string, docopt::value> args = docopt::docopt(USAGE,
    { argv + 1, argv + argc },
    true,
    title.c_str()
  );

  auto stp = args.find("setup");
  if(stp != args.end() && stp->second.asLong() > 0) {
    Env::instance()->setup();

    return EXIT_SUCCESS;
  }

  auto ath = args.find("auth");
  if(ath != args.end() && ath->second.asLong() > 0) {
    Auth::authenticate();

    return EXIT_SUCCESS;
  }

  auto tpl = args.find("templates");
  if(tpl != args.end()) {
    auto upt = args.find("--update");

    if(upt != args.end() && upt->second.asBool()) {
      Template::update();

      return EXIT_SUCCESS;
    }

    auto srch = args.find("--search");

    if(srch != args.end() && srch->second) {
      auto search_for = args.find("<search_term>");

      if(search_for != args.end() && search_for->second) {
        Template::search(search_for->second.asString());

        return EXIT_SUCCESS;
      }
    }
  }

  auto lg = args.find("logs");
  if(lg != args.end() && lg->second.asBool()) {
    string project  = "";
	  long numlines   = 0;
	  bool follow     = false;
    
    auto n = args.find("-n");
    if(n != args.end() && n->second.asBool()) {
      auto num = args.find("<num>");
      
      if(num != args.end() && num->second) {
		    numlines = num->second.asLong();
	    }
    }
    
    auto tail = args.find("--tail");
  
    if(tail != args.end() && tail->second.asBool()) {
		  follow = tail->second.asBool();
    }
    
	  auto token = args.find("<project_token>");
  
    if(token != args.end() && token->second) {
		  project = token->second.asString();
	  }
    
	  Logs::print(project, numlines, follow);

	  return EXIT_SUCCESS;
  }

  auto nw = args.find("init");
  if(nw != args.end()) {
    string tpl = "base";

    auto name_arg = args.find("<name>");

    string name = ".";    
    if(name_arg != args.end() && name_arg->second) {
      name = name_arg->second.asString();
    }

    auto tpl_arg = args.find("--template");
    if(tpl_arg != args.end() && tpl_arg->second.isString()) {
      tpl = tpl_arg->second.asString();
    }


    Project::init(name, tpl);
    
    return EXIT_SUCCESS;
  }

  return EXIT_SUCCESS;
}
