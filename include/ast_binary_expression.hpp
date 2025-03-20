#pragma once

#include "ast_node.hpp"

namespace ast {

enum class BinaryOp {
    Add,        // +
    Subtract,   // -
    Multiply,   // *
    Divide,     // /
    Modulo,     // %
    BitwiseAnd, // &
    BitwiseOr,  // |
    BitwiseXor, // ^
    LogicalAnd, // &&
    LogicalOr,  // ||
    Equal,      // ==
    NotEqual,   // !=
    LessThan,   // <
    GreaterThan,// >
    LessEqual,  // <=
    GreaterEqual,// >=
    LeftShift,  // <<
    RightShift  // >>
};

class BinaryExpression : public Node
{
private:
    NodePtr left_;
    NodePtr right_;
    BinaryOp op_;

public:
    BinaryExpression(NodePtr left, BinaryOp op, NodePtr right)
        : left_(std::move(left)), right_(std::move(right)), op_(op) {}

    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;
};

} // namespace ast
