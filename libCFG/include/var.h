#ifndef LIBCFG_VAR_H
#define LIBCFG_VAR_H

#include <string>
#include <cstdint>
#include <nlohmann/json.hpp>
#include "type.h"

namespace libCFG {
    struct var_addr;
    struct var;
}

struct libCFG::var_addr {
    bool is_unknown;
    bool is_global;
    bool is_local;
    bool is_based_offset;

    int64_t cfa_offset;

    var_addr() 
        : is_unknown(true), is_global(false), is_local(false), is_based_offset(false), cfa_offset(0) {}

    nlohmann::json to_json(bool withHeader) const {
        nlohmann::json content = {
              {"is_unknown", (this->is_unknown?"true":"false")},
              {"is_global", (this->is_global?"true":"false")},
              {"is_local", (this->is_local?"true":"false")},
              {"is_based_offset", (this->is_based_offset?"true":"false")},
              {"cfa_offset", this->cfa_offset},
        };
        if(withHeader)
            return {{"var_addr", content}};
        else
            return content;
    }
};

struct libCFG::var {
    std::string name;
    uint32_t var_id;
    libCFG::var_addr addr;
    libCFG::var_type type;
    bool is_parameter;

    var() {
        this->name = std::string("Unknown");
        this->var_id = 0;
        this->addr = libCFG::var_addr();
        this->type = libCFG::var_type();
        this->is_parameter = false;
    }

    nlohmann::json to_json(bool withHeader) const {
        nlohmann::json content = {
            {"name", (this->name.empty()?std::string("Unknown"):this->name)},
            {"var_id", this->var_id},
            {"is_parameter", (this->is_parameter?"true":"false")},
            this->type.to_json(true),
            this->addr.to_json(true),
        };

        if(withHeader)
            return {{"var", content}};
        else
            return content;
    }
};


#endif