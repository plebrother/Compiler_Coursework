#pragma once

#include "ast_node.hpp"
#include "ast_type_specifier.hpp"

namespace ast {

class VariableDeclaration : public Node
{
private:
    TypeSpecifier type_specifier_;
    NodePtr identifier_;
    NodePtr initializer_;
    bool is_global_;

public:
    VariableDeclaration(TypeSpecifier type_specifier, NodePtr identifier, NodePtr initializer = nullptr, bool is_global = false)
        : type_specifier_(type_specifier), identifier_(std::move(identifier)),
          initializer_(std::move(initializer)), is_global_(is_global) {}

    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;

};

} // namespace ast
