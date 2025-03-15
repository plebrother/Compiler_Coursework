#pragma once

#include "ast_node.hpp"

namespace ast {

// If statement - can have an optional else branch
class IfStatement : public Node {
private:
    NodePtr condition_;
    NodePtr if_branch_;
    NodePtr else_branch_; // Can be nullptr if no else branch

public:
    IfStatement(NodePtr condition, NodePtr if_branch, NodePtr else_branch = nullptr)
        : condition_(std::move(condition)),
          if_branch_(std::move(if_branch)),
          else_branch_(std::move(else_branch)) {}

    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;
};

} // namespace ast
