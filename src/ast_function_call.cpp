#include "ast_function_call.hpp"
#include "ast_identifier.hpp"
#include <vector>

namespace ast {

void FunctionCall::EmitRISC(std::ostream& stream, Context& context) const
{

    std::vector<int> saved_regs;


    int arg_count = 0;
    if (arguments_ != nullptr) {

        const std::vector<NodePtr>& args = arguments_->getNodes();
        arg_count = args.size();
    }

    if (arg_count > 8) {
        std::cerr << "argument number is larger than 8" << std::endl;
        return;
    }



    if (arguments_ != nullptr) {
        const std::vector<NodePtr>& args = arguments_->getNodes();

        for (int i = args.size() - 1; i >= 0; i--) {


            args[i]->EmitRISC(stream, context);



            if (i > 0) {
                stream << "    mv a" << i << ", a0" << std::endl;
            }

        }
    }

    //std::string function_name = function_name_->getName();
    //function_name_->EmitRISC(stream,context);

    int reg_stack_offset = context.get_current_stack_offset();
    int stacksize = 112;

    for (int i = 1; i <= 11; i++) {
        if (context.Regused(i-1)){
            reg_stack_offset = reg_stack_offset - 4;
            stream << "    sw s" << i << ", " << stacksize + reg_stack_offset << "(sp)" << std::endl;
        }

    }




    stream << "    # function call " << function_name_ << std::endl;
    stream << "    call " << function_name_ << std::endl;


    for (int i = 11; i >= 1; i--) {
        if (context.Regused(i-1)){
            stream << "    lw s" << i << ", " << stacksize + reg_stack_offset << "(sp)" << std::endl;
            reg_stack_offset = reg_stack_offset + 4;
        }
    }

    context.set_variable_stack_offset(reg_stack_offset);


}

void FunctionCall::Print(std::ostream& stream) const
{
    stream << function_name_ << "(";


    if (arguments_ != nullptr) {
        const std::vector<NodePtr>& args = arguments_->getNodes();
        for (size_t i = 0; i < args.size(); ++i) {
            if (i > 0) {
                stream << ", ";
            }
            args[i]->Print(stream);
        }
    }

    stream << ")";
}

} // namespace ast
