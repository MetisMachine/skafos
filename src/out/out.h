#ifndef __CLI_OUT__
#define __CLI_OUT__

#include "common.h"
#include "termcolor.hpp"

namespace Out {
  #define GREEN     termcolor::green;
  #define RED       termcolor::red;
  #define RESET     termcolor::reset;
  #define ARROW     "➜"
  #define CHECKMARK "✓"

  #define SPIN_DOTS   {"⠋","⠙","⠹","⠸","⠼","⠴","⠦","⠧","⠇","⠏"}
  #define SPIN_ARC    {"◜","◠","◝","◞","◡","◟"}
  #define SPIN_BAR    {"[    ]","[   =]","[  ==]","[ ===]","[====]","[=== ]","[==  ]","[=   ]"}
  #define SPIN_LINES  {"-","\\","|","/"}

  void loading(std::string message);
}

#endif
