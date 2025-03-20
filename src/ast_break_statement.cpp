#include "ast_break_statement.hpp"

namespace ast {

void BreakStatement::EmitRISC(std::ostream& stream, Context& context) const {
    // Get current loop end label
    int loop_end = context.getCurrentLoopEnd();

    if (loop_end < 0) {
        std::cerr << "Error: 'break' statement not within loop or switch" << std::endl;
        exit(1);
    }

    // Jump to the end of the current loop
    stream << "    j ";

    // Determine the appropriate prefix based on the current loop type
    if (context.isInForLoop()) {
        stream << "for_end_";
    } else if (context.isInWhileLoop()) {
        stream << "while_end_";
    } else if (context.isInDoWhileLoop()) {
        stream << "do_while_end_";
    } else {
        stream << "switch_end_"; // For future switch statements
    }

    stream << loop_end << std::endl;
}

void BreakStatement::Print(std::ostream& stream) const {
    stream << "break;" << std::endl;
}

} // namespace ast
