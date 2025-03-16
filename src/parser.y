// Adapted from: https://www.lysator.liu.se/c/ANSI-C-grammar-y.html

// TODO: you can either continue adding to this grammar file or
// rename parser_full.y to parser.y once you're happy with
// how this example works.

%code requires{
    #include "ast.hpp"

	using namespace ast;

    extern Node* g_root;
    extern FILE* yyin;
    int yylex(void);
    void yyerror(const char*);
	int yylex_destroy(void);
}

%union{
  Node*				node;
  NodeList*			node_list;
  int          		number_int;
  double       		number_float;
  std::string*		string;
  TypeSpecifier 	type_specifier;
  yytokentype  		token;
  int             assign_op;
}

%token IDENTIFIER INT_CONSTANT FLOAT_CONSTANT STRING_LITERAL
%token PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP AND_OP OR_OP
%token MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN XOR_ASSIGN OR_ASSIGN
%token TYPE_NAME TYPEDEF EXTERN STATIC AUTO REGISTER SIZEOF
%token CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE CONST VOLATILE VOID
%token STRUCT UNION ENUM ELLIPSIS
%token CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN

%type <node> translation_unit external_declaration function_definition primary_expression postfix_expression argument_expression_list
%type <node> unary_expression cast_expression multiplicative_expression additive_expression shift_expression relational_expression
%type <node> equality_expression and_expression exclusive_or_expression inclusive_or_expression logical_and_expression logical_or_expression
%type <node> conditional_expression assignment_expression expression constant_expression declaration
%type <node> init_declarator struct_specifier struct_declaration_list struct_declaration specifier_qualifier_list struct_declarator_list
%type <node> struct_declarator enum_specifier enumerator_list enumerator declarator direct_declarator pointer
%type <node> identifier_list type_name abstract_declarator direct_abstract_declarator initializer initializer_list statement labeled_statement
%type <node> compound_statement expression_statement selection_statement iteration_statement jump_statement

%type <node_list> statement_list parameter_list parameter_declaration init_declarator_list declaration_list

%type <string> unary_operator storage_class_specifier
%type <assign_op> assignment_operator

%type <number_int> INT_CONSTANT STRING_LITERAL
%type <number_float> FLOAT_CONSTANT
%type <string> IDENTIFIER
%type <type_specifier> type_specifier
// TODO: Make a better type for this (only needed for advanced features)
%type <type_specifier> declaration_specifiers


%start ROOT
%%

ROOT
  : translation_unit { g_root = $1; }

translation_unit
	: external_declaration { $$ = $1; }
	| translation_unit external_declaration
	;

external_declaration
	: function_definition { $$ = $1; }
	| declaration
	;

function_definition
	: declaration_specifiers declarator declaration_list compound_statement
	| declaration_specifiers declarator compound_statement {
		$$ = new FunctionDefinition($1, NodePtr($2), NodePtr($3));
	}
	| declarator declaration_list compound_statement
	| declarator compound_statement
	;


primary_expression
	: IDENTIFIER { $$ = new VariableReference(*$1);
					delete $1;
	}
	| INT_CONSTANT {
		$$ = new IntConstant($1);
	}
    | FLOAT_CONSTANT
	| STRING_LITERAL
	| '(' expression ')'
	;

postfix_expression
	: primary_expression
	| postfix_expression '[' expression ']'
	| postfix_expression '(' ')'
	| postfix_expression '(' argument_expression_list ')'
	| postfix_expression '.' IDENTIFIER
	| postfix_expression PTR_OP IDENTIFIER
	| postfix_expression INC_OP
	| postfix_expression DEC_OP
	;

argument_expression_list
	: assignment_expression
	| argument_expression_list ',' assignment_expression
	;

unary_expression
	: postfix_expression
	| INC_OP unary_expression
	| DEC_OP unary_expression
	| unary_operator cast_expression
	| SIZEOF unary_expression
	| SIZEOF '(' type_name ')'
	;

unary_operator
	: '&'
	| '*'
	| '+'
	| '-'
	| '~'
	| '!'
	;

cast_expression
	: unary_expression
	| '(' type_name ')' cast_expression
	;

