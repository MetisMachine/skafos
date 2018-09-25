#include "file/file.h"
#include "organization.h"

using namespace json11;

void Organization::list() {
  RestClient::Response resp = Request::my_organizations();
  std::string err;
  Json json = Json::parse(resp.body, err);

  int status_code = resp.code;
  std::string error_message = json["message"].string_value();

  if (status_code != 200) {
    console::error("There was an error listing your organizations: " + std::to_string(status_code) + " - " + error_message + "\n");
  } else if (json.is_array()) {
    console::info("Your organizations: ");
    auto orgs = json.array_items();
    for (int i = 0; i < orgs.size(); i++) {
      console::info("   " + orgs[i]["display_name"].string_value());
    }
  } else {
    console::error("There was an error listing your organizations \n");
  }
}

void Organization::set_default(std::string org_name) {
  RestClient::Response resp = Request::set_default_org(org_name);
  std::string err;
  Json json = Json::parse(resp.body, err);

  int status_code = resp.code;
  std::string error_message = json["message"].string_value();

  if (status_code != 201) {
    console::error("There was an error setting your default organization: " + std::to_string(status_code) + " - " + error_message + "\n");
  } else {
    Env::instance()->write_default_org(org_name);
    console::info(org_name + " is now your default organization");
  }
}

