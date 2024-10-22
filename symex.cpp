//
// Created by shengxin on 2023/12/3.
//
#include <iostream>
#include "assert.h"





int  add1 (int a, int b){

    return 1+ a;
}

int  add2 (int a, int b){

    return  1+ add1(a,b);
}

int  add3 (int a, int b){

    return  1+ add2(a,b);
}

int  add4 (int a, int b){

    return   1+ add3(a,b);
}

int  add5 (int a, int b){
    return 1+ add4(a,b);
}

int  addx_y (int a, int b){
    return  add5(a,b);
}




int main(int argc, char** argv )
{
    int a=7,b=8;
//    std:: cin>> a >>b;
//std:: cout<<argv[1]<<std::endl;
    a= atoi( argv[1]);
    b= atoi( argv[2]);
//    std:: cout<<"a    "<<a <<b<<std::endl;
    int sum= addx_y(a,b);
    assert(a+b==sum);
    return 0;
}


//pc   edi=0
//eax=1


//pc  edi!=0