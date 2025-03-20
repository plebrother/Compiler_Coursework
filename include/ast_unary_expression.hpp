#pragma once

#include "ast_node.hpp"

namespace ast {


enum class UnaryOp {
    Positive,   // +
    Negative,   // -
    BitwiseNot, // ~
    LogicalNot, // !
    PreIncrement, // ++
    PreDecrement, // --
    AddressOf,  // &
    Dereference // *
};

enum class PostOp {
    PostIncrement, // ++
    PostDecrement, // --

};

class UnaryExpression : public Node {
private:
UnaryOp op_;
    NodePtr operand_;

public:
    UnaryExpression(UnaryOp op, NodePtr operand)
        : op_(op), operand_(std::move(operand)) {}

    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;
};

class PostfixExpression : public Node {
    private:
        PostOp op_;
        NodePtr operand_;

    public:
        PostfixExpression(PostOp op, NodePtr operand)
            : op_(op), operand_(std::move(operand)) {}

        void EmitRISC(std::ostream& stream, Context& context) const override;
        void Print(std::ostream& stream) const override;
    };



} // namespace ast
