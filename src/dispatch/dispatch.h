#ifndef __CLI_DISPATCH__
#define __CLI_DISPATCH__

#include "common.h"

struct command{
  std::string name;
  std::vector<std::string> flags;
  bool has_args;
};

class Dispatch {
  public: 
    static int dispatch(int argc, char **argv, int cmd_ondex);
    static int name_match(std::string arg);
    static int flag_match(int argc, char **argv, std::string flag);
    static int find_flags(int argc, char **argv, int cmdIndex);
};

#endif