multiplicative_expression
	: cast_expression
	| multiplicative_expression '*' cast_expression {
		$$ = new BinaryExpression(NodePtr($1), BinaryOp::Multiply, NodePtr($3));
	}
	| multiplicative_expression '/' cast_expression {
		$$ = new BinaryExpression(NodePtr($1), BinaryOp::Divide, NodePtr($3));
	}
	| multiplicative_expression '%' cast_expression {
		$$ = new BinaryExpression(NodePtr($1), BinaryOp::Modulo, NodePtr($3));
	}
	;

additive_expression
	: multiplicative_expression
	| additive_expression '+' multiplicative_expression {
		$$ = new BinaryExpression(NodePtr($1), BinaryOp::Add, NodePtr($3));
	}
	| additive_expression '-' multiplicative_expression {
		$$ = new BinaryExpression(NodePtr($1), BinaryOp::Subtract, NodePtr($3));
	}
	;

shift_expression
	: additive_expression
	| shift_expression LEFT_OP additive_expression
	| shift_expression RIGHT_OP additive_expression
	;

relational_expression
	: shift_expression
	| relational_expression '<' shift_expression {
		$$ = new BinaryExpression(NodePtr($1), BinaryOp::LessThan, NodePtr($3));
	}
	| relational_expression '>' shift_expression {
		$$ = new BinaryExpression(NodePtr($1), BinaryOp::GreaterThan, NodePtr($3));
	}
	| relational_expression LE_OP shift_expression {
		$$ = new BinaryExpression(NodePtr($1), BinaryOp::LessEqual, NodePtr($3));
	}
	| relational_expression GE_OP shift_expression {
		$$ = new BinaryExpression(NodePtr($1), BinaryOp::GreaterEqual, NodePtr($3));
	}
	;

equality_expression
	: relational_expression
	| equality_expression EQ_OP relational_expression {
		$$ = new BinaryExpression(NodePtr($1), BinaryOp::Equal, NodePtr($3));
	}
	| equality_expression NE_OP relational_expression{
		$$ = new BinaryExpression(NodePtr($1), BinaryOp::NotEqual, NodePtr($3));
	}
	;

and_expression
	: equality_expression
	| and_expression '&' equality_expression {
		$$ = new BinaryExpression(NodePtr($1), BinaryOp::BitwiseAnd, NodePtr($3));
	}
	;

exclusive_or_expression
	: and_expression
	| exclusive_or_expression '^' and_expression {
		$$ = new BinaryExpression(NodePtr($1), BinaryOp::BitwiseXor, NodePtr($3));
	}
	;

inclusive_or_expression
	: exclusive_or_expression
	| inclusive_or_expression '|' exclusive_or_expression {
		$$ = new BinaryExpression(NodePtr($1), BinaryOp::BitwiseOr, NodePtr($3));
	}
	;

logical_and_expression
	: inclusive_or_expression
	| logical_and_expression AND_OP inclusive_or_expression {
		$$ = new BinaryExpression(NodePtr($1), BinaryOp::LogicalAnd, NodePtr($3));
	}
	;

logical_or_expression
	: logical_and_expression
	| logical_or_expression OR_OP logical_and_expression {
		$$ = new BinaryExpression(NodePtr($1), BinaryOp::LogicalOr, NodePtr($3));
	}
	;

conditional_expression
	: logical_or_expression
	| logical_or_expression '?' expression ':' conditional_expression
	;

assignment_expression
    : conditional_expression
    | unary_expression assignment_operator assignment_expression {
		AssignmentOp op;
        switch ($2) {
            case 0: op = AssignmentOp::Assign; break;
            case 1: op = AssignmentOp::MulAssign; break;
            case 2: op = AssignmentOp::DivAssign; break;
            case 3: op = AssignmentOp::ModAssign; break;
            case 4: op = AssignmentOp::AddAssign; break;
            case 5: op = AssignmentOp::SubAssign; break;
            case 6: op = AssignmentOp::AndAssign; break;
            case 7: op = AssignmentOp::XorAssign; break;
            case 8: op = AssignmentOp::OrAssign; break;

            default: std::cerr << "Unknown assignment operator" << std::endl; exit(1);
        }
        $$ = new AssignmentExpression(NodePtr($1), op, NodePtr($3));
    }
    ;

