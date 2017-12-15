#include <iostream>
#include <docopt.h>

#include "common.h"
#include "auth/auth.h"
#include "helpers/helpers.h"
#include "version.h"
#include "logs/logs.h"
#include "env/env.h"
#include "templates/templates.h"

using namespace std;

static const char USAGE[] =
R"(
Skafos

Usage:
    skafos (setup|new|auth|version)...
    skafos new <name>
    skafos templates [--update] [--search <search_term>]
    skafos logs [-n <num>] [--tail] [--project <project_token>]
    skafos -h | --help
    skafos --version
Commands:
    setup       Setup development environment.
    new         Create a new project
    auth        Authenticate request.    
    version     Shows version.
    logs        Get logs for a project.
Options:
    -V --version             Shows version.

If you need help, feel free to reach out:
    https://metismachine.com
    https://twitter.com/metismachine
    https://github.com/metismachine

)";

void setup() {
  Env::instance()->setup();
}

void new_project(string name) {
  cout << "New project: " << name << endl;
}

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
    setup();

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
  if(lg != args.end()) {
    string project  = "";
	  long numlines   = 0;
	  bool follow     = false;
    auto num        = args.find("<num>");

	  if(num != args.end() && num->second) {
		  numlines = num->second.asLong();
	  }
    
    auto tail = args.find("--tail");
  
    if(tail != args.end() && tail->second) {
		  follow = tail->second.asBool();
    }
    
	  auto token = args.find("<project_token>");
  
    if(token != args.end() && token->second) {
		  project = token->second.asString();
	  } else {
		  cout << "A project token is required" << endl;
	  }

	  Logs::print(project, numlines, follow);

	  return EXIT_SUCCESS;
  }

  auto nw = args.find("new");
  if(nw != args.end()) {

    auto arg = args.find("<name>");
    if(arg != args.end() && arg->second) {
      new_project(arg->second.asString());
    } else {
      cout << "A project name is required" << endl;
    }

    return EXIT_SUCCESS;
  }

  return EXIT_SUCCESS;
}
