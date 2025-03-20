#include "ast_function_declaration.hpp"
#include "ast_identifier.hpp"
#include "ast_direct_declarator.hpp"

namespace ast {

void FunctionDeclaration::EmitRISC(std::ostream& stream, Context& ) const
{
    const DirectDeclarator* direct_decl = dynamic_cast<const DirectDeclarator*>(declarator_.get());
    if (!direct_decl) {
        std::cerr << "Error: Expected direct declarator in function declaration" << std::endl;
        exit(1);
    }


    const NodePtr& id_ptr = direct_decl->getIdentifier();
    const Identifier* id = dynamic_cast<const Identifier*>(id_ptr.get());
    if (!id) {
        std::cerr << "Error: Expected identifier in function declaration" << std::endl;
        exit(1);
    }
    std::string func_name = id->getName();

    stream << ".extern " << func_name << std::endl;


    // stream << ".type " << func_name << ", @function" << std::endl;
}

void FunctionDeclaration::Print(std::ostream& stream) const
{

    stream << return_type_ << " ";


    if (declarator_ != nullptr) {
        declarator_->Print(stream);
    }

    stream << "(";



    //if (parameter_list_.empty()) {
    //    stream << "void";
    //}

    stream << ");" << std::endl;
}

} // namespace ast
