// Adapted from: https://www.lysator.liu.se/c/ANSI-C-grammar-y.html

%code requires{
    #include "ast.hpp"

    using namespace ast;

    extern Node* g_root;
    extern FILE* yyin;
    int yylex(void);
    void yyerror(const char*);
    int yylex_destroy(void);
}

// Represents the value associated with any kind of AST node.
%union{
  Node*         node;
  NodeList*     node_list;
  int           number_int;
  double        number_float;
  std::string*  string;
  TypeSpecifier type_specifier;
  yytokentype   token;
}

%token IDENTIFIER INT_CONSTANT FLOAT_CONSTANT STRING_LITERAL
%token PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP AND_OP OR_OP
%token MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN XOR_ASSIGN OR_ASSIGN
%token TYPE_NAME TYPEDEF EXTERN STATIC AUTO REGISTER SIZEOF
%token CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE CONST VOLATILE VOID
%token STRUCT UNION ENUM ELLIPSIS
%token CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN

%type <node> translation_unit external_declaration function_definition
%type <node> primary_expression postfix_expression argument_expression_list
%type <node> unary_expression unary_operator cast_expression
%type <node> multiplicative_expression additive_expression shift_expression
%type <node> relational_expression equality_expression and_expression
%type <node> exclusive_or_expression inclusive_or_expression logical_and_expression
%type <node> logical_or_expression conditional_expression assignment_expression
%type <node> assignment_operator expression constant_expression
%type <node> declaration declaration_specifiers init_declarator_list init_declarator
%type <node> storage_class_specifier struct_or_union_specifier struct_or_union
%type <node> struct_declaration_list struct_declaration specifier_qualifier_list
%type <node> struct_declarator_list struct_declarator enum_specifier enumerator_list
%type <node> enumerator declarator direct_declarator pointer type_qualifier_list
%type <node> type_qualifier parameter_type_list parameter_list parameter_declaration
%type <node> identifier_list type_name abstract_declarator direct_abstract_declarator
%type <node> initializer initializer_list statement labeled_statement
%type <node> compound_statement expression_statement selection_statement
%type <node> iteration_statement jump_statement

%type <node_list> statement_list declaration_list

%type <number_int> INT_CONSTANT STRING_LITERAL
%type <number_float> FLOAT_CONSTANT
%type <string> IDENTIFIER
%type <type_specifier> type_specifier
%type <type_specifier> declaration_specifiers

%start ROOT
%%

ROOT
    : translation_unit { g_root = $1; }

translation_unit
    : external_declaration { $$ = $1; }
    | translation_unit external_declaration { $$ = new TranslationUnit(NodePtr($1), NodePtr($2)); }
    ;

external_declaration
    : function_definition { $$ = $1; }
    | declaration { $$ = $1; }
    ;

function_definition
    : declaration_specifiers declarator declaration_list compound_statement {
        $$ = new FunctionDefinition($1, NodePtr($2), NodePtr($3), NodePtr($4));
    }
    | declaration_specifiers declarator compound_statement {
        $$ = new FunctionDefinition($1, NodePtr($2), NodePtr($3));
    }
    | declarator declaration_list compound_statement {
        $$ = new FunctionDefinition(TypeSpecifier::INT, NodePtr($1), NodePtr($2), NodePtr($3));
    }
    | declarator compound_statement {
        $$ = new FunctionDefinition(TypeSpecifier::INT, NodePtr($1), NodePtr($2));
    }
    ;

declaration
    : declaration_specifiers ';' {
        $$ = new Declaration($1, nullptr);
    }
    | declaration_specifiers init_declarator_list ';' {
        $$ = new Declaration($1, NodePtr($2));
    }
    ;

