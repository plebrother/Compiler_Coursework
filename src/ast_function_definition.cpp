#include "ast_function_definition.hpp"
#include "ast_identifier.hpp"
#include "ast_direct_declarator.hpp"
#include <stdexcept>
#include <sstream>

namespace ast {

int FunctionDefinition::calculateStackSize(const Context&) const {

    // int base_size = 56;


    // int param_space = parameter_list_.size() * 4;


    // int local_var_space = context.getLocalVariablesSize();

    // if (local_var_space < 16) {
    //     local_var_space = 16;
    // }

    return 112;
}

void FunctionDefinition::EmitRISC(std::ostream& stream, Context& context) const
{
    if (declarator_ == nullptr) {
        std::cerr << "Error: Function declarator is null" << std::endl;
        return;
    }


    int stacksize = calculateStackSize(context);
    context.enterFunctionScope();


    int func_end_label = context.getNextLabel();
    context.setCurrentFunctionEndLabel(func_end_label);

    const DirectDeclarator* direct_decl = dynamic_cast<const DirectDeclarator*>(declarator_.get());
    std::string func_name;
    const NodePtr& id_ptr = direct_decl->getIdentifier();
    const Identifier* id = dynamic_cast<const Identifier*>(id_ptr.get());
    func_name = id->getName();




    stream << ".globl " << func_name << std::endl;




    stream << func_name << ":" << std::endl;



    stream << "    addi sp, sp, " << -stacksize << std::endl;  // stack space
    stream << "    sw ra, " << stacksize - 4 << "(sp)" << std::endl;  // store ra
    stream << "    sw s0, " << stacksize - 8 << "(sp)" << std::endl;  // store fp
    stream << "    addi s0, sp, " << stacksize << std::endl;  // set up new fp
    //stream << "    mv s0, sp" << std::endl;


    int reg_stack_offset = -16;
    // for (int i = 1; i <= 11; i++) {
    //     if (context.Regused(i-1)){
    //         stream << "    sw s" << i << ", " << stacksize - reg_stack_offset << "(sp)" << std::endl;
    //         reg_stack_offset = reg_stack_offset - 4;
    //     }

    // }

    context.set_variable_stack_offset(reg_stack_offset);


    //for (size_t i = 0; i < parameter_list_.size() && i < 8; i++) {

    //    stream << "    sw a" << i << ", " << -4 * (i + 1) << "(sp)" << std::endl;
    //}
    context.enterScope();

    direct_decl->EmitRISC(stream,context);

    context.freePara();

    if (compound_statement_ != nullptr) {
        compound_statement_->EmitRISC(stream, context);
    }


    stream << "func_end_" << func_end_label << ":" << std::endl;


    // for (int i = 1; i <= 11; i++) {
    //     stream << "    lw s" << i << ", " << stacksize - 8 - 4 * i << "(sp)" << std::endl;
    // }


    stream << "    lw ra, " << stacksize - 4 << "(sp)" << std::endl;
    stream << "    lw s0, " << stacksize - 8 << "(sp)" << std::endl;
    stream << "    addi sp, sp, " << stacksize  << std::endl;


    stream << "    ret" << std::endl;
    context.exitScope();
    stream << std::endl;

    context.exitFunctionScope();
}


void FunctionDefinition::Print(std::ostream& stream) const
{

    stream << return_type_ << " ";


    if (declarator_ != nullptr) {
        declarator_->Print(stream);
    }


    stream << "(";


    for (size_t i = 0; i < parameter_list_.size(); i++) {
        if (i > 0) {
            stream << ", ";
        }
        if (parameter_list_[i] != nullptr) {
            parameter_list_[i]->Print(stream);
        } else {
            stream << "void";
        }
    }


    if (parameter_list_.empty()) {
        stream << "void";
    }

    stream << ")" << std::endl;


    if (compound_statement_ != nullptr) {
        compound_statement_->Print(stream);
    }

    //stream << "}" << std::endl;
}

} // namespace ast
