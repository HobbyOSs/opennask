#ifndef _LOADABLE_H_
#define _LOADABLE_H_

class {
  public:
    int isLoaded() const = 0;
    void Load() = 0;
  private:
    bool loaded;
};

#endif //!_LOADABLE_H_