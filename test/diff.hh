#ifndef DIFF_HH
#define DIFF_HH

#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <sstream>
#include <fstream>
#include <gtest/gtest.h>
#include "diff-match-patch-cpp-stl/diff_match_patch.h"

#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define CYN "\e[0;36m"

using dmp = diff_match_patch<std::string>;
using traits = diff_match_patch_traits<typename std::string::value_type>;

std::string diff_pretty_ansi(const dmp::Diffs &diffs) {
    std::string ansi;
    std::string text;
    for (typename dmp::Diffs::const_iterator cur_diff = diffs.begin(); cur_diff != diffs.end(); ++cur_diff) {
        typename std::string::size_type n = (*cur_diff).text.size();
        typename std::string::const_pointer p, end;
        for (p = (*cur_diff).text.c_str(), end = p + n; p != end; ++p)
            switch (traits::to_wchar(*p)) {
            case L'&': n += 4; break;
            case L'<':
            case L'>': n += 3; break;
            case L'\n': n += 9; break;
            }
        if (n == (*cur_diff).text.size())
            text = (*cur_diff).text;
        else {
            text.clear();
            text.reserve(n);
            for (p = (*cur_diff).text.c_str(); p != end; ++p)
                switch (traits::to_wchar(*p)) {
                case L'&': text += traits::cs(L"&amp;"); break;
                case L'<': text += traits::cs(L"&lt;"); break;
                case L'>': text += traits::cs(L"&gt;"); break;
                case L'\n': text += traits::cs(L"\n"); break;
                default: text += *p;
                }
        }
        switch ((*cur_diff).operation) {
        case dmp::INSERT:
            //ansi += traits::cs(L"<ins style=\"background:#e6ffe6;\">") + text + traits::cs(L"</ins>");
            ansi += GRN + text;
            break;
        case dmp::DELETE:
            //ansi += traits::cs(L"<del style=\"background:#ffe6e6;\">") + text + traits::cs(L"</del>");
            ansi += RED + text;
            break;
        case dmp::EQUAL:
            //ansi += traits::cs(L"<span>") + text + traits::cs(L"</span>");
            ansi += CYN + text;
            break;
        }
    }
    return ansi;
}

std::string diff(std::vector<uint8_t> expected, std::vector<uint8_t> actual) {

    dmp dmp_ins;
    std::stringstream oss1;
    std::stringstream oss2;
    for (int i = 0; i < expected.size(); i++) {
        if (i != 0 && i % 16 == 0) {
            oss1 << '\n';
        }
        auto b = expected.at(i);
        oss1 << std::setfill('0')
             << std::setw(2)
             << std::hex
             << static_cast<int>(b) << ' ';
    }
    for (int i = 0; i < actual.size(); i++) {
        if (i != 0 && i % 16 == 0) {
            oss2 << '\n';
        }
        auto b = actual.at(i);
        oss2 << std::setfill('0')
             << std::setw(2)
             << std::hex
             << static_cast<int>(b) << ' ';
    }

    std::string out1 = oss1.str();
    std::string out2 = oss2.str();

    auto diffs = dmp_ins.diff_main(out1, out2);
    std::stringstream diff;
    return diff_pretty_ansi(diffs);
}

template<typename T>
std::vector<T> slice(std::vector<T> const &v, int m, int n) {
    auto first = v.cbegin() + m;
    auto last = v.cbegin() + n + 1;

    std::vector<T> vec(first, last);
    return vec;
};

// exprN: マクロに与えた式
// valN: マクロに与えられた式の値
::testing::AssertionResult checkTextF(const char* expected_name,
                                      const char* actual_name,
                                      const std::vector<uint8_t>& expected,
                                      const std::vector<uint8_t>& actual) {

    if(std::equal(expected.begin(), expected.end(), actual.begin()))
        return ::testing::AssertionSuccess();

    // 差分が有る場合でも最初の300byteまでしか出さない
    size_t slice_size = 300;
    if (expected.size() <= 300 && actual.size() <= 300) {
        // 出力が300byte以下だった場合は想定値とテスト結果のうち大きい方のサイズで差分を出す
        slice_size = std::max(expected.size(), actual.size());
    }
    auto expected_slice = slice(expected, 0, slice_size);
    auto actual_slice = slice(actual, 0, slice_size);

    const std::string msg = "[diff]\n" + diff(expected_slice, actual_slice);
    return ::testing::AssertionFailure(::testing::Message(msg.c_str()));
}

#endif // ! DIFF_HH