declaration_specifiers
    : storage_class_specifier {
        $$ = TypeSpecifier::INT; // Default to INT for now
    }
    | storage_class_specifier declaration_specifiers {
        $$ = $2; // Use the type from the rest of the declaration
    }
    | type_specifier {
        $$ = $1;
    }
    | type_specifier declaration_specifiers {
        $$ = $1; // Use the explicit type specifier
    }
    | type_qualifier {
        $$ = TypeSpecifier::INT; // Default to INT for now
    }
    | type_qualifier declaration_specifiers {
        $$ = $2; // Use the type from the rest of the declaration
    }
    ;

init_declarator_list
    : init_declarator {
        $$ = new InitDeclaratorList(NodePtr($1));
    }
    | init_declarator_list ',' init_declarator {
        $$ = new InitDeclaratorListExtension(NodePtr($1), NodePtr($3));
    }
    ;

init_declarator
    : declarator {
        $$ = new InitDeclarator(NodePtr($1), nullptr);
    }
    | declarator '=' initializer {
        $$ = new InitDeclarator(NodePtr($1), NodePtr($3));
    }
    ;

storage_class_specifier
    : TYPEDEF { $$ = new StorageClassSpecifier(StorageClassType::TYPEDEF); }
    | EXTERN { $$ = new StorageClassSpecifier(StorageClassType::EXTERN); }
    | STATIC { $$ = new StorageClassSpecifier(StorageClassType::STATIC); }
    | AUTO { $$ = new StorageClassSpecifier(StorageClassType::AUTO); }
    | REGISTER { $$ = new StorageClassSpecifier(StorageClassType::REGISTER); }
    ;

type_specifier
    : VOID { $$ = TypeSpecifier::VOID; }
    | CHAR { $$ = TypeSpecifier::CHAR; }
    | SHORT { $$ = TypeSpecifier::SHORT; }
    | INT { $$ = TypeSpecifier::INT; }
    | LONG { $$ = TypeSpecifier::LONG; }
    | FLOAT { $$ = TypeSpecifier::FLOAT; }
    | DOUBLE { $$ = TypeSpecifier::DOUBLE; }
    | SIGNED { $$ = TypeSpecifier::SIGNED; }
    | UNSIGNED { $$ = TypeSpecifier::UNSIGNED; }
    | struct_or_union_specifier { $$ = TypeSpecifier::STRUCT; } // For now, treat all structs as one type
    | enum_specifier { $$ = TypeSpecifier::INT; } // Enums are essentially ints
    | TYPE_NAME { $$ = TypeSpecifier::INT; } // Default to INT for typedef names for now
    ;

struct_or_union_specifier
    : struct_or_union IDENTIFIER '{' struct_declaration_list '}' {
        $$ = new StructOrUnionSpecifier(NodePtr($1), NodePtr(new Identifier(*$2)), NodePtr($4));
        delete $2;
    }
    | struct_or_union '{' struct_declaration_list '}' {
        $$ = new StructOrUnionSpecifier(NodePtr($1), nullptr, NodePtr($3));
    }
    | struct_or_union IDENTIFIER {
        $$ = new StructOrUnionSpecifier(NodePtr($1), NodePtr(new Identifier(*$2)), nullptr);
        delete $2;
    }
    ;

struct_or_union
    : STRUCT { $$ = new StructOrUnion(true); }
    | UNION { $$ = new StructOrUnion(false); }
    ;

struct_declaration_list
    : struct_declaration {
        $$ = new StructDeclarationList(NodePtr($1));
    }
    | struct_declaration_list struct_declaration {
        $$ = new StructDeclarationListExtension(NodePtr($1), NodePtr($2));
    }
    ;

struct_declaration
    : specifier_qualifier_list struct_declarator_list ';' {
        $$ = new StructDeclaration(NodePtr($1), NodePtr($2));
    }
    ;

specifier_qualifier_list
    : type_specifier specifier_qualifier_list {
        $$ = new SpecifierQualifierList($1, NodePtr($2));
    }
    | type_specifier {
        $$ = new SpecifierQualifierList($1, nullptr);
    }
    | type_qualifier specifier_qualifier_list {
        $$ = new SpecifierQualifierList(TypeSpecifier::INT, NodePtr($2)); // Default to INT
    }
    | type_qualifier {
        $$ = new SpecifierQualifierList(TypeSpecifier::INT, nullptr); // Default to INT
    }
    ;

