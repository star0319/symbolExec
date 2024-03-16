#ifndef LIBCFG_UTILS_H
#define LIBCFG_UTILS_H

#include <cstdint>
#include <sstream>
#include <string>

namespace libCFG {
    template<typename T>
    std::string dec_to_hex(T dec) {
        std::string res;
        std::stringstream ss;
        ss << std::hex << dec;
        ss >> res;
        return res;
    }

    template<typename T>
    std::string dec_to_hex(uint8_t dec) {
        std::string res;
        std::stringstream ss;
        ss << std::hex << static_cast<uint16_t>(dec);
        ss >> res;
        return res;
    }

    bool isHexadecimal(const std::string& str);
    uint64_t hexStringToUInt(const std::string& hexStr);
}

#endif