%{
#include <stdio.h>
#include <stdlib.h>
#include "../oreProject/proc.h"
#define YYDEBUG 1
extern int yylex(void);
int yyerror(char const *str) {
    extern char *yytext;
    extern int gLine;
    ore::Interpreter::getInp()->syntaxExit(1003, gLine, yytext);
    return 0;
}
%}
%code requires {
#include "../oreProject/proc.h"
}
%union {
    const char* fixedString;
    ore::Expression* expression;
    ore::Statement* statement;
    ore::StatementList* statementList;
    ore::Root* root;
}
%token <fixedString> IDENTIFIER
%token <expression> TRUE_T FALSE_T INT_LITERAL DOUBLE_LITERAL STR_LITERAL
%token LP RP LC RC EQ NE LE GE LT GT SEMICOLON ADD SUB MUL DIV MOD CR 
%token MULASS DIVASS MODASS ADDASS SUBASS ASS PRINTN PRINT EXPO
%token IF
%right ASS
%right ADDASS SUBASS AMPASS
%right MULASS DIVASS MODASS
%left ADD SUB
%left MUL DIV MOD
%type <expression> constart_expression identifier_expression
%type <expression> primary_expression mul_expression add_expression
%type <expression> relational_expression equality_expression
%type <expression> assign_expression expression
%type <statement> expression_statement compound_statement internal_statement statement
%type <statement> selection_statement
%type <statementList> statement_list
%type <root> root
%%
root
    : statement_list
    {
        $$ = ore::Interpreter::getInp()->createRoot($1);
    }
    ;
statement_list
    : statement
    {
        $$ = ore::Interpreter::getInp()->createStatementList($1);
    }
    | statement_list statement
    {
        $$ = ore::Interpreter::getInp()->createStatementList($1, $2);
    }
    ;
statement
    : expression_statement
    | compound_statement
    | selection_statement
    | internal_statement
    ;
expression_statement
    : SEMICOLON
    {
        $$ = ore::Interpreter::getInp()->createStatement<ore::ExpressionStm>();
    }
    | expression SEMICOLON
    {
        $$ = ore::Interpreter::getInp()->createStatement<ore::ExpressionStm>($1);
    }
    ;
compound_statement
    : LC RC
    {
        $$ = ore::Interpreter::getInp()->createStatement<ore::BlockStm>();
    }
    | LC statement_list RC
    {
        $$ = ore::Interpreter::getInp()->createStatement<ore::BlockStm>($2);
    }
    ;
selection_statement
    : IF LP expression RP statement
    {
        $$ = ore::Interpreter::getInp()->createStatement<ore::IfStm>($3, $5);
    }
    ;
internal_statement
    : PRINTN expression SEMICOLON
    {
        $$ = ore::Interpreter::getInp()->createStatement<ore::PrintStm>($2, true);
    }
    | PRINT expression SEMICOLON
    {
        $$ = ore::Interpreter::getInp()->createStatement<ore::PrintStm>($2, false);
    }
    ;
expression
    : assign_expression
    ;
assign_expression
    : equality_expression
    | identifier_expression MULASS assign_expression
    {
        $$ = ore::Interpreter::getInp()->createToAssExp($1, $3, ore::ExpressionType::mulAssExp);
    }
    | identifier_expression DIVASS assign_expression
    {
        $$ = ore::Interpreter::getInp()->createToAssExp($1, $3, ore::ExpressionType::divAssExp);
    }
    | identifier_expression MODASS assign_expression
    {
        $$ = ore::Interpreter::getInp()->createToAssExp($1, $3, ore::ExpressionType::modAssExp);
    }
    | identifier_expression ADDASS assign_expression
    {
        $$ = ore::Interpreter::getInp()->createToAssExp($1, $3, ore::ExpressionType::addAssExp);
    }
    | identifier_expression SUBASS assign_expression
    {
        $$ = ore::Interpreter::getInp()->createToAssExp($1, $3, ore::ExpressionType::subAssExp);
    }
    | identifier_expression AMPASS assign_expression
    {
        $$ = ore::Interpreter::getInp()->createToAssExp($1, $3, ore::ExpressionType::ampAssExp);
    }
    | identifier_expression ASS assign_expression
    {
        $$ = ore::Interpreter::getInp()->createAssExp($1, $3);
    }
    ;
equality_expression
    : relational_expression
    | equality_expression EQ relational_expression
    {
        $$ = ore::Interpreter::getInp()->createRelationalExp($1, $3, ore::ExpressionType::eqExp);
    }
    | equality_expression NE relational_expression
    {
        $$ = ore::Interpreter::getInp()->createRelationalExp($1, $3, ore::ExpressionType::neExp);
    }
    ;
relational_expression
    : add_expression
    | relational_expression LT add_expression
    {
        $$ = ore::Interpreter::getInp()->createRelationalExp($1, $3, ore::ExpressionType::ltExp);
    }
    | relational_expression GT add_expression
    {
        $$ = ore::Interpreter::getInp()->createRelationalExp($1, $3, ore::ExpressionType::gtExp);
    }
    | relational_expression LE add_expression
    {
        $$ = ore::Interpreter::getInp()->createRelationalExp($1, $3, ore::ExpressionType::leExp);
    }
    | relational_expression GE add_expression
    {
        $$ = ore::Interpreter::getInp()->createRelationalExp($1, $3, ore::ExpressionType::geExp);
    }
    ;
add_expression
    : mul_expression
    | add_expression ADD mul_expression
    {
        $$ = ore::Interpreter::getInp()->createBinaryExp($1, $3, ore::ExpressionType::addExp);
    }
    | add_expression SUB mul_expression
    {
        $$ = ore::Interpreter::getInp()->createBinaryExp($1, $3, ore::ExpressionType::subExp);
    }
    | add_expression AMP mul_expression
    {
        $$ = ore::Interpreter::getInp()->createBinaryExp($1, $3, ore::ExpressionType::ampExp);
    }
    ;
mul_expression
    : primary_expression
    | mul_expression MUL primary_expression 
    {
        $$ = ore::Interpreter::getInp()->createBinaryExp($1, $3, ore::ExpressionType::mulExp);
    }
    | mul_expression DIV primary_expression
    {
        $$ = ore::Interpreter::getInp()->createBinaryExp($1, $3, ore::ExpressionType::divExp);
    }
    | mul_expression MOD primary_expression
    {
        $$ = ore::Interpreter::getInp()->createBinaryExp($1, $3, ore::ExpressionType::modExp);
    }
    | mul_expression EXPO primary_expression
    {
        $$ = ore::Interpreter::getInp()->createBinaryExp($1, $3, ore::ExpressionType::expoExp);
    }

    ;
primary_expression
    : identifier_expression
    | constart_expression
    | STR_LITERAL
    {
        $$ = ore::Interpreter::getInp()->createStringLiteralExp();
    }
    ;
identifier_expression
    : IDENTIFIER
    {
        $$ = ore::Interpreter::getInp()->createVariableExp($1);
    }
    ;
constart_expression
    : DOUBLE_LITERAL
    {
        $$ = $1;
    }
    | INT_LITERAL
    {
        $$ = $1;
    }
    | TRUE_T
    {
        $$ = ore::Interpreter::getInp()->createBoolLiteralExp(true);
    }
    | FALSE_T
    {
        $$ = ore::Interpreter::getInp()->createBoolLiteralExp(false);
    }
    ;
%%