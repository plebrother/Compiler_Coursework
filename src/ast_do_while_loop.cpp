#include "ast_do_while_loop.hpp"

namespace ast {

void DoWhileStatement::EmitRISC(std::ostream& stream, Context& context) const {
    // Save current loop labels
    int old_loop_start = context.getCurrentLoopStart();
    int old_loop_end = context.getCurrentLoopEnd();

    // Generate labels for loop
    int loop_start = context.getNextLabel();
    int loop_end = context.getNextLabel();

    // Set current loop labels for break/continue
    context.setCurrentLoopStart(loop_start);
    context.setCurrentLoopEnd(loop_end);

    // Set loop type
    context.enterDoWhileLoop();

    // Loop start label
    stream << "do_while_start_" << loop_start << ":" << std::endl;

    // Execute loop body first
    if (body_ != nullptr) {
        body_->EmitRISC(stream, context);
    }

    // Evaluate condition (result in a0)
    if (condition_ != nullptr) {
        condition_->EmitRISC(stream, context);
    } else {
        // If no condition is provided, default to infinite loop (condition is true)
        stream << "    li a0, 1" << std::endl;
    }

    // If condition is true (a0 != 0), loop back
    stream << "    bnez a0, do_while_start_" << loop_start << std::endl;

    // Loop end label
    stream << "do_while_end_" << loop_end << ":" << std::endl;

    // Restore previous loop labels
    context.setCurrentLoopStart(old_loop_start);
    context.setCurrentLoopEnd(old_loop_end);

    // Exit this loop scope
    context.exitLoop();
}

void DoWhileStatement::Print(std::ostream& stream) const {
    stream << "do {" << std::endl;
    if (body_ != nullptr) {
        body_->Print(stream);
    }
    stream << "} while (";
    if (condition_ != nullptr) {
        condition_->Print(stream);
    }
    stream << ");" << std::endl;
}

} // namespace ast
