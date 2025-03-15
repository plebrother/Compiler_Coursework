#pragma once

#include "ast_node.hpp"

namespace ast {

// Do-While loop statement
class DoWhileStatement : public Node {
private:
    NodePtr body_;
    NodePtr condition_;

public:
    DoWhileStatement(NodePtr body, NodePtr condition)
        : body_(std::move(body)),
          condition_(std::move(condition)) {}

    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;
};

} // namespace ast
