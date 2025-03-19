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
    stream << "parameter with offset: " << offset << std::endl;
    //int offset = context.addLocalVariable(para_name);

    //stream << "addi sp,sp,-4"<< std::endl;

    // If there's an initializer, evaluate and store the result
    //if (initializer_ != nullptr) {
        // Evaluate initializer expression (result will be in a0)
    //    initializer_->EmitRISC(stream, context);
        // Store result to variable's stack location
    //    stream << "    sw a0, " << offset << "(s0)" << std::endl;
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
