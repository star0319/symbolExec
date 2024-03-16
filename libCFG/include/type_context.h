#ifndef LIBCFG_TYPE_CONTEXT_H
#define LIBCFG_TYPE_CONTEXT_H

#include "type.h"
#include <iostream>
#include <cstdint>
#include <iterator>
#include <map>

// 用于记录某个block中各个位置的类型
struct type_context {
    std::map<int32_t, libCFG::var_type> type_map;


    type_context() {
        for(int i=1; i<=4; i++)
            this->type_map[i] = libCFG::var_type();
    }

    void print(bool withComma, bool nextLine) {
        std::cout << "{";
        auto it = this->type_map.begin();
        while(it != this->type_map.end()) {
            // 大于0，寄存器相关
            auto nit = std::next(it);
            if((*it).first > 0) {
                if((*it).first == 1) std::cout << "\"eax\":\"" << type_map[1].name << "\"";
                if((*it).first == 2) std::cout << "\"ebx\":\"" << type_map[2].name << "\"";
                if((*it).first == 3) std::cout << "\"ecx\":\"" << type_map[3].name << "\"";
                if((*it).first == 4) std::cout << "\"edx\":\"" << type_map[4].name << "\""; 
            }
            // 小于0,cfa相关地址 
            if((*it).first < 0) {
                std::cout << "\"cfa" << (*it).first << "\":\"" << (*it).second.name << "\"";
            }

            if(nit != this->type_map.end()) {
                std::cout << ",";
            }
            it = nit;
        }
        if(withComma) std::cout << "},";
        else std::cout << "}";
        if(nextLine) std::cout << "\n";
    }
};

#endif