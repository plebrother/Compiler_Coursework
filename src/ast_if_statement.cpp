#include "ast_if_statement.hpp"

namespace ast {

void IfStatement::EmitRISC(std::ostream& stream, Context& context) const {
    // Generate unique labels for branches
    int else_label = context.getNextLabel();
    int end_label = context.getNextLabel();

    // Evaluate the condition (result will be in a0)
    condition_->EmitRISC(stream, context);

    // For RISC-V, we need to check if a0 is non-zero
    // In C, any non-zero value is considered true

    if (else_branch_) {
        // If condition is false (a0 == 0), jump to else branch
        stream << "    beqz a0, else_" << else_label << std::endl;

        // Execute if-branch
        if (if_branch_ != nullptr) {
            if_branch_->EmitRISC(stream, context);
        }

        // Jump to end to skip else branch
        stream << "    j end_if_" << end_label << std::endl;

        // Else branch with label
        stream << "else_" << else_label << ":" << std::endl;
        else_branch_->EmitRISC(stream, context);
    } else {
        // No else branch, just skip the if-branch when condition is false
        stream << "    beqz a0, end_if_" << end_label << std::endl;

        // Execute if-branch
        if (if_branch_ != nullptr) {
            if_branch_->EmitRISC(stream, context);
        }
    }

    // End of if statement
    stream << "end_if_" << end_label << ":" << std::endl;
}

void IfStatement::Print(std::ostream& stream) const {
    stream << "if (";
    condition_->Print(stream);
    stream << ") {" << std::endl;
    if (if_branch_ != nullptr) {
        if_branch_->Print(stream);
    }
    stream << "}" << std::endl;

    if (else_branch_) {
        stream << "else {" << std::endl;
        else_branch_->Print(stream);
        stream << "}" << std::endl;
    }
}

} // namespace ast
