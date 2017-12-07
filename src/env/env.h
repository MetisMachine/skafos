#ifndef __env_h__
#define __env_h__
#pragma once

#include "common.h"

class Env {
public:
  static std::string get(std::string key);
  static bool set(std::string key, std::string val);
};

#endif