struct_declarator_list
    : struct_declarator {
        $$ = new StructDeclaratorList(NodePtr($1));
    }
    | struct_declarator_list ',' struct_declarator {
        $$ = new StructDeclaratorListExtension(NodePtr($1), NodePtr($3));
    }
    ;

struct_declarator
    : declarator {
        $$ = new StructDeclarator(NodePtr($1), nullptr);
    }
    | ':' constant_expression {
        $$ = new StructDeclarator(nullptr, NodePtr($2));
    }
    | declarator ':' constant_expression {
        $$ = new StructDeclarator(NodePtr($1), NodePtr($3));
    }
    ;

enum_specifier
    : ENUM '{' enumerator_list '}' {
        $$ = new EnumSpecifier(nullptr, NodePtr($3));
    }
    | ENUM IDENTIFIER '{' enumerator_list '}' {
        $$ = new EnumSpecifier(NodePtr(new Identifier(*$2)), NodePtr($4));
        delete $2;
    }
    | ENUM IDENTIFIER {
        $$ = new EnumSpecifier(NodePtr(new Identifier(*$2)), nullptr);
        delete $2;
    }
    ;

enumerator_list
    : enumerator {
        $$ = new EnumeratorList(NodePtr($1));
    }
    | enumerator_list ',' enumerator {
        $$ = new EnumeratorListExtension(NodePtr($1), NodePtr($3));
    }
    ;

enumerator
    : IDENTIFIER {
        $$ = new Enumerator(NodePtr(new Identifier(*$1)), nullptr);
        delete $1;
    }
    | IDENTIFIER '=' constant_expression {
        $$ = new Enumerator(NodePtr(new Identifier(*$1)), NodePtr($3));
        delete $1;
    }
    ;

type_qualifier
    : CONST { $$ = new TypeQualifier(true); }
    | VOLATILE { $$ = new TypeQualifier(false); }
    ;

declarator
    : pointer direct_declarator {
        $$ = new Declarator(NodePtr($1), NodePtr($2));
    }
    | direct_declarator {
        $$ = $1;
    }
    ;

direct_declarator
    : IDENTIFIER {
        $$ = new Identifier(std::move(*$1));
        delete $1;
    }
    | '(' declarator ')' {
        $$ = $2;
    }
    | direct_declarator '[' constant_expression ']' {
        $$ = new ArrayDeclarator(NodePtr($1), NodePtr($3));
    }
    | direct_declarator '[' ']' {
        $$ = new ArrayDeclarator(NodePtr($1), nullptr);
    }
    | direct_declarator '(' parameter_type_list ')' {
        $$ = new FunctionDeclarator(NodePtr($1), NodePtr($3));
    }
    | direct_declarator '(' identifier_list ')' {
        $$ = new FunctionDeclarator(NodePtr($1), NodePtr($3));
    }
    | direct_declarator '(' ')' {
        $$ = new DirectDeclarator(NodePtr($1));
    }
    ;

pointer
    : '*' {
        $$ = new Pointer(nullptr, nullptr);
    }
    | '*' type_qualifier_list {
        $$ = new Pointer(NodePtr($2), nullptr);
    }
    | '*' pointer {
        $$ = new Pointer(nullptr, NodePtr($2));
    }
    | '*' type_qualifier_list pointer {
        $$ = new Pointer(NodePtr($2), NodePtr($3));
    }
    ;

type_qualifier_list
    : type_qualifier {
        $$ = new TypeQualifierList(NodePtr($1));
    }
    | type_qualifier_list type_qualifier {
        $$ = new TypeQualifierListExtension(NodePtr($1), NodePtr($2));
    }
    ;

