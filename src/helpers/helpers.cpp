#include "helpers.h"

bool Helpers::success(int status_code) {
  bool success = true;
  for(unsigned int i=0; i<SUCCESS_CODES.size(); i++){
    if (status_code == SUCCESS_CODES[i]){
      return true;
    }
  }
  return false;
}