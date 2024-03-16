#ifndef LIBCFG_ASM_BLOCK_H
#define LIBCFG_ASM_BLOCK_H

#include "asm_statement.h"
#include <cstdint>
#include <memory>
#include <optional>
#include <vector>

namespace libCFG {
    struct asm_block;
    struct edge;
    
    enum class BLOCK_TYPE {
        ENTRY_BLOCK,
        EXIT_BLOCK,
        COMMON_BLOCK
    };
}

struct libCFG::asm_block {
    /* 每个block的唯一标识符 */
    uint32_t sn;
    /* 每个block对应的首地址,对于ENTRY和EXIT块无效 */
    uint64_t addr;
    libCFG::BLOCK_TYPE type;
    std::vector<asm_statement> statements;
    std::optional<std::shared_ptr<asm_block>> forward_block;
    std::optional<std::shared_ptr<asm_block>> backward_block;
    std::vector<std::shared_ptr<edge>> forward_edges;
    std::vector<std::shared_ptr<edge>> backward_edges;

    asm_block()
        : sn(0), addr(0), type(libCFG::BLOCK_TYPE::COMMON_BLOCK) {}

    /* EXIT_BLOCK, type=ENTRY, addr=0 */
    static std::shared_ptr<asm_block> start_block(uint32_t sn) {
        auto s = std::make_shared<asm_block>();
        s->type = libCFG::BLOCK_TYPE::ENTRY_BLOCK;
        s->sn   = sn;
        s->addr = 0;
        return s;
    }

    /* EXIT_BLOCK, type=EXIT, addr=UINT64_MAX */
    static std::shared_ptr<asm_block> exit_block(uint32_t sn) {
        auto e = std::make_shared<asm_block>();
        e->type = libCFG::BLOCK_TYPE::EXIT_BLOCK;
        e->sn   = sn;
        e->addr = UINT64_MAX;
        return e;
    }

    /* COMMON_BLOCK, type=COMMON, addr=addr */
    static std::shared_ptr<asm_block> common_block(uint32_t sn, uint64_t addr) {
        auto c = std::make_shared<asm_block>();
        c->sn   = sn;
        c->addr = addr;
        return c;
    }
};

#endif