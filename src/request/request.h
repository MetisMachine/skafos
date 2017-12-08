#ifndef __request_h__
#define __request_h__
#pragma once

#include "common.h"

class Request {
public:
  static cpr::Response authenticate(std::string email, std::string password);

private:
  
};

#endif
