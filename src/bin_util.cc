#include "bin_util.hh"

const std::array<uint8_t, 1> BinUtil::IntAsByte(const int v) {
    return std::array<uint8_t, 1>{static_cast<uint8_t>(v)};
}

const std::array<uint8_t, 2> BinUtil::IntAsWord(const int word) {
    return std::array<uint8_t, 2>{
        static_cast<uint8_t>( word & 0xff ),
        static_cast<uint8_t>( (word >> 8) & 0xff ),
    };
}

const std::array<uint8_t, 4> BinUtil::LongAsDword(const long dword) {
    return std::array<uint8_t, 4>{
        static_cast<uint8_t>( dword & 0xff ),
        static_cast<uint8_t>( (dword >> 8)  & 0xff ),
        static_cast<uint8_t>( (dword >> 16) & 0xff ),
        static_cast<uint8_t>( (dword >> 24) & 0xff ),
    };
}

std::string BinUtil::to_upper(const std::string &in) const {
    std::string s(in);
    std::transform(s.begin(),
                   s.end(),
                   s.begin(),
                   [](unsigned char const &c) {
                       return ::toupper(c);
                   });
    return s;
}

std::string BinUtil::to_lower(const std::string &in) const {
    std::string s(in);
    std::transform(s.begin(),
                   s.end(),
                   s.begin(),
                   [](unsigned char const &c) {
                       return ::tolower(c);
                   });
    return s;
}

void BinUtil::put4b(std::string& output, size_t from, const int value) {
    output[from+0] =  value & 0xff;
    output[from+1] = (value >> 8)  & 0xff;
    output[from+2] = (value >> 16) & 0xff;
    output[from+3] = (value >> 24) & 0xff;
}

template<typename T>
const std::string BinUtil::int_to_hex(T i) {
    std::stringstream stream;
    stream << "0x"
           << std::setfill ('0') << std::setw(sizeof(T)*2)
           << std::hex << i;
    return stream.str();
}

template const std::string BinUtil::int_to_hex(int8_t i);
template const std::string BinUtil::int_to_hex(int16_t i);
template const std::string BinUtil::int_to_hex(int32_t i);
template const std::string BinUtil::int_to_hex(uint8_t i);
template const std::string BinUtil::int_to_hex(uint16_t i);
template const std::string BinUtil::int_to_hex(uint32_t i);

const std::string BinUtil::bytes_to_hex(std::vector<uint8_t>& bytes) {
    std::vector<std::string> debug_args = {};
    std::transform(bytes.begin(), bytes.end(),
                   std::back_inserter(debug_args),
                   [](uint8_t byte) {
                       std::ostringstream hex;
                       hex << "0x"
                           << std::setfill('0')
                           << std::setw(2)
                           << std::hex
                           << static_cast<int>(byte);
                       return hex.str();
                   });
    return this->join(debug_args, ",");
}

const std::string BinUtil::join(std::vector<std::string>& array, const std::string& sep) {

    std::stringstream ss;
    for(size_t i = 0; i < array.size(); ++i) {
        if(i != 0) {
            ss << sep;
        }
        ss << array[i];
    }
    return ss.str();
}

const std::vector<std::string> BinUtil::split(const std::string &s, char delim) {

    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        if (!item.empty()) {
            elems.push_back(item);
        }
    }
    return elems;
}
