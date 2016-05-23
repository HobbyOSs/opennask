#ifndef _ABSOLUTE_H_
#define _ABSOLUTE_H_

#include "match.h"

class Absolute : public Match {
  public:
    ~Absolute();
    int minSize() const;
    int maxSize() const;
    string Type match(string text, string &result);
  private:
    string correct;    
};

#endif //!_ABSOLUTE_H_