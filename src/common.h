#ifndef __CLI_COMMON__
#define __CLI_COMMON__

#include <list>
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
#include "cli_env.h"
#include "console/console.h"
#include "env/env.h"

#define START_LOADING(message) \
auto __ld_ = new console::loader(message); \
__ld_->start(); \
sleep(1)

#define END_LOADING() \
__ld_->stop(); \
delete __ld_

#endif