assignment_operator
    : '='         { $$ = 0; }  // AssignmentOp::Assign
    | ADD_ASSIGN  { $$ = 1; }  // AssignmentOp::AddAssign
    | SUB_ASSIGN  { $$ = 2; }  // AssignmentOp::SubAssign
    | MUL_ASSIGN  { $$ = 3; }  // AssignmentOp::MulAssign
    | DIV_ASSIGN  { $$ = 4; }  // AssignmentOp::DivAssign
    | MOD_ASSIGN  { $$ = 5; }  // AssignmentOp::ModAssign
    | AND_ASSIGN  { $$ = 6; }  // AssignmentOp::AndAssign
    | OR_ASSIGN   { $$ = 7; }  // AssignmentOp::OrAssign
    | XOR_ASSIGN  { $$ = 8; }  // AssignmentOp::XorAssign
    | LEFT_ASSIGN { $$ = 9; }
    | RIGHT_ASSIGN { $$ = 10; }
    ;
expression
	: assignment_expression
	| expression ',' assignment_expression
	;

constant_expression
	: conditional_expression
	;

declaration
	: declaration_specifiers ';' { $$ = new DeclareStatement($1,nullptr);}
	| declaration_specifiers init_declarator_list ';' { $$ = new DeclareStatement($1,NodePtr($2));}
	;

declaration_specifiers
	: storage_class_specifier
	| storage_class_specifier declaration_specifiers
	| type_specifier { $$ = $1; }
	| type_specifier declaration_specifiers
	;

init_declarator_list
	: init_declarator
	| init_declarator_list ',' init_declarator
	;

init_declarator
	: declarator { $$ = new VariableDeclaration(TypeSpecifier::INT,NodePtr($1), nullptr); }
	| declarator '=' initializer { $$ = new VariableDeclaration(TypeSpecifier::INT,NodePtr($1), NodePtr($3));} //assume declaration_specifiers = type_specifier = INT
	;

storage_class_specifier
	: TYPEDEF
	| EXTERN
	| STATIC
	| AUTO
	| REGISTER
	;

type_specifier
	: VOID
	| CHAR
	| SHORT
	| INT { $$ = TypeSpecifier::INT; }
	| LONG
	| FLOAT
	| DOUBLE
	| SIGNED
	| UNSIGNED
  | struct_specifier
	| enum_specifier
	| TYPE_NAME
	;

struct_specifier
	: STRUCT IDENTIFIER '{' struct_declaration_list '}'
	| STRUCT '{' struct_declaration_list '}'
	| STRUCT IDENTIFIER
	;

struct_declaration_list
	: struct_declaration
	| struct_declaration_list struct_declaration
	;

struct_declaration
	: specifier_qualifier_list struct_declarator_list ';'
	;

specifier_qualifier_list
	: type_specifier specifier_qualifier_list
	| type_specifier
	;

struct_declarator_list
	: struct_declarator
	| struct_declarator_list ',' struct_declarator
	;

struct_declarator
	: declarator
	| ':' constant_expression
	| declarator ':' constant_expression
	;

enum_specifier
	: ENUM '{' enumerator_list '}'
	| ENUM IDENTIFIER '{' enumerator_list '}'
	| ENUM IDENTIFIER
	;

enumerator_list
	: enumerator
	| enumerator_list ',' enumerator
	;

enumerator
	: IDENTIFIER
	| IDENTIFIER '=' constant_expression
	;

declarator
	: pointer direct_declarator
	| direct_declarator { $$ = $1; }
	;

direct_declarator
	: IDENTIFIER {
		$$ = new Identifier(std::move(*$1));
		delete $1;
	}
	| '(' declarator ')'
	| direct_declarator '[' constant_expression ']'
	| direct_declarator '[' ']'
	| direct_declarator '(' parameter_list ')'
	| direct_declarator '(' identifier_list ')'
	| direct_declarator '(' ')' {
		$$ = new DirectDeclarator(NodePtr($1));
	}
	;

pointer
	: '*'
	| '*' pointer
	;

