#include "string_util.hpp"
#include "mod_rm.hpp"
#include "tinyexpr.h"
#include "nask_defs.hpp"

namespace nask_utility {

     bool is_legitimate_numeric(const std::string& s) {
	  return is_hex_notation(s) || is_integer(s);
     }

     bool is_between_bytesize(const long l) {
	  return SCHAR_MIN <= l && l <= SCHAR_MAX;
     }

     bool is_imm8(const std::string& token) {
	  if (is_hex_notation(token)) {
	       return imm8 == get_imm_size(token);
	  } else {
	       if (is_integer(token)) {
		    log()->info("is imm8 ? {}", token);
		    return is_between_bytesize(std::stol(token));
	       }
	  }
	  return true;
     }

     bool is_contains_math_op(const std::string& subject) {
	  try {
	       std::regex re("[^0-9]*([-\\*/+0-9]*)[^0-9]*");
	       std::smatch match;
	       if (std::regex_search(subject, match, re) && match.size() > 1) {
		    std::string matched = match[1].str();
		    // もっと良い書き方はないものか…
		    for (std::string op : PRE_PROCESS_OPERATORS) {
			 if (contains(match[1].str(), op)) {
			      return true;
			 }
		    }
	       }
	  } catch (std::regex_error& e) {
	       log()->info(e.what());
	  }
	  return false;
     }

     std::string expr_math_op(const std::string& subject) {
	  try {
	       std::regex re("([^0-9]*)([-\\*/+0-9]*)([^0-9]*)");
	       std::smatch match;
	       if (std::regex_search(subject, match, re) && match.size() > 1) {
		    int error;
		    const int process = te_interp(match[2].str().c_str(), &error);
		    const std::string cat = match[1].str() + match[2].str() + match[3].str();
		    const std::string empty = "";

		    std::string result = replace(subject, cat, empty);
		    return match[1].str() + std::to_string(process) + match[3].str() + result;
	       }

	  } catch (std::regex_error& e) {
	       log()->info(e.what());
	  }
	  return "";
     }

     bool is_integer(const std::string& s) {
	  if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;
	  char * p ;
	  strtol(s.c_str(), &p, 10) ;
	  return (*p == 0);
     }

     bool is_hex_notation(const std::string& s) {
	  return s.compare(0, 2, "0x") == 0
	       && s.size() > 2
	       && s.find_first_not_of("0123456789abcdefABCDEF", 2) == std::string::npos;
     }

     size_t get_imm_size(const std::string& hex_string) {
	  if (is_hex_notation(hex_string)) {
	       const size_t s = hex_string.substr(2).size() / 2;
	       log()->info("check imm size: {} => {}", hex_string, s);
	       return s;
	  } else {
	       return 0;
	  }
     }

     std::ifstream::pos_type filesize(const char* filename) {
	  std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
	  return in.tellg();
     }

     std::vector<std::string> split(const std::string &str, char delim) {
	  std::istringstream iss(str); std::string tmp; std::vector<std::string> res;
	  while(std::getline(iss, tmp, delim)) res.push_back(tmp);
	  return res;
     }

     const std::string replace(const std::string& in, const std::string& from, const std::string& to) {
	  std::string str = std::string(in);
	  std::string::size_type pos = 0;
	  while (pos = str.find(from, pos), pos != std::string::npos) {
	       str.replace(pos, from.length(), to);
	       pos += to.length();
	  }
	  const auto s = std::string(str);
	  return s;
     }

     bool contains(const std::string& src, const std::string& query) {
	  return src.find(query) != std::string::npos;
     }

     bool ends_with(std::string const &full_string, std::string const &ending) {
	  if (full_string.length() >= ending.length()) {
	       return (0 == full_string.compare(full_string.length() - ending.length(), ending.length(), ending));
	  } else {
	       return false;
	  }
     }

     bool starts_with(std::string const &full_string, std::string const &begining) {
	  log()->info("starts_with ? {} & {}", begining, full_string.substr(0, begining.size()));
	  return full_string.substr(0, begining.size()) == begining;
     }

     bool equals_ignore_case(const std::string& left, const std::string& right) {
	  const size_t sz = left.size();
	  if (right.size() != sz) {
	       return false;
	  }
	  for (size_t i = 0; i < sz; ++i) {
	       if (tolower(left[i]) != tolower(right[i])) {
		    return false;
	       }
	  }
	  return true;
     }
};
