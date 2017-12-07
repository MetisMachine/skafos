#include <iostream>
#include <docopt.h>

#include "yaml.h"
#include "common.h"
#include "auth/auth.h"

using namespace std;

static const char USAGE[] =
R"(Skafos Metis Machine CLI (0.0.1)

Usage:
    skafos (new|auth|version)...
    skafos new <name>
    skafos -h | --help
    skafos --version
Commands:
    new         Create a new project
    auth        Authenticate request.    
    version     Shows version.
Options:
    -V --version             Shows version.

If you need help, feel free to reach out:
    https://metismachine.com
    https://twitter.com/metismachine
    https://github.com/metismachine

\n
)";

void new_project(string name) {
  cout << "New project: " << name << endl;
}

void auth() {
  cout << "AUTH" << endl;

  Auth::authenticate();
}

int main(int argc, char **argv) {
  map<string, docopt::value> args = docopt::docopt(USAGE,
    { argv + 1, argv + argc },
    true,
    "metis 0.0.1"
  );

  auto ath = args.find("auth");
  if(ath != args.end() && ath->second.asLong() > 0) {
    auth();

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
