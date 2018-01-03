#ifndef __CLI_OUT__
#define __CLI_OUT__

#include <atomic>

#include "common.h"
#include "termcolor.hpp"

namespace console {
  /*
  Make this a class so you can do start and stops on a loader.
  
  */
  #define GREEN   termcolor::green
  #define RED     termcolor::red
  #define BLUE    termcolor::blue
  #define YELLOW  termcolor::yellow
  #define CYAN    termcolor::cyan
  #define BOLD    termcolor::bold
  #define RESET   termcolor::reset
  #define WHITE   termcolor::white
  
  const std::string ARROW     = "➜";
  const std::string CHECKMARK = "✓";
  const std::string ECKS      = "✗";
  const std::string DBL_ARROW = "»";
  const std::string EX_POINT  = "!";
  const std::string PINCH     = "ː";

  #define SPIN_DOTS   {"⠋","⠙","⠹","⠸","⠼","⠴","⠦","⠧","⠇","⠏"}
  #define SPIN_ARC    {"◜","◠","◝","◞","◡","◟"}
  #define SPIN_BAR    {"[    ]","[   =]","[  ==]","[ ===]","[====]","[=== ]","[==  ]","[=   ]"}
  #define SPIN_LINES  {"-","\\","|","/"}

  // Pretty command line loader.

  class loader {
  public:
    loader(std::string message);
    ~loader();

    void start();
    void stop();

  private:
    std::atomic<bool> _loading;
    std::string _message;
  };

  void success(std::string message);
  void info(std::string message);
  void warn(std::string message);
  void error(std::string message);
  void debug(std::string message);
  void cli_args(int argc, char **argv);
}

#endif
