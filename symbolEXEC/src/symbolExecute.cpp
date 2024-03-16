//
// Created by star on 2024/3/15.
//

#include <fstream>
#include <iostream>
#include "func.h"


int cfgtest() {
//    std::cout << "libCFG" << std::endl;

    std::ifstream file("C:\\Users\\star\\Desktop\\symbol_exec\\symex-x86-stack", std::ios::binary); // 以二进制模式打开文件

    if (!file.is_open()) { // 检查文件是否成功打开
        std::cerr << "Unable to open file" << std::endl;
        return 1;
    }

    file.seekg(0x1060, std::ios::beg);

    // 分配内存，读取文件内容
    char* buffer = new char[0x295];
    file.read(buffer, 0x295); // 读取字节到缓冲区


    // 关闭文件
    file.close();



    std::string func_name="addx_y";
    uint64_t low_pc=0x1060;
    uint64_t high_pc=0x295;
    libCFG::func f(func_name,low_pc,high_pc);


    f.insn_buffer=buffer;
    f.parse();
    f.constructCFG(f.num_of_insn);








    return 0;




}

int main(){
    cfgtest();
}