#pragma once

#include <algorithm> 
#include <cctype>
#include <locale>

static inline void ltrim(std::string &s);
static inline void rtrim(std::string &s);
static inline void trim(std::string &s);
static inline std::string ltrim_copy(std::string s);
static inline std::string rtrim_copy(std::string s);
static inline std::string trim_copy(std::string s);
