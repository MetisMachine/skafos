#include <iostream>
#include <docopt.h>
#include <json11.hpp>

#include "yaml.h"
#include "common.h"
#include "env/env.h"
#include "thread/thread.h"

using namespace std;

static const char USAGE[] =
R"(Metis Machine
  Usage:
    metis (-h | --help)
    metis setup
    metis init 
    metis --version

  Options:
    -h --help     Show this screen.
    -v --version  Show version.
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