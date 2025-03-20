#pragma once

#include "ast_node.hpp"
#include "ast_type_specifier.hpp"
#include <string>
#include <vector>

namespace ast {

class FunctionDeclaration : public Node
{
private:
    const TypeSpecifier return_type_;
    NodePtr declarator_;

public:

    FunctionDeclaration(TypeSpecifier return_type,
                      NodePtr declarator)
        : return_type_(return_type),
          declarator_(std::move(declarator)) {};

    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;
};

} // namespace ast