parameter_type_list
    : parameter_list {
        $$ = $1;
    }
    | parameter_list ',' ELLIPSIS {
        $$ = new ParameterTypeList(NodePtr($1), true);
    }
    ;

parameter_list
    : parameter_declaration {
        $$ = new ParameterList(NodePtr($1));
    }
    | parameter_list ',' parameter_declaration {
        $$ = new ParameterListExtension(NodePtr($1), NodePtr($3));
    }
    ;

parameter_declaration
    : declaration_specifiers declarator {
        $$ = new ParameterDeclaration($1, NodePtr($2), false);
    }
    | declaration_specifiers abstract_declarator {
        $$ = new ParameterDeclaration($1, NodePtr($2), true);
    }
    | declaration_specifiers {
        $$ = new ParameterDeclaration($1, nullptr, false);
    }
    ;

identifier_list
    : IDENTIFIER {
        $$ = new IdentifierList(NodePtr(new Identifier(*$1)));
        delete $1;
    }
    | identifier_list ',' IDENTIFIER {
        $$ = new IdentifierListExtension(NodePtr($1), NodePtr(new Identifier(*$3)));
        delete $3;
    }
    ;

type_name
    : specifier_qualifier_list {
        $$ = new TypeName(NodePtr($1), nullptr);
    }
    | specifier_qualifier_list abstract_declarator {
        $$ = new TypeName(NodePtr($1), NodePtr($2));
    }
    ;

abstract_declarator
    : pointer {
        $$ = new AbstractDeclarator(NodePtr($1), nullptr);
    }
    | direct_abstract_declarator {
        $$ = new AbstractDeclarator(nullptr, NodePtr($1));
    }
    | pointer direct_abstract_declarator {
        $$ = new AbstractDeclarator(NodePtr($1), NodePtr($2));
    }
    ;

direct_abstract_declarator
    : '(' abstract_declarator ')' {
        $$ = $2;
    }
    | '[' ']' {
        $$ = new DirectAbstractDeclarator(nullptr, nullptr, true, false);
    }
    | '[' constant_expression ']' {
        $$ = new DirectAbstractDeclarator(nullptr, NodePtr($2), true, false);
    }
    | direct_abstract_declarator '[' ']' {
        $$ = new DirectAbstractDeclarator(NodePtr($1), nullptr, true, false);
    }
    | direct_abstract_declarator '[' constant_expression ']' {
        $$ = new DirectAbstractDeclarator(NodePtr($1), NodePtr($3), true, false);
    }
    | '(' ')' {
        $$ = new DirectAbstractDeclarator(nullptr, nullptr, false, true);
    }
    | '(' parameter_type_list ')' {
        $$ = new DirectAbstractDeclarator(nullptr, NodePtr($2), false, true);
    }
    | direct_abstract_declarator '(' ')' {
        $$ = new DirectAbstractDeclarator(NodePtr($1), nullptr, false, true);
    }
    | direct_abstract_declarator '(' parameter_type_list ')' {
        $$ = new DirectAbstractDeclarator(NodePtr($1), NodePtr($3), false, true);
    }
    ;

initializer
    : assignment_expression {
        $$ = new Initializer(NodePtr($1), false);
    }
    | '{' initializer_list '}' {
        $$ = new Initializer(NodePtr($2), true);
    }
    | '{' initializer_list ',' '}' {
        $$ = new Initializer(NodePtr($2), true);
    }
    ;

initializer_list
    : initializer {
        $$ = new InitializerList(NodePtr($1));
    }
    | initializer_list ',' initializer {
        $$ = new InitializerListExtension(NodePtr($1), NodePtr($3));
    }
    ;

statement
    : labeled_statement { $$ = $1; }
    | compound_statement { $$ = $1; }
    | expression_statement { $$ = $1; }
    | selection_statement { $$ = $1; }
    | iteration_statement { $$ = $1; }
    | jump_statement { $$ = $1; }
    ;

