#pragma once

#include "ast_node.hpp"

namespace ast {

// Break statement for use within loops
class BreakStatement : public Node {
public:
    BreakStatement() {}

    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;
};

} // namespace ast
