#include "ast_while_loop.hpp"

namespace ast {

void WhileStatement::EmitRISC(std::ostream& stream, Context& context) const {
    // Save current loop labels to support nested loops with break/continue
    int old_loop_start = context.getCurrentLoopStart();
    int old_loop_end = context.getCurrentLoopEnd();

    // Generate labels for loop
    int loop_start = context.getNextLabel();
    int loop_end = context.getNextLabel();

    // Set current loop labels for break/continue
    context.setCurrentLoopStart(loop_start);
    context.setCurrentLoopEnd(loop_end);

    // Set loop type
    context.enterWhileLoop();

    // Loop start label
    stream << "while_start_" << loop_start << ":" << std::endl;

    // Evaluate condition (result in a0)
    if (condition_ != nullptr) {
        condition_->EmitRISC(stream, context);

        // If condition is false (a0 == 0), exit the loop
        stream << "    beqz a0, while_end_" << loop_end << std::endl;
    }

    // Execute loop body
    if (body_ != nullptr) {
        body_->EmitRISC(stream, context);
    }

    // Jump back to start to reevaluate condition
    stream << "    j while_start_" << loop_start << std::endl;

    // Loop end label
    stream << "while_end_" << loop_end << ":" << std::endl;

    // Restore previous loop labels
    context.setCurrentLoopStart(old_loop_start);
    context.setCurrentLoopEnd(old_loop_end);

    // Exit this loop scope
    context.exitLoop();
}

void WhileStatement::Print(std::ostream& stream) const {
    stream << "while (";
    if (condition_ != nullptr) {
        condition_->Print(stream);
    }
    stream << ") {" << std::endl;
    if (body_ != nullptr) {
        body_->Print(stream);
    }
    stream << "}" << std::endl;
}

} // namespace ast
