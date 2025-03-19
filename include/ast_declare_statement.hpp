#pragma once

#include "ast_node.hpp"
#include "ast_type_specifier.hpp"

namespace ast {

class DeclareStatement : public Node
{
private:
    const TypeSpecifier declaration_specifiers_;
    NodeList* declarator_;

public:
    DeclareStatement(TypeSpecifier declaration_specifiers, NodeList* declarator) : declaration_specifiers_(declaration_specifiers),declarator_(std::move(declarator)) {}

    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;
};

} // namespace ast
