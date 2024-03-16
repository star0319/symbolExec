#ifndef LIBCFG_TYPE_H
#define LIBCFG_TYPE_H

#include <nlohmann/json.hpp>
#include <string>
#include <cstdint>

namespace libCFG {
    struct general_type;
    struct var_type;
}

struct libCFG::general_type {
    uint16_t module;
    uint64_t ident;
    uint64_t dw_tag;
    std::string name;
    uint32_t size;
    uint64_t reference;
    uint32_t array_bound;

    general_type() {
        /* 默认使用void类型 */
        this->module = 0;
        this->name = "void";
        this->ident = 0;
        this->dw_tag = 0;
        this->size = 0;
        /* void指针类型中,reference会被设置为0,为简单起见,将reference默认设置为0,同时含有void的含义 */
        this->reference = 0; 
        this->array_bound = 0;
    }

    nlohmann::json to_json(bool withHeader) const {
        nlohmann::json content = {
            {"module", this->module},
            {"name", this->name},
            {"ident", this->ident},
            {"reference", this->reference},
            // {"dw_tag", dw_tag_to_str(this->dw_tag)},
        };
        if(withHeader)
            return {{"general_type", content}};
        else
            return content;
    }
};

struct libCFG::var_type {
    uint16_t module;
    uint64_t ident;
    uint32_t sn;
    uint64_t dw_tag;
    std::string name;
    uint32_t size;
    uint64_t refer_type_ident;
    uint64_t refer_type_sn;

    var_type() {
        /* 和general_type同理,默认使用void类型 */
        this->module = 0;
        this->name = "void";
        this->ident = 0;
        this->sn = 0;
        this->dw_tag = 0;
        this->size = 0;
        this->refer_type_ident = 0;
        this->refer_type_sn = 0;
    }

    nlohmann::json to_json(bool withHeader) const {
        nlohmann::json content = {
            {"module", this->module},
            {"name", this->name},
            {"ident", this->ident},
            {"sn", this->sn},
            {"refer_type_id", this->refer_type_ident},
            {"refer_type_sn", this->refer_type_sn},
            // {"dw_tag", dw_tag_to_str(this->dw_tag)},
        };
        if(withHeader)
            return {{"type", content}};
        else
            return content;
    }
};


#endif