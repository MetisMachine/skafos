#include <iostream>
#include <signal.h>
#include <docopt.h>
#include "usage.h"
#include "common.h"
#include "auth/auth.h"
#include "logs/logs.h"
#include "env/env.h"
#include "templates/templates.h"
#include "project/project.h"
#include "dispatch/dispatch.h"
#include "termcolor.hpp"

using namespace std;

void handle_signal(int s) {
  cout << endl << flush;
  
  exit(EXIT_SUCCESS);
}

int main(int argc, char **argv) {
  if(ENVIRONMENT == "staging") {
    cout
    << endl
    << termcolor::bold
    << termcolor::cyan
    << ">>> Running in staging..."
    << termcolor::reset
    << endl
    << endl;
  }


  signal(SIGINT, handle_signal);  

  Env::instance()->load_credentials();

  string title = (
    string("\nSkafos version: ") + 
    VERSION + 
    string("\nMetis Machine https://metismachine.com\n")
  );

  map<string, docopt::value> args = docopt::docopt(
    USAGE,
    { argv + 1, argv + argc },
    true,
    title.c_str()
  );

  Dispatch::dispatch(
    argc, 
    argv, 
    Dispatch::name_match(string(argv[1]))
  );

  cout << endl << flush;

  return EXIT_SUCCESS;
}
