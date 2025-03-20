#pragma once

#include "ast_node.hpp"

namespace ast {

class FunctionCall : public Node {
private:
    std::string function_name_;
    NodeListPtr arguments_;

public:

    FunctionCall(std::string function_name, NodeListPtr arguments = nullptr)
        : function_name_(function_name), arguments_(std::move(arguments)) {}



    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;
};

} // namespace ast
