#ifndef LIBCFG_FUNC_H
#define LIBCFG_FUNC_H

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <string>
#include <sys/types.h>
#include <vector>
#include <cxxabi.h>
#include <optional>

#include "capstone.h"
#include <nlohmann/json.hpp>
#include "nlohmann/json_fwd.hpp"
#include "var.h"
#include "asm_cfg.h"

namespace libCFG {
    struct func;
}

struct libCFG::func {
    /* Basic Information */

    uint16_t module;
    std::string name;
    uint64_t low_pc;
    uint64_t high_pc;
    uint64_t size;

    /* For Capstone */

    csh handle;
    cs_insn *instructions;
    uint32_t num_of_insn;
    char* insn_buffer;

    /* variable(s), including parameter(s) */
    std::vector<var> var_list;

    /* cfg of func */
    std::optional<libCFG::asm_cfg> cfg;
public:
    /* 禁止默认构造 */
    func()=delete;
    /* 指定函数名称以及函数low_pc,high_pc,  high_pc为大小 */

    func(std::string& func_name, uint64_t low_pc, uint64_t high_pc) /* Ref: DWARF5.pdf p52 */
        : name(func_name), low_pc(low_pc), high_pc(high_pc+low_pc), size(high_pc)
    {
        this->module = 0;
        this->insn_buffer = nullptr;
        this->num_of_insn = 0;
        this->instructions = nullptr;
    }

    /* 当该函数被指定了汇编代码段数据时,返回True;否则返回False */
    bool isContainedData() {
        return !(this->insn_buffer == nullptr);
    }

    /* 当该函数被反汇编为汇编指令时,返回True;否则返回False */
    bool isParsed() {
        return !(this->instructions == nullptr);
    }

    bool hasCFG() const {
        return this->cfg.has_value();
    }

    bool parse() {
        if(!this->isContainedData()) 
            return false;

        if(cs_open(CS_ARCH_X86, CS_MODE_64, &(this->handle)) != CS_ERR_OK)
            return false;
        //new
        if(cs_option(this->handle,CS_OPT_DETAIL,CS_OPT_ON) !=CS_ERR_OK)
            return false;

        this->num_of_insn = cs_disasm(
            this->handle, reinterpret_cast<uint8_t*>(this->insn_buffer), 
            this->size, this->low_pc, 0, &(this->instructions)
        );

        return true;
    }

    int cs_OP_Count( const cs_insn insn, unsigned int op_type){
        return cs_op_count(this->handle,&insn,op_type);
    }

    int  cs_OP_Index(const cs_insn insn, unsigned int op_type,unsigned int position){
        return cs_op_index(this->handle,&insn,op_type,position);

    }


    bool constructCFG(uint32_t& serial_number) {
        if(!this->isParsed())
            return false;
        if(this->hasCFG())
            return true;

        this->cfg.emplace(this->name, this->instructions, this->num_of_insn, serial_number);
        return true;
    }

    nlohmann::json to_json(bool withHeader) {
        int status;
        std::string demangled_name;
        char* name = abi::__cxa_demangle(this->name.c_str(), nullptr, 0, &status);
        if (status != 0) // 发生错误时,认为该名称无法转化,于是使用默认的名称
            demangled_name = this->name;
        else
            demangled_name = std::string(name);
        free(name);

        std::vector<nlohmann::json> vars_json_list(this->var_list.size());
        std::transform(this->var_list.begin(), this->var_list.end(), std::back_inserter(vars_json_list), [](const var& v){ return v.to_json(false); });
        std::vector<std::string> asm_list(this->num_of_insn);
        if(this->isParsed()) {
           for(uint32_t i = 0; i < this->num_of_insn; i++) {
                cs_insn& in = this->instructions[i];
                asm_list[i] = "0x" + dec_to_hex(in.address) + " " + in.mnemonic + " " + in.op_str;
            }
        }

        nlohmann::json content ={
            {"module", this->module},
            {"name", demangled_name},
            {"size", this->size},
            {"low_pc", this->low_pc},
            {"high_pc", this->high_pc},
            {"num_of_insn", this->num_of_insn},
            {"Initialized", ((this->isContainedData())?"true":"false")},
            {"Decompiled", ((this->isParsed())?"true":"false")},
            {"Vars", nlohmann::json(vars_json_list)},
            {"Asm", nlohmann::json(asm_list)},
        };

        if(withHeader)
            return {{"function", content}};
        else
            return content;
    }
};

#endif