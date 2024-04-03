//
// Created by shengxin on 2024/3/31.
//

#ifndef SYMBOL_EXEC_TOOLUTIL_H
#define SYMBOL_EXEC_TOOLUTIL_H
#include <iostream>
#include <vector>
std::vector<std::string> split(std::string str, std::string pattern)
{
//    std::cout<<str;
    std::string::size_type pos;
    std::vector<std::string> result;
    str += pattern;//扩展字符串以方便操作
    int size = str.size();
    for (int i = 0; i < size; i++)
    {
        pos = str.find(pattern, i);
        if (pos < size)
        {
            std::string s = str.substr(i, pos - i);
            result.push_back(s);
            i = pos + pattern.size() - 1;
        }
    }
    return result;
}

void printInsn(bool print,cs_insn insn){
    if(!print) return;
    std::cout << "0x" << libCFG:: dec_to_hex(insn.address) << ":   " << insn.mnemonic << "  " << insn.op_str<<std::endl;
}
void printPassedInsn(bool print,cs_insn insn){
    if(!print) return;
    std::cout << " passed*   *******              0x" << libCFG:: dec_to_hex(insn.address) << ":   " << insn.mnemonic << "  " << insn.op_str<<std::endl;
}


#endif //SYMBOL_EXEC_TOOLUTIL_H
