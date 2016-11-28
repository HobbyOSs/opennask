#ifndef STRING_UTIL_HPP_
#define STRING_UTIL_HPP_

#include <fstream>
#include <sstream>
#include <algorithm>
#include <functional>
#include "nask_defs.hpp"

namespace nask_utility {

     // Javaにはありがちな基本的文字列処理
     std::ifstream::pos_type filesize(const char* filename);
     std::vector<std::string> split(const std::string &str, char delim);
     const std::string replace(const std::string& in, const std::string& from, const std::string& to);
     bool contains(const std::string& src, const std::string& query);
     bool ends_with(std::string const &full_string, std::string const &ending);
     bool starts_with(std::string const &full_string, std::string const &begining);
     bool equals_ignore_case(const std::string& left, const std::string& right);
     std::string trim(const std::string& string, const char* trim_character_list = " \t\v\r\n");

     // RubyのArray#join
     const std::string join(std::vector<std::string>& array, const std::string& sep = "");
     const std::string string_to_hex(const std::string& input, bool no_notate = false);
     const std::string string_to_hex_no_notate(const std::string& input);

     // アセンブラ処理で使う判定系処理
     bool is_hex_notation(const std::string& s);
     bool is_integer(const std::string& s);
     bool is_legitimate_numeric(const std::string& s);
     bool is_between_bytesize(const long l);
     bool is_imm8(const std::string& token);
     bool is_contains_math_op(const std::string& subject);
     std::string expr_math_op(const std::string& subject);

     size_t get_imm_size(const std::string& hex_string);
};

#endif /* STRING_UTIL_HPP_ */
