#ifndef _MATCH_H_
#define _MATCH_H_

#include <string>

class Match {
  public:
    enum Type {EXACT, BEST, NOTHING};
    virtual ~Match() const = 0;
    virtual int minSize() const = 0;
    virtual int maxSize() const = 0;
    virtual Type match(string text, string &result) = 0;
  private:
};

#endif //!_MATCH_H_