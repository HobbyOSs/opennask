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
