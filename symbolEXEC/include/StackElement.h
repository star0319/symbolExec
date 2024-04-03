//
// Created by shengxin on 2024/3/29.
//

#ifndef SYMBOL_EXEC_STACKELEMENT_H
#define SYMBOL_EXEC_STACKELEMENT_H

#include <iostream>

class StackElement{
public:
    uint8_t data;
    bool isSymbol;
    StackElement();
    StackElement(uint8_t data,bool isSymbol){
        this->data=data;
        this->isSymbol=isSymbol;
    }

    void print(){
        std::cout<<  "data: " << std::hex << std::setw(2) << std::setfill('0')<<static_cast<int>(data)<< std::dec<<", isSymbol: " << isSymbol << std::endl;
    }

};


#endif //SYMBOL_EXEC_STACKELEMENT_H