labeled_statement
    : IDENTIFIER ':' statement {
        $$ = new LabeledStatement(NodePtr(new Identifier(*$1)), NodePtr($3));
        delete $1;
    }
    | CASE constant_expression ':' statement {
        $$ = new CaseStatement(NodePtr($2), NodePtr($4));
    }
    | DEFAULT ':' statement {
        $$ = new DefaultStatement(NodePtr($3));
    }
    ;

compound_statement
    : '{' '}' {
        $$ = new CompoundStatement(nullptr, nullptr);
    }
    | '{' statement_list '}' {
        $$ = $2;
    }
    | '{' declaration_list '}' {
        $$ = new CompoundStatement(NodePtr($2), nullptr);
    }
    | '{' declaration_list statement_list '}' {
        $$ = new CompoundStatement(NodePtr($2), NodePtr($3));
    }
    ;

declaration_list
    : declaration {
        $$ = new DeclarationList(NodePtr($1));
    }
    | declaration_list declaration {
        $1->PushBack(NodePtr($2));
        $$ = $1;
    }
    ;

statement_list
    : statement {
        $$ = new NodeList(NodePtr($1));
    }
    | statement_list statement {
        $1->PushBack(NodePtr($2));
        $$ = $1;
    }
    ;

expression_statement
    : ';' {
        $$ = new ExpressionStatement(nullptr);
    }
    | expression ';' {
        $$ = new ExpressionStatement(NodePtr($1));
    }
    ;

selection_statement
    : IF '(' expression ')' statement {
        $$ = new IfStatement(NodePtr($3), NodePtr($5), nullptr);
    }
    | IF '(' expression ')' statement ELSE statement {
        $$ = new IfStatement(NodePtr($3), NodePtr($5), NodePtr($7));
    }
    | SWITCH '(' expression ')' statement {
        $$ = new SwitchStatement(NodePtr($3), NodePtr($5));
    }
    ;

iteration_statement
    : WHILE '(' expression ')' statement {
        $$ = new WhileStatement(NodePtr($3), NodePtr($5));
    }
    | DO statement WHILE '(' expression ')' ';' {
        $$ = new DoWhileStatement(NodePtr($2), NodePtr($5));
    }
    | FOR '(' expression_statement expression_statement ')' statement {
        $$ = new ForStatement(NodePtr($3), NodePtr($4), nullptr, NodePtr($6));
    }
    | FOR '(' expression_statement expression_statement expression ')' statement {
        $$ = new ForStatement(NodePtr($3), NodePtr($4), NodePtr($5), NodePtr($7));
    }
    ;

jump_statement
    : GOTO IDENTIFIER ';' {
        $$ = new GotoStatement(NodePtr(new Identifier(*$2)));
        delete $2;
    }
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

expression
    : assignment_expression {
        $$ = $1;
    }
    | expression ',' assignment_expression {
        $$ = new CommaExpression(NodePtr($1), NodePtr($3));
    }
    ;

assignment_expression
    : conditional_expression {
        $$ = $1;
    }
    | unary_expression assignment_operator assignment_expression {
        $$ = new AssignmentExpression(NodePtr($1), NodePtr($2), NodePtr($3));
    }
    ;

assignment_operator
    : '=' { $$ = new AssignmentOperator(AssignmentType::ASSIGN); }
    | MUL_ASSIGN { $$ = new AssignmentOperator(AssignmentType::MUL_ASSIGN); }
    | DIV_ASSIGN { $$ = new AssignmentOperator(AssignmentType::DIV_ASSIGN); }
    | MOD_ASSIGN { $$ = new AssignmentOperator(AssignmentType::MOD_ASSIGN); }
    | ADD_ASSIGN { $$ = new AssignmentOperator(AssignmentType::ADD_ASSIGN); }
    | SUB_ASSIGN { $$ = new AssignmentOperator(AssignmentType::SUB_ASSIGN); }
    | LEFT_ASSIGN { $$ = new AssignmentOperator(AssignmentType::LEFT_ASSIGN); }
    | RIGHT_ASSIGN { $$ = new AssignmentOperator(AssignmentType::RIGHT_ASSIGN); }
    | AND_ASSIGN { $$ = new AssignmentOperator(AssignmentType::AND_ASSIGN); }
    | XOR_ASSIGN { $$ = new AssignmentOperator(AssignmentType::XOR_ASSIGN); }
    | OR_ASSIGN { $$ = new AssignmentOperator(AssignmentType::OR_ASSIGN); }
    ;

