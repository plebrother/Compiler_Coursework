#pragma once

#include "ast_node.hpp"
#include "ast_context.hpp"

namespace ast {

enum class AssignmentOp {
    Assign,       // =
    AddAssign,    // +=
    SubAssign,    // -=
    MulAssign,    // *=
    DivAssign,    // /=
    ModAssign,    // %=
    AndAssign,    // &=
    OrAssign,     // |=
    XorAssign,    // ^=
    LeftAssign,   // <<=  添加这行
    RightAssign   // >>=  添加这行
};
class AssignmentExpression : public Node {
private:
    NodePtr left_;    // Left-hand side (usually a variable reference)
    NodePtr right_;   // Right-hand side (expression to assign)
    AssignmentOp op_; // Assignment operator type

public:
    AssignmentExpression(NodePtr left, AssignmentOp op, NodePtr right)
        : left_(std::move(left)), right_(std::move(right)), op_(op) {}

    // Convenience constructor for simple '=' assignments
    AssignmentExpression(NodePtr left, NodePtr right)
        : left_(std::move(left)), right_(std::move(right)), op_(AssignmentOp::Assign) {}

    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;
};

} // namespace ast
