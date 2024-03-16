#ifndef LIBCFG_ASM_CFG_H
#define LIBCFG_ASM_CFG_H

#include <cstdint>
#include <iostream>
#include <memory>
#include <optional>
#include <ostream>
#include <set>
#include <unordered_map>
#include <utility>
#include <vector>
#include "asm_block.h"
#include "capstone.h"
#include "libcfg_error.h"
#include "edge.h"
#include "libcfg_utils.h"
#include "x86.h"


namespace libCFG {
    struct asm_cfg;
}

struct libCFG::asm_cfg {
    /* Name of CFG */
    std::string name;
    /* 仅对函数内CFG有效,表示函数地址最低地址 */
    uint64_t low_pc;
    /* 仅对函数内CFG有效,表示函数地址最高地址 */
    uint64_t high_pc;
    std::unordered_map<uint64_t, std::shared_ptr<asm_block>> blocks;
    std::vector<std::shared_ptr<edge>> edges;

    asm_cfg()=default;

    asm_cfg(std::string& name, const cs_insn* instructions, uint32_t num_of_insn, uint32_t& serial_number) {
        if(num_of_insn == 0)
            libcfg_error("The number of instructions is zero!");
        this->name = name;
        this->low_pc = instructions[0].address;
        this->high_pc = instructions[num_of_insn-1].address;
        /* 标记每个基本块的第一条指令 */
        std::set<uint64_t> block_head_addr{ instructions[0].address };
        for(uint32_t i = 0; i < num_of_insn; i++) {
            auto& insn = instructions[i];
            switch (insn.id) {
                /* JUMP Instructments */
                case X86_INS_JMP  :
                case X86_INS_JA   :
                case X86_INS_JB   :
                case X86_INS_JG   :
                case X86_INS_JE   :
                case X86_INS_JAE  :
                case X86_INS_JBE  :
                case X86_INS_JGE  :
                case X86_INS_JNE  : {
                    if(isHexadecimal(insn.op_str)) {
                        uint64_t addr = hexStringToUInt(insn.op_str);
                        if(addr >= low_pc && addr <= high_pc)
                            block_head_addr.insert(addr);
                    }
                    if(i < num_of_insn-1)
                        block_head_addr.insert(instructions[i+1].address);
                    break;
                }
                /* RETURN Instructments */
                case X86_INS_RET  : {
                    if(i < num_of_insn-1)
                        block_head_addr.insert(instructions[i+1].address);
                    break;
                }
                /* CALL Instructments */
                case X86_INS_CALL : {
                    if(i < num_of_insn-1)
                        block_head_addr.insert(instructions[i+1].address);
                    break;
                }
                default: break;
            }
        }

        /* 分配汇编基本块 */
        this->blocks.insert(std::make_pair(0, libCFG::asm_block::start_block(serial_number++)));
        this->blocks.insert(std::make_pair(UINT64_MAX, libCFG::asm_block::exit_block(serial_number++)));
        for(uint64_t addr : block_head_addr) {
            this->blocks.insert(std::make_pair(addr, libCFG::asm_block::common_block(serial_number++, addr)));
        }
        /* 初始化汇编基本块中的指令 */
        auto cur_block = this->blocks[0];
        for(uint32_t i = 0; i < num_of_insn; i++) {
            auto& insn = instructions[i];
            if(this->blocks.find(insn.address) != this->blocks.end()) {
                auto next_block = this->blocks[insn.address];
                cur_block->forward_block = next_block;
                next_block->backward_block = cur_block;
                cur_block = next_block;
            }
            cur_block->statements.push_back(insn);
        }

        /* 插入边 */
        for(auto& entry : this->blocks) {
            auto& block = entry.second;
            switch (block->type) {
                case libCFG::BLOCK_TYPE::EXIT_BLOCK   : {
                    break; 
                }
                case libCFG::BLOCK_TYPE::ENTRY_BLOCK  : {
                    if(block->forward_block.has_value()) {
                        std::shared_ptr<edge> e = std::make_shared<edge>(block, block->forward_block.value(), EDGE_TYPE::DEFAULT_EDGE);
                        block->forward_edges.push_back(e);
                        block->forward_block.value()->backward_edges.push_back(e);
                        this->edges.push_back(e);
                    }
                    break;
                }
                case libCFG::BLOCK_TYPE::COMMON_BLOCK : {
                    auto& last_insn = block->statements.back();
                    switch (last_insn.id) {
                        case X86_INS_JMP  : {
                            if(isHexadecimal(last_insn.op_str)) {
                                uint64_t addr = hexStringToUInt(last_insn.op_str);
                                if(this->blocks.find(addr) != this->blocks.end()) {
                                    std::shared_ptr<edge> e = std::make_shared<edge>(block, this->blocks[addr], EDGE_TYPE::DEFAULT_EDGE);
                                    block->forward_edges.push_back(e);
                                    this->blocks[addr]->backward_edges.push_back(e);
                                    this->edges.push_back(e);
                                }
                            }
                            break;
                        }
                        case X86_INS_JA   :
                        case X86_INS_JB   :
                        case X86_INS_JG   :
                        case X86_INS_JE   :
                        case X86_INS_JAE  :
                        case X86_INS_JBE  :
                        case X86_INS_JGE  :
                        case X86_INS_JNE  : {
                            if(block->forward_block.has_value()) {
                                std::shared_ptr<edge> e = std::make_shared<edge>(block, block->forward_block.value(), EDGE_TYPE::BRANCH_FALSE);
                                block->forward_edges.push_back(e);
                                block->forward_block.value()->backward_edges.push_back(e);
                                this->edges.push_back(e);
                            }
                            if(isHexadecimal(last_insn.op_str)) {
                                uint64_t addr = hexStringToUInt(last_insn.op_str);
                                if(this->blocks.find(addr) != this->blocks.end()) {
                                    std::shared_ptr<edge> e = std::make_shared<edge>(block, this->blocks[addr], EDGE_TYPE::BRANCH_TRUE);
                                    block->forward_edges.push_back(e);
                                    this->blocks[addr]->backward_edges.push_back(e);
                                    this->edges.push_back(e);
                                }
                            }
                            break;
                        }
                        case X86_INS_RET : {
                            std::shared_ptr<edge> e = std::make_shared<edge>(block, this->blocks[UINT64_MAX], EDGE_TYPE::DEFAULT_EDGE);
                            block->forward_edges.push_back(e);
                            this->blocks[UINT64_MAX]->backward_edges.push_back(e);
                            this->edges.push_back(e);
                            break;
                        }
                        case X86_INS_CALL:
                        default          : {
                            if(block->forward_block.has_value()) {
                                std::shared_ptr<edge> e = std::make_shared<edge>(block, block->forward_block.value(), EDGE_TYPE::CALL_RETURN);
                                block->forward_edges.push_back(e);
                                block->forward_block.value()->backward_edges.push_back(e);
                                this->edges.push_back(e);
                            }
                            break;
                        }
                    }
                    break;
                }
                default: break;
            }
        }
    }