conditional_expression
    : logical_or_expression {
        $$ = $1;
    }
    | logical_or_expression '?' expression ':' conditional_expression {
        $$ = new ConditionalExpression(NodePtr($1), NodePtr($3), NodePtr($5));
    }
    ;

constant_expression
    : conditional_expression {
        $$ = $1;
    }
    ;

logical_or_expression
    : logical_and_expression {
        $$ = $1;
    }
    | logical_or_expression OR_OP logical_and_expression {
        $$ = new LogicalOrExpression(NodePtr($1), NodePtr($3));
    }
    ;

logical_and_expression
    : inclusive_or_expression {
        $$ = $1;
    }
    | logical_and_expression AND_OP inclusive_or_expression {
        $$ = new LogicalAndExpression(NodePtr($1), NodePtr($3));
    }
    ;

inclusive_or_expression
    : exclusive_or_expression {
        $$ = $1;
    }
    | inclusive_or_expression '|' exclusive_or_expression {
        $$ = new BitwiseOrExpression(NodePtr($1), NodePtr($3));
    }
    ;

exclusive_or_expression
    : and_expression {
        $$ = $1;
    }
    | exclusive_or_expression '^' and_expression {
        $$ = new BitwiseXorExpression(NodePtr($1), NodePtr($3));
    }
    ;

and_expression
    : equality_expression {
        $$ = $1;
    }
    | and_expression '&' equality_expression {
        $$ = new BitwiseAndExpression(NodePtr($1), NodePtr($3));
    }
    ;

equality_expression
    : relational_expression {
        $$ = $1;
    }
    | equality_expression EQ_OP relational_expression {
        $$ = new EqualityExpression(NodePtr($1), NodePtr($3), true);
    }
    | equality_expression NE_OP relational_expression {
        $$ = new EqualityExpression(NodePtr($1), NodePtr($3), false);
    }
    ;

relational_expression
    : shift_expression {
        $$ = $1;
    }
    | relational_expression '<' shift_expression {
        $$ = new RelationalExpression(NodePtr($1), NodePtr($3), RelationalOperator::LT);
    }
    | relational_expression '>' shift_expression {
        $$ = new RelationalExpression(NodePtr($1), NodePtr($3), RelationalOperator::GT);
    }
    | relational_expression LE_OP shift_expression {
        $$ = new RelationalExpression(NodePtr($1), NodePtr($3), RelationalOperator::LE);
    }
    | relational_expression GE_OP shift_expression {
        $$ = new RelationalExpression(NodePtr($1), NodePtr($3), RelationalOperator::GE);
    }
    ;

shift_expression
    : additive_expression {
        $$ = $1;
    }
    | shift_expression LEFT_OP additive_expression {
        $$ = new ShiftExpression(NodePtr($1), NodePtr($3), true);
    }
    | shift_expression RIGHT_OP additive_expression {
        $$ = new ShiftExpression(NodePtr($1), NodePtr($3), false);
    }
    ;

additive_expression
    : multiplicative_expression {
        $$ = $1;
    }
    | additive_expression '+' multiplicative_expression {
        $$ = new AdditiveExpression(NodePtr($1), NodePtr($3), true);
    }
    | additive_expression '-' multiplicative_expression {
        $$ = new AdditiveExpression(NodePtr($1), NodePtr($3), false);
    }
    ;