parameter_list
	: parameter_declaration
	| parameter_list ',' parameter_declaration
	;

parameter_declaration
	: declaration_specifiers declarator
	| declaration_specifiers abstract_declarator
	| declaration_specifiers
	;

identifier_list
	: IDENTIFIER
	| identifier_list ',' IDENTIFIER
	;

type_name
	: specifier_qualifier_list
	| specifier_qualifier_list abstract_declarator
	;

abstract_declarator
	: pointer
	| direct_abstract_declarator
	| pointer direct_abstract_declarator
	;

direct_abstract_declarator
	: '(' abstract_declarator ')'
	| '[' ']'
	| '[' constant_expression ']'
	| direct_abstract_declarator '[' ']'
	| direct_abstract_declarator '[' constant_expression ']'
	| '(' ')'
	| '(' parameter_list ')'
	| direct_abstract_declarator '(' ')'
	| direct_abstract_declarator '(' parameter_list ')'
	;

initializer
	: assignment_expression
	| '{' initializer_list '}'
	| '{' initializer_list ',' '}'
	;

initializer_list
	: initializer
	| initializer_list ',' initializer
	;

statement
	: labeled_statement
	| compound_statement
	| expression_statement
	| selection_statement
	| iteration_statement
	| jump_statement { $$ = $1; }
	;

labeled_statement
	: IDENTIFIER ':' statement
	| CASE constant_expression ':' statement
	| DEFAULT ':' statement
	;

compound_statement
    : '{' '}' {
        $$ = new NodeList(nullptr);
    }
    | '{' statement_list '}' {
        $$ = $2;
    }
    | '{' declaration_list '}' {
        $$ = $2; // 使用declaration_list而非nullptr
    }
    | '{' declaration_list statement_list '}'  {
        // 创建一个新的NodeList，将declaration_list作为第一个节点
        NodeList* result = new NodeList(NodePtr($2));

        // 然后将statement_list作为第二个节点添加
        result->PushBack(NodePtr($3));

        $$ = result;
    }
    ;

declaration_list
	: declaration
	| declaration_list declaration
	;

statement_list
	: statement { $$ = new NodeList(NodePtr($1)); }
	| statement_list statement { $1->PushBack(NodePtr($2)); $$=$1; }
	;

expression_statement
	: ';' { $$ = nullptr; }
	| expression ';' { $$ = $1; }
	;

selection_statement
	: IF '(' expression ')' statement {
		$$ = new IfStatement(NodePtr($3), NodePtr($5), nullptr);
	}
	| IF '(' expression ')' statement ELSE statement {
		$$ = new IfStatement(NodePtr($3), NodePtr($5), NodePtr($7));
	}
	| SWITCH '(' expression ')' statement
	;

iteration_statement
	: WHILE '(' expression ')' statement {
		$$ = new WhileStatement(NodePtr($3), NodePtr($5));
	}
	| DO statement WHILE '(' expression ')' ';' //{
	//	$$ = new DoWhileStatement(NodePtr($2), NodePtr($5));
	//}
	| FOR '(' expression_statement expression_statement ')' statement {
		$$ = new ForStatement(NodePtr($3), NodePtr($4), nullptr, NodePtr($6));
	}
	| FOR '(' expression_statement expression_statement expression ')' statement {
		$$ = new ForStatement(NodePtr($3), NodePtr($4), NodePtr($5), NodePtr($7));
	}
	;

jump_statement
	: GOTO IDENTIFIER ';'
	| CONTINUE ';' {
		$$ = new ContinueStatement();
	}
	| BREAK ';' {
		$$ = new BreakStatement();
	}
	| RETURN ';' {
		$$ = new ReturnStatement(nullptr);
	}
	| RETURN expression ';' {
		$$ = new ReturnStatement(NodePtr($2));
	}
	;

%%

Node* g_root;

NodePtr ParseAST(std::string file_name)
{
  yyin = fopen(file_name.c_str(), "r");
  if(yyin == NULL){
    std::cerr << "Couldn't open input file: " << file_name << std::endl;
    exit(1);
  }
  g_root = nullptr;
  yyparse();
  fclose(yyin);
  yylex_destroy();
  return NodePtr(g_root);
}
