#pragma once

#include "ast_node.hpp"
#include "ast_type_specifier.hpp"

namespace ast {

class ParameterDeclaration : public Node
{
private:
    TypeSpecifier type_specifier_;
    NodePtr identifier_;

public:
    ParameterDeclaration(TypeSpecifier type_specifier, NodePtr identifier)
        : type_specifier_(type_specifier), identifier_(std::move(identifier)) {}

    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;
};

} // namespace ast
