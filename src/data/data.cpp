#include <typeinfo>
#include "data.h"
#include "request/request.h"
#include "project_env/project_env.h"

using namespace std;
using namespace json11;

void Data::fetch(string table) {
  console::info("\nFetching records from " + table + "\n");

  RestClient::Response response = Request::fetch(PROJECT_TOKEN, table);

  string err;
  Json json = Json::parse(response.body, err);
  auto list = json["data"].array_items();

  if(list.empty()) {
    console::info("Fetch results were empty");
    
    exit(EXIT_SUCCESS);
  }

  for(auto i : list) {
    console::info(Json(i).dump());
  }

  console::info("\nCURL Request:");
  string curl_cmd = "curl -D - -H \"X-API-Token: " + Env::instance()->get(METIS_API_TOKEN) + "\" -H \"Accept: application/json\" http://api.metismachine.io/v1/data/8e406744fb8f3c9d7d19fa03/" + table;
  console::info(curl_cmd);
  
}
