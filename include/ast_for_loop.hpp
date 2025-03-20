#pragma once

#include "ast_node.hpp"

namespace ast {

// For loop: for(init; condition; increment) body
class ForStatement : public Node {
private:
    NodePtr init_;       // Can be nullptr
    NodePtr condition_;  // Can be nullptr
    NodePtr increment_;  // Can be nullptr
    NodePtr body_;

public:
    ForStatement(NodePtr init, NodePtr condition, NodePtr increment, NodePtr body)
        : init_(std::move(init)),
          condition_(std::move(condition)),
          increment_(std::move(increment)),
          body_(std::move(body)) {}

    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;
};

} // namespace ast
