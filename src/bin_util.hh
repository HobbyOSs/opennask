#ifndef BIN_UTIL_HH
#define BIN_UTIL_HH

#include <string>
#include <vector>
#include <array>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>

template<typename T>
std::vector<T> slice(std::vector<T> const &v, int m, int n) {
    auto first = v.cbegin() + m;
    auto last = v.cbegin() + n + 1;

    std::vector<T> vec(first, last);
    return vec;
};

class BinUtil {

public:
    const static size_t NASK_BYTE = 1;
    const static size_t NASK_WORD = 2;
    const static size_t NASK_DWORD = 4;

    const std::array<uint8_t, NASK_BYTE> IntAsByte(const int);
    const std::array<uint8_t, NASK_WORD> IntAsWord(const int);
    const std::array<uint8_t, NASK_DWORD> LongAsDword(const long);

    const std::string bytes_to_hex(std::vector<uint8_t>& bytes);
    const std::string join(std::vector<std::string>&, const std::string& = "");
};

#endif // ! BIN_UTIL_HH