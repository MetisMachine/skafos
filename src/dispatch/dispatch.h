#ifndef __CLI_DISPATCH__
#define __CLI_DISPATCH__

#include "common.h"

struct command{
  std::string name;
  std::vector<std::string> flags;
  bool hasArgs;
};

class Dispatch {
  public: 
    static int dispatch(int argc, char **argv, int cmdIndex);
    static int nameMatch(std::string arg);
    static int flagMatch(int argc, char **argv, std::string flag);
    static int findFlags(int argc, char **argv, int cmdIndex);
};

#endif