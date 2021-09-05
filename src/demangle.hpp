#ifndef DEMANGLE_HPP_
#define DEMANGLE_HPP_

#include <string>
#include <typeinfo>

std::string demangle(const char* name);

template <class T>
std::string type(const T& t) {
    return demangle(typeid(t).name());
}

#endif /* DEMANGLE_HPP_ */
