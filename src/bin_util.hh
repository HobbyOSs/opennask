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

class BinUtil {

public:
    const static size_t NASK_BYTE = 1;
    const static size_t NASK_WORD = 2;
    const static size_t NASK_DWORD = 4;

    const std::array<uint8_t, NASK_BYTE> IntAsByte(const int);
    const std::array<uint8_t, NASK_WORD> IntAsWord(const int);
    const std::array<uint8_t, NASK_DWORD> LongAsDword(const long);

    void put4b(std::string&, size_t, const int);

    std::string to_upper(const std::string& in) const;
    std::string to_lower(const std::string& in) const;

    template<typename T>
    const std::string int_to_hex(T i);
    const std::string bytes_to_hex(std::vector<uint8_t>&);
    const std::string join(std::vector<std::string>&, const std::string& = "");
    const std::vector<std::string> split(const std::string&, char);

    template<typename T>
    std::vector<T> slice(std::vector<T> const &v, int m, int n) {
        auto first = v.cbegin() + m;
        auto last = v.cbegin() + n + 1;

        std::vector<T> vec(first, last);
        return vec;
    };
};

#endif // ! BIN_UTIL_HH
