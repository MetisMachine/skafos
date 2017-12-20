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
#include "out/out.h"

const std::string CLIENT_ID = "f1f6e59f3f6f8ffecde29d34ad18f673";
const std::string API_URL   = "https://api.metis.wtf/v1";

// const std::string LOCAL_ENDPOINT      = "https://localhost:4000";
// const std::string STAGING_ENDPOINT    = "http://api.metis.wtf";
// const std::string PRODUCTION_ENDPOINT = "https://api.metismachine.io";
// const std::string API_VERSION         = "v1";


#endif
