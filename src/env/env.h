#ifndef __env_h__
#define __env_h__
#pragma once

#include "common.h"

const std::string METIS_API_TOKEN   = "METIS_API_TOKEN";
const std::string METIS_AUTH_TOKEN  = "METIS_OAUTH";
const std::string METIS_CREDENTIALS = "credentials.json";

struct metis_path {
  std::string home;
  std::string env;
  std::string credentials;
};

class Env {
public:
  static Env *instance();

  std::string get(std::string key);
  void set(std::string key, std::string val);
  bool load_credentials();
  void write_credentials(json11::Json object);

private:
  static Env *instance_;
  metis_path paths;
  bool ping();
  
  Env();
  std::string home_dir();
  std::string metis_dir();
};

#endif
