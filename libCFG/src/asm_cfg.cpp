#include <iostream>
#include "func.h"
#include <fstream>
#include <vector>



int cfgtest() {
//    std::cout << "libCFG" << std::endl;

    std::ifstream file("../../symex-x86-stack", std::ios::binary); // 以二进制模式打开文件

    if (!file.is_open()) { // 检查文件是否成功打开
        std::cerr << "Unable to open file" << std::endl;
        return 1;
    }

    file.seekg(0x1060, std::ios::beg);

    // 分配内存，读取文件内容
    char* buffer = new char[0x295];
    file.read(buffer, 0x295); // 读取字节到缓冲区

//    // 检查实际读取了多少字节
//    std::streamsize bytesRead = file.gcount();
//    if (file.fail()) { // 读取失败，可能是因为文件大小小于请求的字节数
//        std::cerr << "Failed to read the entire buffer" << std::endl;
//    } else {
//        // 处理读取到的字节
//        for (std::streamsize i = 0; i < bytesRead; ++i) {
//            std::cout << std::hex << static_cast<int>(buffer[i]) << ' ';
//        }
//        std::cout << std::endl;
//    }

    // 关闭文件
    file.close();





    std::string func_name="_Z3fooii";
    uint64_t low_pc=0x1060;
    uint64_t high_pc=0x295;
    libCFG::func f(func_name,low_pc,high_pc);


    f.insn_buffer=buffer;
    f.parse();
    f.constructCFG(f.num_of_insn);
//    std::cout<<f.instructions<<std::endl;
    const char *outputFile="../../CFG/addx_y_linux.dot";
    freopen(outputFile,"w",stdout);
    f.cfg->to_dot();





    return 0;




}

int main(){
    cfgtest();
}



//
//
//int capstonetest(){
//    csh handle;
//    cs_err err;
//    const uint8_t code[] = { /* ... your machine code here ... */ };
//    size_t code_size = sizeof(code);
//    cs_insn *insn;
//    size_t count;
//
//    // Initialize Capstone
//    if (cs_open(CS_ARCH_X86, CS_MODE_64, &handle) != CS_ERR_OK) {
//        std::cerr << "Failed to initialize Capstone" << std::endl;
//        return 1;
//    }
//
//    // Disassemble code
//    count = cs_disasm(handle, code, code_size, 0x1000, 0, &insn);
//    if (count > 0) {
//        // Print disassembled instructions
//        for (size_t i = 0; i < count; i++) {
//            std::cout << insn[i].mnemonic << "\t" << insn[i].op_str << "\n";
//        }
//
//        // Free memory allocated by cs_disasm
//        cs_free(insn, count);
//    } else {
//        std::cerr << "Failed to disassemble code" << std::endl;
//    }
//
//    // Close Capstone handle
//    cs_close(&handle);
//
//    return 0;
//
//}