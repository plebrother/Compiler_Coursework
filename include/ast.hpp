#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "ast_direct_declarator.hpp"
#include "ast_function_definition.hpp"
#include "ast_identifier.hpp"
#include "ast_jump_statement.hpp"
#include "ast_declare_statement.hpp"
#include "ast_node.hpp"
#include "ast_type_specifier.hpp"
#include "ast_constant.hpp"
#include "ast_context.hpp"
#include "ast_binary_expression.hpp"
#include "ast_variable_declaration.hpp"
#include "ast_variable.hpp"
#include "ast_assignment_expression.hpp"
#include "ast_if_statement.hpp"
#include "ast_for_loop.hpp"
#include "ast_while_loop.hpp"
#include "ast_continue_statement.hpp"
#include "ast_break_statement.hpp"
#include "ast_do_while_loop.hpp"
#include "ast_compound_statement.hpp"
#include "ast_scope_statement.hpp"

ast::NodePtr ParseAST(std::string file_name);