    /* 输出CFG对应的dot文件,默认输出到标准输出 */
    void to_dot(std::ostream& os=std::cout) {
        os << "digraph \"CFG for '"<< this->name << "' function\" {" << std::endl;
        os << "label=\"CFG for '" << this->name << "' function\";" << std::endl;
        for(auto& it : this->blocks) {
            auto& block = it.second;
            os << "Block" << block->sn;
            os << "[shape=record, color=\"#3d50c3ff\", style=filled, fillcolor=\"#b9d0f970\",";
            if(block->type == libCFG::BLOCK_TYPE::ENTRY_BLOCK)
                os << "label=\"{" << this->name << "::ENTRY"  << "\\l ";
            else if(block->type == libCFG::BLOCK_TYPE::EXIT_BLOCK)
                os << "label=\"{" << this->name << "::EXIT"  << "\\l ";
            else 
                os << "label=\"{B" << block->sn << "\\l ";

            for(auto& insn : block->statements) {
                os << "0x" << dec_to_hex(insn.address) << ":   " << insn.mnemonic << "  " << insn.op_str << "\\l ";
            }
            os << "}\"" << "];" << std::endl;
            for(auto& e : block->forward_edges) {
                if(e->type == EDGE_TYPE::CALL || e->type == EDGE_TYPE::RETURN)
                    continue;
                os << "Block" << e->from->sn << " -> " << "Block" << e->to->sn << ";\n";
            }
        }
        os << "}" << std::endl;
    }

    std::shared_ptr<libCFG::asm_block>& get_entry_block() {
        return this->blocks[0];
    }

    std::shared_ptr<libCFG::asm_block>& get_exit_block() {
        return this->blocks[UINT64_MAX];
    }
};

#endif