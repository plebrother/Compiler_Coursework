#pragma once

#include "ast_node.hpp"

namespace ast {

// CompoundStatement class represents a compound statement in C (a block of code enclosed in braces)
// It now contains a single NodeList that can include both declarations and statements
class CompoundStatement : public Node
{
private:
    NodeListPtr statements_; // The list containing both declarations and statements

public:
    // Constructor that takes a single NodeList containing all statements and declarations
    CompoundStatement(NodeListPtr statements)
        : statements_(std::move(statements)) {}


    // Override methods from Node base class
    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;
};

} // namespace ast
