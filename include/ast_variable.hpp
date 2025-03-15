#pragma once

#include "ast_node.hpp"

namespace ast {

class VariableReference : public Node {
private:
    std::string name_;

public:
    VariableReference(const std::string& name) : name_(name) {}

    void EmitRISC(std::ostream& stream, Context& context) const override {
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

    void Print(std::ostream& stream) const override {
        stream << name_;
    }
};

} // namespace ast
