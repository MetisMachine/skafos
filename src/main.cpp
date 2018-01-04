#include <iostream>
#include <docopt.h>
#include "usage.h"
#include "common.h"
#include "auth/auth.h"
#include "logs/logs.h"
#include "env/env.h"
#include "templates/templates.h"
#include "project/project.h"
#include "dispatch/dispatch.h"

using namespace std;

int cmdMatch;

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

  cmdMatch = Dispatch::nameMatch(string(argv[1]));
  Dispatch::dispatch(argc, argv, cmdMatch);

  return EXIT_SUCCESS;
}
