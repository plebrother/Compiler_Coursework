#include "ast_unary_expression.hpp"
#include "ast_variable.hpp"

namespace ast {

void UnaryExpression::EmitRISC(std::ostream& stream, Context& context) const {
    // 先计算操作数的值（结果在 a0）
    operand_->EmitRISC(stream, context);

    // 根据一元操作符类型执行相应操作
    switch (op_) {
        case UnaryOp::Positive:
            // +x 不做任何操作，x 的值已经在 a0 中
            break;

        case UnaryOp::Negative:
            // -x 对 a0 进行取负操作
            stream << "    neg a0, a0" << std::endl;
            break;

        case UnaryOp::BitwiseNot:
            // ~x 对 a0 进行按位取反
            stream << "    not a0, a0" << std::endl;
            break;

        case UnaryOp::LogicalNot:
            // !x 逻辑非：如果 a0 为 0，结果为 1；否则结果为 0
            stream << "    seqz a0, a0" << std::endl;
            break;

        case UnaryOp::PreIncrement: {
            // ++x 需要增加变量值并存回内存
            const VariableReference* var_ref = dynamic_cast<const VariableReference*>(operand_.get());
            if (var_ref) {
                std::string var_name = var_ref->getName();
                int offset = context.getVariableOffset(var_name);

                // 增加值
                stream << "    addi a0, a0, 1" << std::endl;

                // 存回内存
                stream << "    sw a0, " << offset << "(s0)" << std::endl;
            } else {
                std::cerr << "Error: Pre-increment can only be applied to variables" << std::endl;
                exit(1);
            }
            break;
        }

        case UnaryOp::PreDecrement: {
            // --x 需要减少变量值并存回内存
            const VariableReference* var_ref = dynamic_cast<const VariableReference*>(operand_.get());
            if (var_ref) {
                std::string var_name = var_ref->getName();
                int offset = context.getVariableOffset(var_name);

                // 减少值
                stream << "    addi a0, a0, -1" << std::endl;

                // 存回内存
                stream << "    sw a0, " << offset << "(s0)" << std::endl;
            } else {
                std::cerr << "Error: Pre-decrement can only be applied to variables" << std::endl;
                exit(1);
            }
            break;
        }

        case UnaryOp::AddressOf: {
            // &x 需要获取变量的地址
            const VariableReference* var_ref = dynamic_cast<const VariableReference*>(operand_.get());
            if (var_ref) {
                std::string var_name = var_ref->getName();
                int offset = context.getVariableOffset(var_name);

                // 计算变量地址
                stream << "    addi a0, s0, " << offset << std::endl;
            } else {
                std::cerr << "Error: Address-of operator can only be applied to variables" << std::endl;
                exit(1);
            }
            break;
        }

        case UnaryOp::Dereference:
            // *x 需要读取指针所指向的内存
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
