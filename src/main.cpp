#include <iostream>
#include <docopt.h>
#include <json11.hpp>

#include "yaml.h"
#include "common.h"

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

int main(int argc, char **argv) {
  map<string, docopt::value> args = docopt::docopt(USAGE,
    { argv + 1, argv + argc },
    true,
    "metis 0.0.1"
  );

  for(auto const& arg : args) {
    cout << arg.first << " " << arg.second << endl;
  }

  YAML::Node node;
  node["key"] = "value";

  return 0;
}
