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

    stream << "    # function call " << function_name_ << std::endl;
    stream << "    call " << function_name_ << std::endl;



}

void FunctionCall::Print(std::ostream& stream) const
{
    stream << function_name_ << "(";

    // 打印参数列表
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
