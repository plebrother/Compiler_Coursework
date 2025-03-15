#include "ast_variable.hpp"

namespace ast {

void VariableReference::EmitRISC(std::ostream& stream, Context& context) const {
    // Check if variable exists
    if (!context.hasVariable(name_)) {
        std::cerr << "Error: Undefined variable '" << name_ << "'" << std::endl;
        exit(1);
    }

    // Get the offset of the variable
    int offset = context.getVariableOffset(name_);

    // Load the variable value into a0
    stream << "    lw a0, " << offset << "(s0)" << std::endl;
}

void VariableReference::Print(std::ostream& stream) const {
    stream << name_;
}

} // namespace ast
