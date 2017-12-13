#include <iostream>
#include <docopt.h>

#include "yaml.h"
#include "common.h"
#include "auth/auth.h"
#include "helpers/helpers.h"
#include "version.h"
#include "logs/logs.h"

using namespace std;

static const char USAGE[] =
R"(
Skafos

Usage:
    skafos (new|auth|version)...
    skafos new <name>
    skafos logs [-n <num>] [--tail] [--project <project_token>]
    skafos -h | --help
    skafos --version
Commands:
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

void new_project(string name) {
  cout << "New project: " << name << endl;
}

void auth() {
  cout << "AUTH" << endl;

  Auth::authenticate();
}

int main(int argc, char **argv) {
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

  auto ath = args.find("auth");
  if(ath != args.end() && ath->second.asLong() > 0) {
    auth();

    return 0;
  }

  auto lg = args.find("logs");
  if(lg != args.end()) {
    std::string project = "";
	long        numlines = 0;
	bool        follow = false;

    auto num = args.find("<num>");
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

	return 0;
  }

  auto nw = args.find("new");
  if(nw != args.end()) {

    auto arg = args.find("<name>");
    if(arg != args.end() && arg->second) {
      new_project(arg->second.asString());
    } else {
      cout << "A project name is required" << endl;
    }

    return 0;
  }

  return 0;
}
