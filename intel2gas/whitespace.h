#ifndef _WHITESPACE_H_
#define _WHITESPACE_H_

#include "match.h"

class Whitespace : public Match{
  public:
    ~Whitespace();
    int minSize() const;
    int maxSize() const;
    string Type match(string text, string &result);
};

#endif //!_WHITESPACE_H_