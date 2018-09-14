#include "env/env.h"
#include "whoami.h"
#include "organization/organization.h"

using namespace json11;

void Whoami::information() {
  Env::instance()->load_defaults();
  std::string default_org_name = Env::instance()->get(METIS_DEFAULT_ORG);

  std::string err;
  Json json = Json::parse(Request::whoami().body, err);
  std::string error_message = json["error"].string_value();

  if (error_message.size() > 0) {
    console::error("There was an error listing your user information. \n");
  } else {
    console::info("Current Skafos user: ");
    console::info("   " + json["email"].string_value() + "\n");

    console::info("Default organization: ");
    if (default_org_name.size() == 0) {
      console::info("   You don't have a default organization set. \n     You can set one using the skafos orgs command. \n");
    } else {
      console::info("   " + default_org_name + "\n");
    }

    Organization::list();
    }
}
