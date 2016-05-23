#ifndef _LIST_H_
#define _LIST_H_

#include "match.h"
#include "absolute.h"
#include "loadable.h"

class List : public Match, public Loadable {
  public:
    ~List();
    int minSize() const;
    int maxSize() const;
    string Type match(string text, string &result);
  private:
    slist<Absolute> values;
    int min_length;
    int max_length;
};

#endif //!_LIST_H_