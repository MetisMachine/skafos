#include "file/file.h"
#include "organization.h"

using namespace json11;

void Organization::list() {
  std::string err;
  Json json = Json::parse(Request::my_organizations().body, err);
  std::string error_message = json["error"].string_value();

  if (error_message.size() > 0) {
    console::error("There was an error listing your organizations: " + error_message + "\n");
  } else if (json.is_array()) {
    console::info("Your organizations: ");
    auto orgs = json.array_items();
    for (int i = 0; i < orgs.size(); i++) {
      console::info("   " + orgs[i]["display_name"].string_value());
    }
  }
}

void Organization::set_default(std::string org_name) {
  std::string err;
  Json json = Json::parse(Request::org_by_name(org_name).body, err);
  std::string error_message = json["error"].string_value();

  if (error_message.size() > 0) {
    console::error("There was an error listing your organizations: " + error_message + "\n");
  } else {
    Env::instance()->write_default_org(org_name);
    console::info(org_name + " is now your default organization");
  }
}

