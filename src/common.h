
#ifndef __common_h__
#define __common_h__
#pragma once

#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <termios.h>
#include <sys/types.h>
#include <pwd.h>
#include <fstream>

#include "json11.hpp"
#include "cpr/cpr.h"


#define __DIR__(stringName)             char buffer[PATH_MAX + 1]; \
                                        char *dir = realpath(__FILE__, buffer); \
                                        stringName = string(dir); \
                                        stringName.substr(0, stringName.rfind('/'));

#define EXPLODE(vect, str, sep)        int found = str.find_first_of(sep); \
                                       while(found != (int)string::npos) { \
                                          if(found > 0) vect.push_back(str.substr(0, found)); \
                                          str = str.substr(found + 1); \
                                          found = str.find_first_of(sep); } \
                                       if(str.length() > 0) vect.push_back(str);  


const std::string CLIENT_ID             = "f1f6e59f3f6f8ffecde29d34ad18f673";
const std::string API_URL               = "https://argus.metis.wtf/v1/users/login/";
const std::string METIS_API_TOKEN       = "METIS_API_TOKEN";
const std::string METIS_AUTH_TOKEN_ENV  = "METIS_OAUTH";
                                       
#endif
