//
// Created by shengxin on 2024/4/26.
//
#include <stdio.h>
#include "assert.h"
// 全局变量定义
int globalVar = 10;

void function1() {
    // 在函数1中访问全局变量
    assert(globalVar==10);
    globalVar = 20; // 修改全局变量的值
}

void function2() {

    // 在函数2中访问全局变量
    assert(globalVar==20);
}

int main() {
    // 在main函数中访问全局变量
    assert(globalVar==10);

    // 调用函数1
    function1();

    // 再次在main函数中访问全局变量，此时值已被函数1修改
    assert(globalVar==20);

    // 调用函数2
    function2();

    return 0;
}