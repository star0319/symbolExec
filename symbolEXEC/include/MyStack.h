//
// Created by shengxin on 2024/3/22.
//

#ifndef SYMBOL_EXEC_MYSTACK_H
#define SYMBOL_EXEC_MYSTACK_H


#include <iostream>
#include <vector>
#include "StackElement.h"
#include "MyRegister.h"

using namespace std;

#define rbp execRegister->registerMap[X86_REG_RBP]
#define rsp execRegister->registerMap[X86_REG_RSP]
//template <typename T>
class MyStack {


private:

    int capacity; // 栈的容量
    std::vector<StackElement*> memory;  // 栈的内存
    MyRegister* execRegister;
//    int rsp;                    // 栈指针（模拟rsp）
//    int rbp;                    // 基指针（模拟rbp）

//    // 辅助函数：将整数转换为字节序列并压入栈中
//    template <typename T>
//    void pushInt(T value) {
//        static_assert(std::is_integral<T>::value, "Can only push integral types.");
//        for (size_t i = 0; i < sizeof(T); ++i) {
//            memory[rsp] = reinterpret_cast<uint8_t*>(&value)[sizeof(T) - 1 - i];
//            --rsp;
//        }
//    }
//
//    // 辅助函数：从栈中弹出字节序列并转换为整数
//    template <typename T>
//    T popInt() {
//        static_assert(std::is_integral<T>::value, "Can only pop integral types.");
//        T value = 0;
//        for (size_t i = 0; i < sizeof(T); ++i) {
//            ++rsp;
//            reinterpret_cast<uint8_t*>(&value)[i] = memory[rsp];
//        }
//        return value;
//    }

public:
    //rsp rbp 都指向顶部
    MyStack(int initialSize = 100000) {
        capacity=initialSize ;
        execRegister= (MyRegister::GetRegister());
        rsp=initialSize ;
        rbp=initialSize ;
        memory.resize(capacity);
//        std::fill(memory.begin(), memory.end(),0);
    }

    // 压栈操作
//    void push(StackElement* value) {
//        memory[rsp] = value;
//        --rsp;
//    }


//TODO：寄存器的低高地址需要修改地址
    uint64_t  pop64() {
        uint64_t value=0;
        for (int i = 0; i < 8; ++i) {
//            value=value>>8;
            uint64_t tmp=pop8();
            value=value+  (tmp<<(8*i));
        }
        return value;
    }

    uint8_t pop8() {
        uint8_t value=memory[rsp++]->data ;
        return value;
    }

//TODO：寄存器的低高地址需要修改地址
    void push64(uint64_t value) {
        for (int i = 7; i >= 0; --i) {
            push8((value >>(8*i)) & 0xFF );
        }

    }

    void push8(uint8_t value) {
        memory[--rsp] = new StackElement(value, false);
//        execRegister->registerMap[X86_REG_RSP]--;
//        execRegister->update(X86_REG_RSP,execRegister->registerMap[X86_REG_RSP]-1);
//        execRegister->printRegister();
    }

    void update64(uint64_t adress,uint64_t value) {
        for (int i = 0; i < 8; ++i) {
            update8(adress+i ,(value >>(8*i)) & 0xFF );
        }
    }
    void update32(uint64_t adress,uint32_t value) {
        for (int i = 0; i < 4; ++i) {
            update8(adress+i ,(value >>(8*i)) & 0xFF );
        }
    }
    void update8(uint64_t adress,uint8_t value) {
        memory[adress] = new StackElement(value, false);
//        execRegister->registerMap[X86_REG_RSP]--;
//        execRegister->update(X86_REG_RSP,execRegister->registerMap[X86_REG_RSP]-1);
//        execRegister->printRegister();

    }
    uint64_t get64(uint64_t adress) {
        uint64_t value=0;
        for (int i = 7; i >=0; --i) {

            value=( value<<8) +  get8(adress+i );
        }
    }
    uint32_t  get32(uint64_t adress) {
        uint32_t value=0;
        for (int i = 3; i >=0 ; --i) {
            value=( value<<8) +  get8(adress+i );
        }
    }
    uint8_t  get8(uint64_t adress) {
        return memory[adress]->data ;
    }


//    // 弹出栈顶元素
//    StackElement* pop() {
//        ++rsp;
////        execRegister->registerMap[X86_REG_RSP]++;
////        execRegister->update(X86_REG_RSP,execRegister->registerMap[X86_REG_RSP]+1);
//        return memory[rsp];
//    }
//
//    // 查看栈顶元素但不弹出
//    StackElement* peek() const {
//        return memory[rsp + 1];
//    }



    // 是否栈空
    bool isEmpty() const {
        return rsp == memory.size() - 1;
    }

    // 是否栈满
    bool isFull() const {
        return rsp == 0;
    }

//    // 设置rbp（基指针） 记得加改变寄存器
//    void setRbp(int newRbp) {
//        rbp = newRbp;
//    }

    // 获取rbp（基指针）
    uint64_t getRbp() const {
        return rbp;
    }

//    // 设置rsp（基指针）   记得加改变寄存器
//    void setRsp(int newrsp) {
//        rsp = newrsp;
//    }

    // 获取rsp（基指针）
    uint64_t getRsp() const {
        return rsp;
    }

//    // 压入多种数据格式
//    void push(int value) {
//        pushInt<int>(value);
//    }
//
//    void push(long long value) {
//        pushInt<long long>(value);
//    }
//
//    // 弹出多种数据格式
//    int popInt() {
//        return popInt<int>();
//    }
//
//    long long popLongLong() {
//        return popInt<long long>();
//    }
//
//    // 打印栈状态
    void printStack(bool print) const {
        if (!print) return;
        std::cout << "Stack contents (high to low address):" << std::endl;
        for (uint64_t i = rsp ; i < memory.size(); ++i) {
            std::cout << "address: "<<static_cast<int>(i)<<"  ";
            if (memory[i]!= nullptr)
            memory[i]->print();
            else cout<<endl;
        }
        std::cout << std::endl;
        std::cout << "rsp: " << rsp << ", rbp: " << rbp << std::endl;
    }









//private:
//    vector<uint8_t> elements;
//
//public:
//    // 判断栈是否为空
//    bool isEmpty() {
//        return elements.empty();
//    }
//
//    // 入栈
//    void push(int value) {
//        elements.push_back(value);
//    }
//
//    // 出栈
//    int pop() {
//        if (isEmpty()) {
//            cout << "Error: Stack is empty." << endl;
//            return -1;
//        }
//        int top = elements.back();
//        elements.pop_back();
//        return top;
//    }
//
//    // 获取栈顶元素
//    int top() {
//        if (isEmpty()) {
//            cout << "Error: Stack is empty." << endl;
//            return -1;
//        }
//        return elements.back();
//    }
//
//    // 获取栈的大小
//    int size() {
//        return elements.size();
//    }


};


#endif //SYMBOL_EXEC_MYSTACK_H
