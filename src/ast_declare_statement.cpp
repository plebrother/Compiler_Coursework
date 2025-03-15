#include "ast_declare_statement.hpp"

namespace ast {

void DeclareStatement::EmitRISC(std::ostream& stream, Context& context) const
{
    if (declaration_specifiers_ != TypeSpecifier::INT){
        std::cerr << "unkown type in declare statement" << std::endl;
        exit(1);
    }

    if (declarator_ != nullptr)
    {
        declarator_->EmitRISC(stream, context);
    }
    //stream << ":" << std::endl;
    //stream << ".zero 4" << std::endl;
}

void DeclareStatement::Print(std::ostream& stream) const
{
    stream << declaration_specifiers_;
    if (declarator_ != nullptr)
    {
        stream << " ";
        declarator_->Print(stream);
    }
    stream << ";" << std::endl;
}

}
