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
%token <expression> INT_LITERAL DOUBLE_LITERAL STR_LITERAL
%token SEMICOLON ADD SUB MUL DIV CR 
%token MULASS DIVASS ADDASS SUBASS ASS PRINTN PRINT EXPO
%right ASS
%right ADDASS SUBASS
%right MULASS DIVASS
%left ADD SUB
%left MUL DIV
%type <expression> constart_expression identifier_expression
%type <expression> primary_expression mul_expression add_expression
%type <expression> assign_expression expression
%type <statement> expression_statement internal_statement statement
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
    : add_expression
    | identifier_expression MULASS assign_expression
    {
        $$ = ore::Interpreter::getInp()->createToAssExp($1, $3, ore::ExpressionType::mulAssExp);
    }
    | identifier_expression DIVASS assign_expression
    {
        $$ = ore::Interpreter::getInp()->createToAssExp($1, $3, ore::ExpressionType::divAssExp);
    }
    | identifier_expression ADDASS assign_expression
    {
        $$ = ore::Interpreter::getInp()->createToAssExp($1, $3, ore::ExpressionType::addAssExp);
    }
    | identifier_expression SUBASS assign_expression
    {
        $$ = ore::Interpreter::getInp()->createToAssExp($1, $3, ore::ExpressionType::subAssExp);
    }
    | identifier_expression ASS assign_expression
    {
        $$ = ore::Interpreter::getInp()->createAssExp($1, $3);
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
    ;
%%