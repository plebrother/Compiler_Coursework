#include "ast_function_definition.hpp"

namespace ast {

void FunctionDefinition::EmitRISC(std::ostream& stream, Context& context) const
{
    // Emit assembler directives.
    // TODO: these are just examples ones, make sure you understand
    // the concept of directives and correct them.

    int stacksize = 52;
    stream << ".text" << std::endl;
    stream << ".globl ";
    declarator_->Print(stream);
    stream << std::endl;

    declarator_->EmitRISC(stream, context);

    stream <<"addi sp, sp, "<< -stacksize <<std::endl; //shift spGIT
    stream <<"sw ra, "<< stacksize - 4 << "(sp)"<<std::endl; //store previous return addess 4 below the fp

    for (int i = 0; i < 12; i++){
        stream << "sw s" << i << ", " << stacksize - 8 - 4 * i << "(sp)" <<std::endl;
    }
    stream << "addi s0, sp, "<<stacksize<<std::endl;

    if (compound_statement_ != nullptr)
    {
        compound_statement_->EmitRISC(stream, context);
    }

    for (int i = 0; i < 12; i++){
        stream << "lw s" << i << ", " << stacksize - 8 - 4 * i << "(sp) "<<std::endl;
    }

    stream << "lw ra, " << stacksize - 4 << "(sp)" <<std::endl; //restore  previous return addess
    stream << "addi sp, sp, " << stacksize << std::endl; //restore sp
    stream << "ret" << std::endl;
    stream << "nop" << std::endl;
    stream << std::endl;
}

void FunctionDefinition::Print(std::ostream& stream) const
{
    stream << declaration_specifiers_ << " ";

    declarator_->Print(stream);
    stream << "() {" << std::endl;

    if (compound_statement_ != nullptr)
    {
        compound_statement_->Print(stream);
    }
    stream << "}" << std::endl;
}

}
