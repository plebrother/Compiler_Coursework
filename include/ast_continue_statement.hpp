#pragma once

#include "ast_node.hpp"

namespace ast {

// Continue statement for use within loops
class ContinueStatement : public Node {
public:
    ContinueStatement() {}

    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;
};

} // namespace ast
