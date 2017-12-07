#ifndef __auth_h__
#define __auth_h__
#pragma once

#include "common.h"

class Auth {
public:
  static void authenticate();
private:
  static std::string password_input(std::string prompt);
  static void write_credentials(json11::Json object);
};

#endif
