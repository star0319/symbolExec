//
// Created by shengxin on 2024/3/29.
//

#ifndef SYMBOL_EXEC_MYREGISTER_H
#define SYMBOL_EXEC_MYREGISTER_H

#include <iostream>
#include "func.h"
typedef enum x86_rflag{
    CF=0,
    PF=2,
    AF=4,
    ZF=6,
    SF=7,
    TF,
    IF,
    DF,
    OF,
    IOPL,  //12-13 两位
    NT=14,
    RF=16,
    VM,
    AC,
    VIF,
    VIP,
    ID
} x86_rflag;


class MyRegister{
private:
    static MyRegister* myRegister;
    MyRegister(){

    }
public:
    libCFG::func *f;
    std::map <x86_reg, uint64_t> registerMap;
    uint64_t RFlags=0x02;
    static MyRegister* GetRegister(){
        if (myRegister== nullptr){
            myRegister=new MyRegister();
        }
        return myRegister;

    }


    void set_f(libCFG::func *f){
        this->f= f;

    }


    void update(x86_reg reg ,uint64_t data){
        registerMap[reg]=data;
    }

    //TODO: get32   update32

//    // 打印寄存器状态
    void printRegister(bool print) const {
        if (!print) return;
        std::cout << "Register contents :" << std::endl;
        std::for_each(registerMap.begin(), registerMap.end(), [&](const auto &item) {
            std::cout<<"reg: " << cs_reg_name(f->handle,item.first) <<"  data: "<< std::hex << std::setw(16) << std::setfill('0')<<static_cast<int>(item.second)<< std::dec<<std::endl;
        });
    }
    void printRflags(bool print) const {
        if (!print) return;
        std::cout << "RFlags contents :" ;
        std::cout<<"  data: "<< std::hex << std::setw(16) << std::setfill('0')<<static_cast<int>(RFlags)<< std::dec<<std::endl;
    }



    void set_RFlags(x86_rflag flag ){
        uint64_t temp=1ULL<<flag;
        RFlags=RFlags|temp;
    }
    void unset_RFlags(x86_rflag flag){
        uint64_t temp= ~(1ULL<<flag);
        RFlags=RFlags&temp;
    }

    //true ->has set flag
    bool get_RFlags(x86_rflag flag){
        uint64_t temp=1ULL<<flag;
        temp=RFlags & temp;
        return temp != 0;
    }

};
MyRegister* MyRegister::myRegister= nullptr;

// Sub-flags of EFLAGS
//#define X86_EFLAGS_MODIFY_AF (1ULL << 0)
//#define X86_EFLAGS_MODIFY_CF (1ULL << 1)
//#define X86_EFLAGS_MODIFY_SF (1ULL << 2)
//#define X86_EFLAGS_MODIFY_ZF (1ULL << 3)
//#define X86_EFLAGS_MODIFY_PF (1ULL << 4)
//#define X86_EFLAGS_MODIFY_OF (1ULL << 5)


#endif //SYMBOL_EXEC_MYREGISTER_H
