
#pragma once

#include "../common.h"

using namespace std;

namespace Fero {
  class Env {
    public:
      static string get(string key);
      static bool set(string key, string val);
  };
}
