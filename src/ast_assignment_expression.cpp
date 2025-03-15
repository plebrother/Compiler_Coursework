#include "ast_assignment_expression.hpp"
#include "ast_identifier.hpp"
#include <stdexcept>

namespace ast {

void AssignmentExpression::EmitRISC(std::ostream& stream, Context& context) const
{
    // For simple assignments, we need to:
    // 1. Evaluate the right-hand side expression (result will be in a0)
    // 2. Store the result into the memory location of the left-hand side variable

    // First, evaluate the right-hand side expression
    right_->EmitRISC(stream, context);

    // We may need an extra register for compound assignments
    int temp_reg = -1;
    if (op_ != AssignmentOp::Assign) {
        temp_reg = context.allocate();
        stream << "    mv s" << temp_reg << ", a0" << std::endl;
    }

    // Get the variable name from the left side
    // This assumes the left side is a direct variable reference (which is typically the case)
    // For more complex lvalues (like array elements), this would need to be extended
    const Identifier* id = dynamic_cast<const Identifier*>(left_.get());
    if (!id) {
        std::cerr << "Error: Left side of assignment is not a variable" << std::endl;
        exit(1);
    }

    std::string var_name = id->getName();

    // Check if the variable exists
    if (!context.hasVariable(var_name)) {
        std::cerr << "Error: Undefined variable '" << var_name << "'" << std::endl;
        exit(1);
    }

    // Get the variable's stack offset
    int offset = context.getVariableOffset(var_name);

    // For compound assignments (+=, -=, etc.), we need to:
    // 1. Load the current value of the variable
    // 2. Perform the operation
    // 3. Store the result back
    if (op_ != AssignmentOp::Assign) {
        // Load current value
        stream << "    lw a0, " << offset << "(s0)" << std::endl;

        // Perform the operation
        switch (op_) {
            case AssignmentOp::AddAssign:
                stream << "    add a0, a0, s" << temp_reg << std::endl;
                break;
            case AssignmentOp::SubAssign:
                stream << "    sub a0, a0, s" << temp_reg << std::endl;
                break;
            case AssignmentOp::MulAssign:
                stream << "    mul a0, a0, s" << temp_reg << std::endl;
                break;
            case AssignmentOp::DivAssign:
                stream << "    div a0, a0, s" << temp_reg << std::endl;
                break;
            case AssignmentOp::ModAssign:
                stream << "    rem a0, a0, s" << temp_reg << std::endl;
                break;
            case AssignmentOp::AndAssign:
                stream << "    and a0, a0, s" << temp_reg << std::endl;
                break;
            case AssignmentOp::OrAssign:
                stream << "    or a0, a0, s" << temp_reg << std::endl;
                break;
            case AssignmentOp::XorAssign:
                stream << "    xor a0, a0, s" << temp_reg << std::endl;
                break;
            case AssignmentOp::LeftAssign:
                stream << "    sll a0, a0, s" << temp_reg << std::endl;
                break;
            case AssignmentOp::RightAssign:
                stream << "    srl a0, a0, s" << temp_reg << std::endl;
                break;
            default:
                std::cerr << "Error: Unknown compound assignment operator" << std::endl;
                exit(1);
        }

        // Free the temporary register
        context.freeReg(temp_reg);
    }

    // Store the result (which is in a0) to the variable's location
    stream << "    sw a0, " << offset << "(s0)" << std::endl;
}

void AssignmentExpression::Print(std::ostream& stream) const
{
    left_->Print(stream);

    switch (op_) {
        case AssignmentOp::Assign:
            stream << " = ";
            break;
        case AssignmentOp::AddAssign:
            stream << " += ";
            break;
        case AssignmentOp::SubAssign:
            stream << " -= ";
            break;
        case AssignmentOp::MulAssign:
            stream << " *= ";
            break;
        case AssignmentOp::DivAssign:
            stream << " /= ";
            break;
        case AssignmentOp::ModAssign:
            stream << " %= ";
            break;
        case AssignmentOp::AndAssign:
            stream << " &= ";
            break;
        case AssignmentOp::OrAssign:
            stream << " |= ";
            break;
        case AssignmentOp::XorAssign:
            stream << " ^= ";
            break;
        case AssignmentOp::LeftAssign:  // 添加这个case
            stream << " <<= ";
            break;
        case AssignmentOp::RightAssign: // 添加这个case
            stream << " >>= ";
            break;
    }

    right_->Print(stream);
}
} // namespace ast
