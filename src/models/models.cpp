#include "models.h"

void Models::list(std::string project_token, std::map<std::string, std::string> params){
  if(project_token.compare(".") == 0){
    project_token = PROJECT_TOKEN;
  }

  RestClient::Response resp = Request::list_models(project_token, params);
  std::string err;
  Json json = Json::parse(resp.body, err);

  int status_code = resp.code;
  std::string error_message = json["message"].string_value();

   if (not(Helpers::success(status_code))) {
    console::error("There was an error listing your models: " + std::to_string(status_code) + " " + error_message + "\n");
  } else if (json.is_array()) {
    console::info("Your models: ");
    auto models = json.array_items();
    for (int i = 0; i < models.size(); i++) {
      auto tags = models[i]["tags"].array_items();
      std::string model_tags;
      if (tags.size() == 0){
        model_tags = "nil";
      } else {
        for (int j = 0; j < tags.size(); j++) {
          if(j == tags.size() - 1){
            model_tags = model_tags + " " + tags[j].string_value();
          } else {
            model_tags = model_tags + " " + tags[j].string_value() + ",";
          }
        }
        
      }

      console::info("  Name: " + models[i]["display_name"].string_value() + 
                    "\n    Version: " + models[i]["version"].string_value() + 
                    "\n    Tags: " + model_tags);
    }
  } else {
    console::error("There was an error listing your models \n");
  }
}