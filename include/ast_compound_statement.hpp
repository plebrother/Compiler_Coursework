#pragma once

#include "ast_node.hpp"

namespace ast {

// CompoundStatement class represents a compound statement in C (a block of code enclosed in braces)
// It can contain both declarations and statements
class CompoundStatement : public Node
{
private:
    NodePtr declarations_; // The list of declarations
    NodePtr statements_;   // The list of statements

public:
    // Constructor with both declarations and statements
    CompoundStatement(NodePtr declarations, NodePtr statements)
        : declarations_(std::move(declarations)),
          statements_(std::move(statements)) {}

    // Constructor with only statements
    explicit CompoundStatement(NodePtr statements)
        : declarations_(nullptr),
          statements_(std::move(statements)) {}

    // Default constructor (empty block)
    CompoundStatement()
        : declarations_(nullptr),
          statements_(nullptr) {}

    // Override methods from Node base class
    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;
};

} // namespace ast
