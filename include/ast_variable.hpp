#pragma once

#include "ast_node.hpp"

namespace ast {

class VariableReference : public Node {
private:
    std::string name_;

public:
    VariableReference(const std::string& name) : name_(name) {}

    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;
    const std::string& getName() const { return name_; }
};

} // namespace ast
