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
      Json tags = models[i]["tags"];
      std::string model_tags = tags_to_string(tags);

      console::info("  Name: " + models[i]["display_name"].string_value() + 
                    "\n    Version: " + models[i]["version"].string_value() + 
                    "\n    Tags: " + model_tags);
    }
  } else {
    console::error("There was an error listing your models \n");
  }
}

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

    Models::download_to_file(project_token, display_name, version, output_path);

  } else {
    console::error("There was an error downloading your model \n");
  }
}

std::string Models::tags_to_string(Json tags){
  auto tags_list = tags.array_items();
  std::string model_tags;
  if (tags_list.size() == 0){
    model_tags = "nil";
  } else {
    for (int j = 0; j < tags_list.size(); j++) {
      if(j == tags_list.size() - 1){
        model_tags = model_tags + " " + tags_list[j].string_value();
      } else {
        model_tags = model_tags + " " + tags_list[j].string_value() + ",";
      }
    }
  }
  return model_tags;
}

void Models::download_to_file(std::string project_token, std::string display_name, std::string version, std::string output_path){

    std::string model_url = DOWNLOAD_URL + "/projects/" + project_token + "/models?name=" + display_name + "&version=" + version;

    std::string output_file = display_name;
    std::string download_path;
    
    try {
      download_path = resolve_download_path(output_path, output_file);
      console::debug("Download path: " + download_path);
      Request::download(model_url, download_path);

      YAML::Node model_error = YAML::LoadFile(download_path);
      if (model_error["Error"]){
        console::error("There was an error downloading your model to " + download_path + ": " + model_error["Error"].as<std::string>());
      } else{
        console::error("There was an error downloading your model to " + download_path);
      }
      FileManager::delete_file(download_path);
    } catch (YAML::ParserException& e){
      console::info("Model successfully downloaded to " + download_path);
    } catch(std::string &error) {
      console::error("Error: " + error);
    } catch (...) {
      console::error("There was an error downloading your model to " + download_path);
    }
}

std::string Models::resolve_download_path(std::string output_path, std::string output_file) {
  std::string download_path;

  if (FileManager::is_dir(output_path)){
    char ch = output_path.back();
    if (ch == '/') {
      download_path = output_path + output_file;
    } else {
      download_path = output_path + "/" + output_file;
    }
  } else {
    std::size_t found = output_path.rfind("/");
    if (found == std::string::npos) {
      download_path = FileManager::resolve_path(output_path);
    } else {
      string dir = output_path.substr(0, found);
      if (FileManager::is_dir(dir)){
        download_path = output_path;
      } else {
        throw "Directory does not exist: " + dir;
      }
    }
  }
  return download_path;
}
