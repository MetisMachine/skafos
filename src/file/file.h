#ifndef __CLI_FILE__
#define __CLI_FILE__

#include "common.h"

class FileManager {
public:
  static std::string cwd();
  static std::string resolve_path(std::string path);
  static bool create_path(mode_t mode, const std::string& rootPath, std::string& path);
  static bool create_path(mode_t mode, std::string path);
  static void write(std::string path, std::string content);
  static std::string read(std::string path);
  static bool file_exists(std::string path);
  static bool dir_exists(std::string path);
  static bool is_dir(std::string path);
  static bool is_dot_dir(std::string path);
  static void delete_dir(std::string path);
  static std::list<std::string> dir_list(std::string path, std::string extension = "");
  static bool unzip(std::string source, std::string destination);

private:
  static int copy_data(struct archive *ar, struct archive *aw);
};

#endif
