#include "string_util.hpp"
#include "mod_rm.hpp"
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
            bool t = (ModRM::get_imm_size(token) == imm8) ? true : false;
            log()->info("{} is imm8 ? : {}", token, t);
            return t;
        } else {
            if (is_integer(token)) {
                bool t = is_between_bytesize(std::stol(token));
                log()->info("{} is imm8 ? : {}", token, t);
                return t;
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

    std::string replace_hex2dec(const std::string& subject) {

        const std::regex reg_hex("(.*)(0x[0-9]*)(.*)");

        try {
            std::smatch match;
            std::string tmp = subject;

            while (std::regex_search(tmp, match, reg_hex) && match.size() > 1) {
                const std::string hex = std::to_string(
                    std::strtol(match[2].str().c_str(), NULL, 16)
                );
                tmp = match[1].str();
                tmp += hex;
                tmp += match[3].str();
            }
            return tmp;

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
        log()->debug("starts_with ? {} & {}", begining, full_string.substr(0, begining.size()));
        return full_string.substr(0, begining.size()) == begining;
    }

    bool equals_ignore_case(const std::string& left, const std::string& right) {
        if (right.length() != left.length()) {
            return false;
        }
        for (size_t i = 0; i < right.length(); ++i) {
            if (std::isalpha(left[i]) && std::isalpha(right[i])) {
                // if both chars are alphabet, exec compare
                log()->debug("compare {} vs {}", left[i], right[i]);
                if (tolower(left[i]) != tolower(right[i])) {
                    return false;
                }
            } else {
                // else simply compare
                if (left[i] != right[i]) {
                    log()->debug("compare {} vs {}", left[i], right[i]);
                    return false;
                }
            }
        }
        return true;
    }

    std::string trim(const std::string& string, const char* trim_character_list) {
        std::string result;
        std::string::size_type left = string.find_first_not_of(trim_character_list);

        if (left != std::string::npos) {
            std::string::size_type right = string.find_last_not_of(trim_character_list);
            result = string.substr(left, right - left + 1);
        }

        return result;
    }

    const std::string join(std::vector<std::string>& array, const std::string& sep) {
        std::stringstream ss;
        for(size_t i = 0; i < array.size(); ++i) {
            if(i != 0) {
                ss << sep;
            }
            ss << array[i];
        }
        return ss.str();
    }

    const std::string string_to_hex(const std::string& input, bool no_notate) {
        static const char* const lut = "0123456789ABCDEF";
        const size_t len = input.length();
        const size_t reserv_len = no_notate ? 2 : 5;
        std::string output;
        output.reserve(reserv_len * len);
        for (size_t i = 0; i < len; ++i) {
            const unsigned char c = input[i];
            if (no_notate) {
	            output.push_back(lut[c >> 4]);
	            output.push_back(lut[c & 15]);
            } else {
	            output.push_back('0');
	            output.push_back('x');
	            output.push_back(lut[c >> 4]);
	            output.push_back(lut[c & 15]);
	            output.push_back(' ');
            }
        }
        return output;
    }

    const std::string string_to_hex_no_notate(const std::string& input) {
        return string_to_hex(input, true);
    }
};
