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
void Models::download(std::string project_token, std::map<std::string, std::string> params, std::string output_path){
  if(project_token.compare(".") == 0){
    project_token = PROJECT_TOKEN;
  }

  RestClient::Response resp = Request::list_models(project_token, params);
  std::string err;
  Json json = Json::parse(resp.body, err);

  int status_code = resp.code;
  std::string error_message = json["message"].string_value();

  Json selected_model;

  if (not(Helpers::success(status_code))) {
    console::error("There was an error retreiving your model for download: " + std::to_string(status_code) + " " + error_message + "\n");
  } else if (json.is_array()) {
    auto models = json.array_items();
    string list_size = std::to_string(models.size());
    
    if(stoi(list_size) == 1){
      selected_model = models[0];
    } else{
      cout << "Please select the model you would like to download." << endl;
      int opt_iter = 1; 
      for(int i = 0; i < models.size(); i++){
        Json tags = models[i]["tags"];
        std::string model_tags = tags_to_string(tags);

        cout << std::to_string(opt_iter) << ". " << models[i]["display_name"].string_value() << endl;
        cout << "Version: " + models[i]["version"].string_value() << endl;
        cout << "Tags: " + model_tags << endl;
        cout << endl;
        opt_iter++;
      } 
      cout << "Enter your choice and press return: ";
      string opt_select;
      cin >> opt_select;
      auto idx = stoi(opt_select) - 1;
      if(idx >= models.size()) {
        cin.clear();
        cin.ignore(1, '\n');
        cout << "Invalid choice. Valid numbers are [1-" << list_size << "].";
        return;
          } else {
        selected_model = models[idx];
          }
        }
    std::string display_name = selected_model["display_name"].string_value();
    std::string version = selected_model["version"].string_value();

    std::string model_url = DOWNLOAD_URL + "/projects/" + project_token + "/models?name=" + display_name + "&version=" + version;

    std::string output_file = version + "_" + display_name + ".txt";
    std::string download_path;
        
    if (output_path.compare(".") == 0){
      download_path = FileManager::cwd() + "/" + output_file;
    } else {
      download_path = output_path + "/" + output_file;
      }

    Request::download(model_url, download_path);

  } else {
    console::error("There was an error downloading your model \n");
  }
    }
  } else {
    console::error("There was an error listing your models \n");
  }
}