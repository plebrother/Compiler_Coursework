#include "ast_direct_declarator.hpp"

namespace ast {

void DirectDeclarator::EmitRISC(std::ostream& stream, Context& context) const
{
    //identifier_->EmitRISC(stream, context);
    //stream << ":" << std::endl;
    if (parameter_list_ != nullptr){
        parameter_list_->EmitRISC(stream,context);
    }
}

void DirectDeclarator::Print(std::ostream& stream) const
{
    identifier_->Print(stream);
}

} // namespace ast
