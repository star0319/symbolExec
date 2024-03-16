#ifndef LIBCFG_EDGE_H
#define LIBCFG_EDGE_H

#include <memory>

namespace libCFG {
    struct asm_block;
    struct edge;
    enum class EDGE_TYPE {
        CALL,
        RETURN,
        CALL_RETURN,
        BRANCH_TRUE,
        BRANCH_FALSE,
        DEFAULT_EDGE,
    };
    
}

struct libCFG::edge {
    std::shared_ptr<asm_block> from;
    std::shared_ptr<asm_block> to;
    libCFG::EDGE_TYPE type;

    edge(std::shared_ptr<asm_block>& from, std::shared_ptr<asm_block>& to, libCFG::EDGE_TYPE type)
        : from(from), to(to), type(type) {}
};

#endif