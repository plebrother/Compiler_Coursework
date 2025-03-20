#pragma once

#include "ast_node.hpp"
#include "ast_type_specifier.hpp"

namespace ast {

class DeclareStatement : public Node
{
private:
    const TypeSpecifier declaration_specifiers_;
    NodeListPtr declarator_;

public:
    DeclareStatement(TypeSpecifier declaration_specifiers, NodeListPtr declarator) : declaration_specifiers_(declaration_specifiers),declarator_(std::move(declarator)) {}

    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;
};

} // namespace ast
