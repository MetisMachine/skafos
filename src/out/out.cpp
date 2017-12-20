#include "out.h"

using namespace std;

namespace Out {

  void loading(string message) {

    list<string> frames = SPIN_DOTS;

    int frame_index = 0;
    
    list<string>::iterator it = frames.begin();

    while(true) {
      if(it == frames.end()) {
        it = frames.begin();

        continue;  
      }

      string frame = *it;

      cout << "\r" << frame << " " << message << flush;
      usleep(useconds_t(0.08 * 1000000));

      advance(it, 1);
    }
  }
}
