#include "ast_variable_declaration.hpp"
#include "ast_identifier.hpp"

namespace ast {

void VariableDeclaration::EmitRISC(std::ostream& stream, Context& context) const
{

    const Identifier* id = dynamic_cast<const Identifier*>(identifier_.get());
    if (!id) {
        std::cerr << "Error: Expected identifier in variable declaration" << std::endl;
        exit(1);
    }
    std::string var_name = id->getName();


    if (is_global_) {
        // Global variable declaration (goes in .data section)
        stream << ".data" << std::endl;
        identifier_->EmitRISC(stream,context);
        stream <<  ":" << std::endl;

        if (initializer_ != nullptr) {
            // Handle global initialization with constant
            stream << ".word ";
            initializer_->EmitRISC(stream, context);
            stream << std::endl;
        } else {
            // Zero-initialized
            stream << ".zero 4" << std::endl;
        }
    } else {
        // Local variable declaration (on the stack)
        // Add to symbol table and get offset
        std::string var_name = id->getName();
        int offset = context.addLocalVariable(var_name);
        stream << "addi sp,sp,-4"<< std::endl;

        // If there's an initializer, evaluate and store the result
        if (initializer_ != nullptr) {
            // Evaluate initializer expression (result will be in a0)
            initializer_->EmitRISC(stream, context);
            // Store result to variable's stack location
            stream << "    sw a0, " << offset << "(s0)" << std::endl;
        }
    }
}

void VariableDeclaration::Print(std::ostream& stream) const
{
    // For debugging/pretty-printing
    stream << type_specifier_ << " ";
    identifier_->Print(stream);

    if (initializer_ != nullptr) {
        stream << " = ";
        initializer_->Print(stream);
    }

    stream << ";" << std::endl;
}

} // namespace ast