multiplicative_expression
    : cast_expression {
        $$ = $1;
    }
    | multiplicative_expression '*' cast_expression {
        $$ = new MultiplicativeExpression(NodePtr($1), NodePtr($3), MultiplicativeOperator::MUL);
    }
    | multiplicative_expression '/' cast_expression {
        $$ = new MultiplicativeExpression(NodePtr($1), NodePtr($3), MultiplicativeOperator::DIV);
    }
    | multiplicative_expression '%' cast_expression {
        $$ = new MultiplicativeExpression(NodePtr($1), NodePtr($3), MultiplicativeOperator::MOD);
    }
    ;

cast_expression
    : unary_expression {
        $$ = $1;
    }
    | '(' type_name ')' cast_expression {
        $$ = new CastExpression(NodePtr($2), NodePtr($4));
    }
    ;

unary_expression
    : postfix_expression {
        $$ = $1;
    }
    | INC_OP unary_expression {
        $$ = new UnaryExpression(NodePtr($2), UnaryOperator::INCREMENT, true);
    }
    | DEC_OP unary_expression {
        $$ = new UnaryExpression(NodePtr($2), UnaryOperator::DECREMENT, true);
    }
    | unary_operator cast_expression {
        $$ = new UnaryExpression(NodePtr($2), NodePtr($1));
    }
    | SIZEOF unary_expression {
        $$ = new SizeofExpression(NodePtr($2), nullptr);
    }
    | SIZEOF '(' type_name ')' {
        $$ = new SizeofExpression(nullptr, NodePtr($3));
    }
    ;

unary_operator
    : '&' { $$ = new UnaryOperatorNode(UnaryOperator::ADDRESS); }
    | '*' { $$ = new UnaryOperatorNode(UnaryOperator::DEREFERENCE); }
    | '+' { $$ = new UnaryOperatorNode(UnaryOperator::PLUS); }
    | '-' { $$ = new UnaryOperatorNode(UnaryOperator::MINUS); }
    | '~' { $$ = new UnaryOperatorNode(UnaryOperator::BITWISE_NOT); }
    | '!' { $$ = new UnaryOperatorNode(UnaryOperator::LOGICAL_NOT); }
    ;

postfix_expression
    : primary_expression {
        $$ = $1;
    }
    | postfix_expression '[' expression ']' {
        $$ = new ArraySubscriptExpression(NodePtr($1), NodePtr($3));
    }
    | postfix_expression '(' ')' {
        $$ = new FunctionCallExpression(NodePtr($1), nullptr);
    }
    | postfix_expression '(' argument_expression_list ')' {
        $$ = new FunctionCallExpression(NodePtr($1), NodePtr($3));
    }
    | postfix_expression '.' IDENTIFIER {
        $$ = new MemberAccessExpression(NodePtr($1), NodePtr(new Identifier(*$3)), false);
        delete $3;
    }
    | postfix_expression PTR_OP IDENTIFIER {
        $$ = new MemberAccessExpression(NodePtr($1), NodePtr(new Identifier(*$3)), true);
        delete $3;
    }
    | postfix_expression INC_OP {
        $$ = new UnaryExpression(NodePtr($1), UnaryOperator::INCREMENT, false);
    }
    | postfix_expression DEC_OP {
        $$ = new UnaryExpression(NodePtr($1), UnaryOperator::DECREMENT, false);
    }
    ;

primary_expression
    : IDENTIFIER {
        $$ = new Identifier(std::move(*$1));
        delete $1;
    }
    | INT_CONSTANT {
        $$ = new IntConstant($1);
    }
    | FLOAT_CONSTANT {
        $$ = new FloatConstant($1);
    }
    | STRING_LITERAL {
        $$ = new StringLiteral($1);
    }
    | '(' expression ')' {
        $$ = $2;
    }
    ;

argument_expression_list
    : assignment_expression {
        $$ = new ArgumentExpressionList(NodePtr($1));
    }
    | argument_expression_list ',' assignment_expression {
        $$ = new ArgumentExpressionListExtension(NodePtr($1), NodePtr($3));
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
