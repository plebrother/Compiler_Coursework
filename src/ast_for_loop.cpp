#include "ast_for_loop.hpp"

namespace ast {

void ForStatement::EmitRISC(std::ostream& stream, Context& context) const {
    // Save current loop labels to support nested loops
    int old_loop_start = context.getCurrentLoopStart();
    int old_loop_end = context.getCurrentLoopEnd();
    int old_loop_continue = context.getCurrentLoopContinue();

    // Generate labels for loop
    int loop_start = context.getNextLabel();
    int loop_end = context.getNextLabel();
    int loop_continue = context.getNextLabel();

    // Set current loop labels for break/continue
    context.setCurrentLoopStart(loop_start);
    context.setCurrentLoopEnd(loop_end);
    context.setCurrentLoopContinue(loop_continue);

    // Set loop type
    context.enterForLoop();

    // Initialization expression (if exists)
    if (init_ != nullptr) {
        init_->EmitRISC(stream, context);
    }

    // Loop start label (condition check)
    stream << "for_start_" << loop_start << ":" << std::endl;

    // Condition check (if exists)
    if (condition_ != nullptr) {
        condition_->EmitRISC(stream, context);
        stream << "    beqz a0, for_end_" << loop_end << std::endl;
    }

    // Execute loop body
    if (body_ != nullptr) {
        body_->EmitRISC(stream, context);
    }

    // Continue label (for 'continue' statements)
    stream << "for_continue_" << loop_continue << ":" << std::endl;

    // Increment expression (if exists)
    if (increment_ != nullptr) {
        increment_->EmitRISC(stream, context);
    }

    // Jump back to condition check
    stream << "    j for_start_" << loop_start << std::endl;

    // Loop end label
    stream << "for_end_" << loop_end << ":" << std::endl;

    // Restore previous loop labels
    context.setCurrentLoopStart(old_loop_start);
    context.setCurrentLoopEnd(old_loop_end);
    context.setCurrentLoopContinue(old_loop_continue);

    // Exit this loop scope
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
