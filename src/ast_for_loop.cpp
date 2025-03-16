#include "ast_for_loop.hpp"

namespace ast {

    void ForStatement::EmitRISC(std::ostream& stream, Context& context) const {
        // 保存当前的循环标签
        int old_loop_start = context.getCurrentLoopStart();
        int old_loop_end = context.getCurrentLoopEnd();
        int old_loop_continue = context.getCurrentLoopContinue();

        // 生成循环的标签
        int loop_start = context.getNextLabel();
        int loop_end = context.getNextLabel();
        int loop_continue = context.getNextLabel();

        // 设置当前循环标签
        context.setCurrentLoopStart(loop_start);
        context.setCurrentLoopEnd(loop_end);
        context.setCurrentLoopContinue(loop_continue);

        // 设置循环类型
        context.enterForLoop();

        // 初始化表达式（如果存在）
        if (init_ != nullptr) {
            init_->EmitRISC(stream, context);
        }

        // 打印明显的循环开始标签和注释
        stream << "    # For loop begins" << std::endl;
        stream << "for_start_" << loop_start << ":" << std::endl;

        // 条件检查（如果存在）
        if (condition_ != nullptr) {
            stream << "    # Loop condition" << std::endl;
            condition_->EmitRISC(stream, context);
            stream << "    beqz a0, for_end_" << loop_end << std::endl;
        }

        // 执行循环体
        stream << "    # Loop body" << std::endl;
        if (body_ != nullptr) {
            body_->EmitRISC(stream, context);
        }

        // Continue标签
        stream << "for_continue_" << loop_continue << ":" << std::endl;

        // 递增表达式（如果存在）
        stream << "    # Loop increment" << std::endl;
        if (increment_ != nullptr) {
            increment_->EmitRISC(stream, context);
        }

        // 跳回条件检查
        stream << "    j for_start_" << loop_start << std::endl;

        // 循环结束标签
        stream << "for_end_" << loop_end << ":" << std::endl;
        stream << "    # For loop ends" << std::endl;

        // 恢复之前的循环标签
        context.setCurrentLoopStart(old_loop_start);
        context.setCurrentLoopEnd(old_loop_end);
        context.setCurrentLoopContinue(old_loop_continue);

        // 退出此循环作用域
        context.exitLoop();
    }

void ForStatement::Print(std::ostream& stream) const {
    stream << "for (";

    // Print initialization
    if (init_ != nullptr) {
        init_->Print(stream);
    } else {
        stream << " ";
    }
    stream << "; ";

    // Print condition
    if (condition_ != nullptr) {
        condition_->Print(stream);
    }
    stream << "; ";

    // Print increment
    if (increment_ != nullptr) {
        increment_->Print(stream);
    }
    stream << ") {" << std::endl;

    // Print body
    if (body_ != nullptr) {
        body_->Print(stream);
    }
    stream << "}" << std::endl;
}

} // namespace ast
