#include "logs.h"
#include "sse.h"
#include "http.h"
#include "env/env.h"
#include "common.h"

using namespace json11;

DEFINE_OBJECT(Options, options);

void sse_event(const char* data) {
  std::string err;
  Json json = Json::parse(data, err);
  if(err.length() > 0) {
    // Ignoring. Below is one possible solution if we choose to do it.
    //std::cerr << "[SKAFOS] Unable to parse log message!: " << err << std::endl;
    return;
  }

  std::string message = json["data"].string_value();
  std::cout << message << std::endl;
}

static const char* verify_response(CURL* curl) {
  #define EXPECTED_CONTENT_TYPE "text/event-stream"

  static const char expected_content_type[] = EXPECTED_CONTENT_TYPE;

  char* content_type;
  curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &content_type);
  if(!content_type) {
    content_type = (char*)std::string("").c_str();
  }

  if(!strncmp(content_type, expected_content_type, strlen(expected_content_type)))
    return 0;

  return "Invalid content_type, should be '" EXPECTED_CONTENT_TYPE "'.";
}

static size_t on_data(char *ptr, size_t size, size_t nmemb, void *userdata)
{
  parse_sse(ptr, size * nmemb);

  return size * nmemb;
}

void Logs::print(std::string project, long num, bool tail) {
  VERIFY_AUTH();

  handle_sse_event = sse_event;
  std::string auth = std::string("x-api-token: ") + Env::instance()->get(METIS_API_TOKEN);
  const char* headers[] = {
    "Accept: text/event-stream",
    auth.c_str(),
    NULL
  };

  std::string logs_url = "";
  const char* env = getenv("ENV");

  if(env == NULL) {
	  logs_url = "https://api.metismachine.io/logs/" + project;
  } else if(strcmp(env, "local") == 0) {
    std::cout << "Using local environment!" << std::endl;

    logs_url = "http://localhost:4000/logs/" + project;
  } else if(strcmp(env, "dev") == 0) {
    std::cout << "Using staging environment!" << std::endl;
    logs_url = "http://argus.metis.wtf/logs/" + project;
  }

  http(HTTP_GET, (char*)logs_url.c_str(), headers, 0, 0, on_data, verify_response);
}
