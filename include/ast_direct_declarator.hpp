#pragma once

#include "ast_node.hpp"

namespace ast {

class DirectDeclarator : public Node
{
private:
    NodePtr identifier_;
    NodeListPtr parameter_list_;

public:
    DirectDeclarator(NodePtr identifier, NodeListPtr parameter_list)
    : identifier_(std::move(identifier)), parameter_list_(std::move(parameter_list)) {}

    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;
    const NodePtr& getIdentifier() const { return identifier_; }
};

} // namespace ast
