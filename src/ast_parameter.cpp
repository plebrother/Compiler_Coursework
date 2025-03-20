#include "ast_parameter.hpp"
#include "ast_identifier.hpp"

namespace ast {

void ParameterDeclaration::EmitRISC(std::ostream& stream, Context& context) const
{

    const Identifier* id = dynamic_cast<const Identifier*>(identifier_.get());
    if (!id) {
        std::cerr << "Error: Expected identifier in variable declaration" << std::endl;
        exit(1);
    }
    std::string para_name = id->getName();

    int offset = context.addLocalVariable(para_name);
    //stream << "parameter with offset: " << offset << std::endl;
    std::string para_reg = "a";
    stream << "sw " << para_reg << context.allocatePara() << ", " << offset << "(s0)" << std::endl;

    }



void ParameterDeclaration::Print(std::ostream& stream) const
{
    // For debugging/pretty-printing
    stream << type_specifier_ << " ";
    identifier_->Print(stream);

    //if (initializer_ != nullptr) {
    //    stream << " = ";
    //    initializer_->Print(stream);
    //}

    stream << ";" << std::endl;
}

} // namespace ast
