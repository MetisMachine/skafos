#include "env.h"

namespace Fero {
  string Env::get(string key) {
    char *result = getenv(key.c_str());

    return (result == NULL)? string() : string(result);
  }

  bool Env::set(string key, string value) {
    string build = key + "=" + value;

    return (
      putenv((char *)build.c_str()) == -1
    )? false : true;
  }
}