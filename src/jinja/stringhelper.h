#ifndef __CLI_STRINGHELPER__
#define __CLI_STRINGHELPER__

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <cstdlib>

class IHasToString {
public:
    virtual std::string toString() = 0;
};


template<typename T> std::string toString(T val ) {
   std::ostringstream myostringstream;
   myostringstream << val;
   return myostringstream.str();
}

std::vector<std::string> split(const std::string &str, const std::string &separator = " " );
std::string trim( const std::string &target );

inline float atof( std::string stringvalue ) {
   return (float)std::atof(stringvalue.c_str());
}
inline int atoi( std::string stringvalue ) {
   return std::atoi(stringvalue.c_str());
}

inline std::string getToken( std::string targetstring, int tokenIndexFromZero, std::string separator = " " ) {
   std::vector<std::string> splitstring = split( targetstring, separator );
   if( tokenIndexFromZero < (int)splitstring.size() ) {
      return splitstring[tokenIndexFromZero];
   } else {
      return "";
   }
}

std::string replace( std::string targetString, std::string oldValue, std::string newValue );
std::string replaceGlobal( std::string targetString, std::string oldValue, std::string newValue );

std::string toLower(std::string in );

void strcpy_safe( char *destination, char const*source, int maxLength );

#endif
