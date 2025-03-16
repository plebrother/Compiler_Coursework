#include "ast_function_definition.hpp"
#include "ast_identifier.hpp"
#include <stdexcept>
#include <sstream>

namespace ast {

int FunctionDefinition::calculateStackSize(const Context& context) const {
    // 基本栈大小 = 返回地址(4) + 帧指针(4) + 12个保存寄存器(48)
    int base_size = 56;

    // 为参数预留空间 (4字节对齐)
    int param_space = parameter_list_.size() * 4;

    // 为局部变量预留额外空间 (可以根据compound_statement分析进一步优化)
    int local_var_space = context.getLocalVariablesSize();

    if (local_var_space < 16) {
        local_var_space = 16;
    }

    return base_size + param_space + local_var_space;
}

void FunctionDefinition::EmitRISC(std::ostream& stream, Context& context) const
{
    if (declarator_ == nullptr) {
        std::cerr << "Error: Function declarator is null" << std::endl;
        return;
    }

    // 计算栈空间大小
    int stacksize = calculateStackSize(context);
    context.enterFunctionScope();

    // 生成唯一的函数结束标签
    int func_end_label = context.getNextLabel();
    context.setCurrentFunctionEndLabel(func_end_label);

    // 函数序言部分
    stream << ".text" << std::endl;
    stream << ".globl ";
    declarator_->Print(stream);
    stream << std::endl;

    // 函数标签
    declarator_->EmitRISC(stream, context);

    // 栈帧设置
    stream << "    addi sp, sp, " << -stacksize << std::endl;  // stack space
    stream << "    sw ra, " << stacksize - 4 << "(sp)" << std::endl;  // store ra
    stream << "    sw s0, " << stacksize - 8 << "(sp)" << std::endl;  // store fp
    stream << "    addi s0, sp, " << 4 << std::endl;  // set up new fp

    // 保存被调用者保存寄存器 (s1-s11)
    for (int i = 1; i <= 11; i++) {
        stream << "    sw s" << i << ", " << stacksize - 16 - 4 * i << "(s0)" << std::endl;
    }

    // 处理函数参数 (假设参数通过寄存器a0-a7传递)
    for (size_t i = 0; i < parameter_list_.size() && i < 8; i++) {
        // 将参数从参数寄存器移动到栈内存
        stream << "    sw a" << i << ", " << -4 * (i + 1) << "(s0)" << std::endl;
    }

    // 函数体
    if (compound_statement_ != nullptr) {
        compound_statement_->EmitRISC(stream, context);
    }

    // 函数结束标签 - 注意这个标签现在放在函数体之后
    stream << "func_end_" << func_end_label << ":" << std::endl;

    // 恢复被调用者保存寄存器
    for (int i = 1; i <= 11; i++) {
        stream << "    lw s" << i << ", " << stacksize - 8 - 4 * i << "(s0)" << std::endl;
    }

    // 恢复返回地址和帧指针
    stream << "    lw ra, " << stacksize - 8 << "(s0)" << std::endl;
    stream << "    lw s0, " << stacksize - 12 << "(s0)" << std::endl;
    stream << "    addi sp, sp, " << stacksize << std::endl;  // 释放栈空间

    // 返回
    stream << "    ret" << std::endl;
    stream << std::endl;

    context.exitFunctionScope();
}


void FunctionDefinition::Print(std::ostream& stream) const
{
    // 打印返回类型
    stream << return_type_ << " ";

    // 打印函数名
    if (declarator_ != nullptr) {
        declarator_->Print(stream);
    }

    // 打印参数列表
    stream << "(";

    // 如果有参数，则打印它们
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

    // 如果没有参数，打印void
    if (parameter_list_.empty()) {
        stream << "void";
    }

    stream << ") {" << std::endl;

    // 打印函数体
    if (compound_statement_ != nullptr) {
        compound_statement_->Print(stream);
    }

    stream << "}" << std::endl;
}

} // namespace ast
