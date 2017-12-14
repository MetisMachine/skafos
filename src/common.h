#ifndef __CLI_COMMON__
#define __CLI_COMMON__

#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>
#include <termios.h>
#include <sys/types.h>
#include <pwd.h>

#include "termcolor.hpp"
#include "json11.hpp"

const std::string CLIENT_ID = "f1f6e59f3f6f8ffecde29d34ad18f673";
const std::string API_URL   = "https://api.metis.wtf/v1";

#define console(msg) std::cout << msg << std::endl

#endif
