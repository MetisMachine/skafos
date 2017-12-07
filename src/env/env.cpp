#include "env.h"


// class Env {
// public:
//   static string get(std::string key);
//   static bool set(std::string key, std::string val);
// };

using namespace std;

string Env::get(string key) {
  char *res = getenv(key.c_str());

  return (res == NULL)? string() : string(res);
}

bool Env::set(string key, string val) {
  string build = key + "=" + val;

  return (putenv((char *)build.c_str()) == -1)? false : true;
}
