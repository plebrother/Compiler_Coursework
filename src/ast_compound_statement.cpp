#include "ast_compound_statement.hpp"

namespace ast {

void CompoundStatement::EmitRISC(std::ostream& stream, Context& context) const
{
    // Create a new scope for variables declared within this block
    context.enterScope();

    // First emit code for declarations (if any)
    if (declarations_ != nullptr) {
        declarations_->EmitRISC(stream, context);
    }

    // Then emit code for statements (if any)
    if (statements_ != nullptr) {
        statements_->EmitRISC(stream, context);
    }

    // Exit the scope when leaving the block
    context.exitScope();
}

void CompoundStatement::Print(std::ostream& stream) const
{
    stream << "{" << std::endl;

    // Print declarations if available
    if (declarations_ != nullptr) {
        declarations_->Print(stream);
    }

    // Print statements if available
    if (statements_ != nullptr) {
        statements_->Print(stream);
    }

    stream << "}" << std::endl;
}

} // namespace ast
