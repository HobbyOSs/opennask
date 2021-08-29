 #pragma once
 #include <string>
 #include <stdexcept>


 class parse_error : public std::runtime_error
 {
 public:
     parse_error(int line, std::string str)
         : std::runtime_error(str)
         , m_line(line) {}
     int getLine() {
         return m_line;
     }
 private:
     int m_line;
 };

