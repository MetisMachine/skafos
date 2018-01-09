#include "console.h"
#include <iostream>
#include <docopt.h>

#include "common.h"
#include "usage.h"

#include <thread>
#include <stdarg.h>

using namespace std;

#define FLASH_MSG(clr, symbl, msg) \
cout      \
<< RESET  \
<< BOLD   \
<< clr    \
<< symbl  \
<< " "    \
<< RESET  \
<< msg    \
<< RESET  \
<< endl   

namespace console {

  loader::loader(string message) {
    this->_loading = false;
    this->_message = message;
  }

  loader::~loader() {
    this->_loading = false;
  }

  void loader::start() {
    if(this->_loading == true) {
      return;
    }
    
    std::thread t([&]() {
      this->_loading = true;

      list<string> frames       = SPIN_LINES;
      list<string>::iterator it = frames.begin();

      while(this->_loading) {        
        if(it == frames.end()) {
          it = frames.begin();

          continue;  
        }

        string frame = *it;

        cout << "\r" << BOLD << " " << frame << " " << this->_message << flush;
        usleep(useconds_t(0.08 * 1000000));

        advance(it, 1);
      }
    });

    t.detach();
  }

  void loader::stop() {
    cout << flush;
    cout << endl;
    this->_loading = false;
  }

  void headline(std::string message) {
    cout
    << RESET
    << BOLD 
    << message  
    << RESET
    << endl;
  }

  void success(string message) {
    FLASH_MSG(GREEN, CHECKMARK, message);
  }

  void log(string message) {
    FLASH_MSG(WHITE, PINCH, message);
  }

  void info(string message) {
    FLASH_MSG(BLUE, ARROW, message);
  }

  void warn(string message) {
    FLASH_MSG(YELLOW, (EX_POINT + ARROW), message);
  }

  void error(string message) {
    FLASH_MSG(RED, ECKS, message);
  }

  void debug(string message) {
    if(ENVIRONMENT == "production") { return; }

    FLASH_MSG(BLUE, DBL_ARROW, message);
  }

  void cli_args(int argc, char **argv) {
    if(ENVIRONMENT == "production") { return; }

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

    cout 
    << endl
    << termcolor::on_white
    << BLUE
    << PINCH << PINCH << PINCH << PINCH << PINCH << PINCH << PINCH << PINCH << PINCH << PINCH
    << PINCH << PINCH << PINCH << PINCH << PINCH << PINCH << PINCH << PINCH << PINCH << PINCH
    << "DEBUGGING"
    << PINCH << PINCH << PINCH << PINCH << PINCH << PINCH << PINCH << PINCH << PINCH << PINCH
    << PINCH << PINCH << PINCH << PINCH << PINCH << PINCH << PINCH << PINCH << PINCH << PINCH
    << RESET
    << endl
    << endl;

    for(auto const& arg : args) {
      cout 
      << YELLOW 
      << DBL_ARROW
      << RESET
      << WHITE
      << " Argument: "
      << RESET
      << BOLD
      << WHITE
      << arg.first
      << RESET
      << WHITE
      << " value: "
      << RESET
      << BOLD
      << WHITE
      << arg.second
      << endl;
    }

    cout 
    << endl
    << termcolor::on_white
    << BLUE
    << PINCH << PINCH << PINCH << PINCH << PINCH << PINCH << PINCH << PINCH << PINCH << PINCH
    << PINCH << PINCH << PINCH << PINCH << PINCH << PINCH << PINCH << PINCH << PINCH << PINCH
    << PINCH << PINCH << PINCH << PINCH << PINCH << PINCH << PINCH << PINCH << PINCH << PINCH
    << PINCH << PINCH << PINCH << PINCH << PINCH << PINCH << PINCH << PINCH << PINCH << PINCH
    << RESET
    << endl
    << endl;
  }
}
