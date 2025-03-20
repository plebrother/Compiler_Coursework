#include "ast_unary_expression.hpp"
#include "ast_variable.hpp"

namespace ast {

void UnaryExpression::EmitRISC(std::ostream& stream, Context& context) const {

    operand_->EmitRISC(stream, context);


    switch (op_) {
        case UnaryOp::Positive:

            break;

        case UnaryOp::Negative:

            stream << "    neg a0, a0" << std::endl;
            break;

        case UnaryOp::BitwiseNot:

            stream << "    not a0, a0" << std::endl;
            break;

        case UnaryOp::LogicalNot:

            stream << "    seqz a0, a0" << std::endl;
            break;

        case UnaryOp::PreIncrement: {

            const VariableReference* var_ref = dynamic_cast<const VariableReference*>(operand_.get());
            if (var_ref) {
                std::string var_name = var_ref->getName();
                int offset = context.getVariableOffset(var_name);


                stream << "    addi a0, a0, 1" << std::endl;


                stream << "    sw a0, " << offset << "(s0)" << std::endl;
            } else {
                std::cerr << "Error: Pre-increment can only be applied to variables" << std::endl;
                exit(1);
            }
            break;
        }

        case UnaryOp::PreDecrement: {

            const VariableReference* var_ref = dynamic_cast<const VariableReference*>(operand_.get());
            if (var_ref) {
                std::string var_name = var_ref->getName();
                int offset = context.getVariableOffset(var_name);


                stream << "    addi a0, a0, -1" << std::endl;


                stream << "    sw a0, " << offset << "(s0)" << std::endl;
            } else {
                std::cerr << "Error: Pre-decrement can only be applied to variables" << std::endl;
                exit(1);
            }
            break;
        }

        case UnaryOp::AddressOf: {

            const VariableReference* var_ref = dynamic_cast<const VariableReference*>(operand_.get());
            if (var_ref) {
                std::string var_name = var_ref->getName();
                int offset = context.getVariableOffset(var_name);


                stream << "    addi a0, s0, " << offset << std::endl;
            } else {
                std::cerr << "Error: Address-of operator can only be applied to variables" << std::endl;
                exit(1);
            }
            break;
        }

        case UnaryOp::Dereference:

            stream << "    lw a0, 0(a0)" << std::endl;
            break;
    }
}

void UnaryExpression::Print(std::ostream& stream) const {
    switch (op_) {
        case UnaryOp::Positive:
            stream << "+";
            break;
        case UnaryOp::Negative:
            stream << "-";
            break;
        case UnaryOp::BitwiseNot:
            stream << "~";
            break;
        case UnaryOp::LogicalNot:
            stream << "!";
            break;
        case UnaryOp::PreIncrement:
            stream << "++";
            break;
        case UnaryOp::PreDecrement:
            stream << "--";
            break;
        case UnaryOp::AddressOf:
            stream << "&";
            break;
        case UnaryOp::Dereference:
            stream << "*";
            break;
    }

    stream << "(";
    operand_->Print(stream);
    stream << ")";
}

void PostfixExpression::EmitRISC(std::ostream& stream, Context& context) const {
    const VariableReference* var_ref = dynamic_cast<const VariableReference*>(operand_.get());
    if (!var_ref) {
        std::cerr << "Error: Postfix operations can only be applied to variables" << std::endl;
        exit(1);
    }

    std::string var_name = var_ref->getName();
    int offset = context.getVariableOffset(var_name);

    stream << "    lw a0, " << offset << "(s0)" << std::endl;

    int temp_reg = context.allocate();
    stream << "    mv s" << temp_reg << ", a0" << std::endl;


    switch (op_) {
        case PostOp::PostIncrement:
            stream << "    addi s" << temp_reg << ", s" << temp_reg << ", 1" << std::endl;
            break;
        case PostOp::PostDecrement:
            stream << "    addi s" << temp_reg << ", s" << temp_reg << ", -1" << std::endl;
            break;
    }


    stream << "    sw s" << temp_reg << ", " << offset << "(s0)" << std::endl;

    context.freeReg(temp_reg);
}

void PostfixExpression::Print(std::ostream& stream) const {
    operand_->Print(stream);
    switch (op_) {
        case PostOp::PostIncrement:
            stream << "++";
            break;
        case PostOp::PostDecrement:
            stream << "--";
            break;
    }


}
} // namespace ast
