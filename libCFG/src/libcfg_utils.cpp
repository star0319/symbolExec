#include "libcfg_utils.h"
#include "libcfg_error.h"

bool libCFG::isHexadecimal(const std::string& str) {
    if (str.empty())
        return false;

    if (str.size() < 2 || (str[0] != '0' || (str[1] != 'x' && str[1] != 'X')))
        return false;

    for (size_t i = 2; i < str.size(); ++i) {
        if (!isxdigit(static_cast<unsigned char>(str[i]))) {
            return false;
        }
    }

    return true;
}

uint64_t libCFG::hexStringToUInt(const std::string& hexStr) {
    if (!libCFG::isHexadecimal(hexStr))
        libcfg_error("\"", hexStr ,"\"", " is not a HEX number");

    return std::strtol(hexStr.c_str() + 2, nullptr, 16);
}