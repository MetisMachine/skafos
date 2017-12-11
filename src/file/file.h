#pragma once

#include "common.h"

class FileManager {
public:
  static int create_path(mode_t mode, const std::string& rootPath, std::string& path);
  static void write(std::string path, std::string content);
  static std::string read(std::string path);
  static bool file_exists(std::string path);
};
