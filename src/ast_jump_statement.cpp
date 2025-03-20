#include "ast_jump_statement.hpp"

namespace ast {

    void ReturnStatement::EmitRISC(std::ostream& stream, Context& context) const
    {
        if (expression_ != nullptr)
        {
            expression_->EmitRISC(stream, context);
        }

        int end_label = context.getCurrentFunctionEndLabel();

        if (end_label >= 0) {
            stream << "    j func_end_" << end_label << std::endl;
        } else {
            stream << "    ret" << std::endl;
        }
    }

void ReturnStatement::Print(std::ostream& stream) const
{
    stream << "return";
    if (expression_ != nullptr)
    {
        stream << " ";
        expression_->Print(stream);
    }
    stream << ";" << std::endl;
}

}
