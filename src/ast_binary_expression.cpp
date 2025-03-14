#include "ast_binary_expression.hpp"

namespace ast {

void BinaryExpression::EmitRISC(std::ostream& stream, Context& context) const
{
    // Allocate registers for left and right expressions
    int left_reg = context.allocate();
    int right_reg = context.allocate();

    // Register names for s-registers
    const std::string left_reg_name = "s" + std::to_string(left_reg);
    const std::string right_reg_name = "s" + std::to_string(right_reg);

    // Emit code for the left expression and store result in left_reg
    left_->EmitRISC(stream, context);
    stream << "mv " << left_reg_name << ", a0" << std::endl;

    // Emit code for the right expression and store result in right_reg
    right_->EmitRISC(stream, context);
    stream << "mv " << right_reg_name << ", a0" << std::endl;

    // Perform operation based on operator type
    switch (op_) {
        case BinaryOp::Add:
            stream << "add a0, " << left_reg_name << ", " << right_reg_name << std::endl;
            break;
        case BinaryOp::Subtract:
            stream << "sub a0, " << left_reg_name << ", " << right_reg_name << std::endl;
            break;
        case BinaryOp::Multiply:
            stream << "mul a0, " << left_reg_name << ", " << right_reg_name << std::endl;
            break;
        case BinaryOp::Divide:
            stream << "div a0, " << left_reg_name << ", " << right_reg_name << std::endl;
            break;
        case BinaryOp::Modulo:
            stream << "rem a0, " << left_reg_name << ", " << right_reg_name << std::endl;
            break;
        case BinaryOp::BitwiseAnd:
            stream << "and a0, " << left_reg_name << ", " << right_reg_name << std::endl;
            break;
        case BinaryOp::BitwiseOr:
            stream << "or a0, " << left_reg_name << ", " << right_reg_name << std::endl;
            break;
        case BinaryOp::BitwiseXor:
            stream << "xor a0, " << left_reg_name << ", " << right_reg_name << std::endl;
            break;
        case BinaryOp::LogicalAnd: {
            // Implement logical AND: if left is 0, result is 0, otherwise result is (right != 0)
            int label_end = context.getNextLabel();
            stream << "beqz " << left_reg_name << ", zero_result_" << label_end << std::endl;
            stream << "snez a0, " << right_reg_name << std::endl;
            stream << "j end_logical_and_" << label_end << std::endl;
            stream << "zero_result_" << label_end << ":" << std::endl;
            stream << "li a0, 0" << std::endl;
            stream << "end_logical_and_" << label_end << ":" << std::endl;
            break;
        }
        case BinaryOp::LogicalOr: {
            // Implement logical OR: if left is non-zero, result is 1, otherwise result is (right != 0)
            int label_end = context.getNextLabel();
            stream << "bnez " << left_reg_name << ", one_result_" << label_end << std::endl;
            stream << "snez a0, " << right_reg_name << std::endl;
            stream << "j end_logical_or_" << label_end << std::endl;
            stream << "one_result_" << label_end << ":" << std::endl;
            stream << "li a0, 1" << std::endl;
            stream << "end_logical_or_" << label_end << ":" << std::endl;
            break;
        }
        case BinaryOp::Equal:
            stream << "xor a0, " << left_reg_name << ", " << right_reg_name << std::endl;
            stream << "seqz a0, a0" << std::endl;
            break;
        case BinaryOp::NotEqual:
            stream << "xor a0, " << left_reg_name << ", " << right_reg_name << std::endl;
            stream << "snez a0, a0" << std::endl;
            break;
        case BinaryOp::LessThan:
            stream << "slt a0, " << left_reg_name << ", " << right_reg_name << std::endl;
            break;
        case BinaryOp::GreaterThan:
            stream << "slt a0, " << right_reg_name << ", " << left_reg_name << std::endl;
            break;
        case BinaryOp::LessEqual:
            stream << "slt a0, " << right_reg_name << ", " << left_reg_name << std::endl;
            stream << "xori a0, a0, 1" << std::endl;
            break;
        case BinaryOp::GreaterEqual:
            stream << "slt a0, " << left_reg_name << ", " << right_reg_name << std::endl;
            stream << "xori a0, a0, 1" << std::endl;
            break;
    }

    // Free the registers
    context.freeReg(left_reg);
    context.freeReg(right_reg);
}

void BinaryExpression::Print(std::ostream& stream) const
{
    stream << "(";
    left_->Print(stream);

    switch (op_) {
        case BinaryOp::Add:
            stream << " + ";
            break;
        case BinaryOp::Subtract:
            stream << " - ";
            break;
        case BinaryOp::Multiply:
            stream << " * ";
            break;
        case BinaryOp::Divide:
            stream << " / ";
            break;
        case BinaryOp::Modulo:
            stream << " % ";
            break;
        case BinaryOp::BitwiseAnd:
            stream << " & ";
            break;
        case BinaryOp::BitwiseOr:
            stream << " | ";
            break;
        case BinaryOp::BitwiseXor:
            stream << " ^ ";
            break;
        case BinaryOp::LogicalAnd:
            stream << " && ";
            break;
        case BinaryOp::LogicalOr:
            stream << " || ";
            break;
        case BinaryOp::Equal:
            stream << " == ";
            break;
        case BinaryOp::NotEqual:
            stream << " != ";
            break;
        case BinaryOp::LessThan:
            stream << " < ";
            break;
        case BinaryOp::GreaterThan:
            stream << " > ";
            break;
        case BinaryOp::LessEqual:
            stream << " <= ";
            break;
        case BinaryOp::GreaterEqual:
            stream << " >= ";
            break;
    }

    right_->Print(stream);
    stream << ")";
}

} // namespace ast
