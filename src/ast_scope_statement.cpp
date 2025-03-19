#include "ast_scope_statement.hpp"

namespace ast {

void ScopeStatement::EmitRISC(std::ostream& stream, Context& context) const
{
    // Create a new scope for variables declared within this block

    context.enterScope();
    // Emit code for all statements (including declarations)
    if (statements_ != nullptr) {
        statements_->EmitRISC(stream, context);
    }

    // Exit the scope when leaving the block
    context.exitScope();
}

void ScopeStatement::Print(std::ostream& stream) const
{
    stream << "{" << std::endl;

    // Print all statements (including declarations)
    if (statements_ != nullptr) {
        statements_->Print(stream);
    }

    stream << "}" << std::endl;
}

} // namespace ast
