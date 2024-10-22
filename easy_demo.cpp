//
// Created by shengxin on 2024/4/26.
//
#include "assert.h"

using namespace std;
void foo(int a, int b){
    int x = 1, y = 0;
    if(a != 0){
        y = 3+x;
        if(b == 0)
            x = 2*(a+b);
    }
    assert(x-y != 0);
}