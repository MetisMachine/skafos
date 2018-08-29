#ifndef __CLI__ORGANIZATION_H__
#define __CLI__ORGANIZATION_H__

#include "common.h"

class Organization {
  public:
    static void list();
    static void set_default(std::string display_name);
};

#endif // !__CLI__ORGANIZATION_H__
