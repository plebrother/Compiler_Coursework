#include "ast_continue_statement.hpp"

namespace ast {

void ContinueStatement::EmitRISC(std::ostream& stream, Context& context) const {
    // For 'for' loops, continue jumps to the increment expression
    // For 'while' and 'do-while' loops, continue jumps to the condition check

    if (context.isInForLoop()) {
        // Handle continue in for loops
        int loop_continue = context.getCurrentLoopContinue();
        if (loop_continue < 0) {
            std::cerr << "Error: 'continue' statement not within loop" << std::endl;
            exit(1);
        }
        stream << "    j for_continue_" << loop_continue << std::endl;
    } else if (context.isInWhileLoop()) {
        // Handle continue in while loops
        int loop_start = context.getCurrentLoopStart();
        if (loop_start < 0) {
            std::cerr << "Error: 'continue' statement not within loop" << std::endl;
            exit(1);
        }
        stream << "    j while_start_" << loop_start << std::endl;
    } else if (context.isInDoWhileLoop()) {
        // Handle continue in do-while loops
        int loop_start = context.getCurrentLoopStart();
        if (loop_start < 0) {
            std::cerr << "Error: 'continue' statement not within loop" << std::endl;
            exit(1);
        }
        stream << "    j do_while_start_" << loop_start << std::endl;
    } else {
        // Not inside any loop
        std::cerr << "Error: 'continue' statement not within loop" << std::endl;
        exit(1);
    }
}

void ContinueStatement::Print(std::ostream& stream) const {
    stream << "continue;" << std::endl;
}

} // namespace ast
