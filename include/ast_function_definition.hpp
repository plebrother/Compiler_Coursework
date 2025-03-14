#pragma once

#include "ast_node.hpp"
#include "ast_type_specifier.hpp"
#include <string>
#include <vector>

namespace ast {

class FunctionDefinition : public Node
{
private:
    const TypeSpecifier return_type_;
    NodePtr declarator_;
    NodePtr compound_statement_;
    std::vector<NodePtr> parameter_list_; // 新增：函数参数列表

    // 计算函数所需栈空间大小的辅助方法
    int calculateStackSize() const;

public:
    // 更新构造函数以支持参数列表
    FunctionDefinition(TypeSpecifier return_type,
                      NodePtr declarator,
                      NodePtr compound_statement,
                      std::vector<NodePtr> parameter_list = {})
        : return_type_(return_type),
          declarator_(std::move(declarator)),
          compound_statement_(std::move(compound_statement)),
          parameter_list_(std::move(parameter_list)) {};

    void EmitRISC(std::ostream& stream, Context& context) const override;
    void Print(std::ostream& stream) const override;

};

} // namespace ast
