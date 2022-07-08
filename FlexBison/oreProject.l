%{
#include <stdio.h>
#include "oreProject.tab.h"
#include "../oreProject/proc.h"
#define YY_SKIP_YYWRAP 1
int gLine = 1;
int yywrap(void){ return 1; }

%}
%s COMMENT
%s LINE_COMMENT
%s STR_LITERAL_ST_D
%s STR_LITERAL_ST_S
%%
<INITIAL>{
    ";"     return SEMICOLON;
    "*=" return MULASS;
    "/=" return DIVASS;
    "+=" return ADDASS;
    "-=" return SUBASS;
    "+"     return ADD;
    "-"     return SUB;
    "*"     return MUL;
    "/"     return DIV;
    "**"    return EXPO;
    "=" return ASS;
    "output"     return PRINTN;
    "p"     return PRINT;
    "/@"    { BEGIN(COMMENT);}
    [@][@]+ { BEGIN(LINE_COMMENT);}
    "\""    {
                ore::Interpreter::getInp()->startStringLiteral();
                BEGIN(STR_LITERAL_ST_D);
            }
    "\'"    {
                ore::Interpreter::getInp()->startStringLiteral();
                BEGIN(STR_LITERAL_ST_S);
            }
    [0-9][0-9]* {
        yylval.expression = ore::Interpreter::getInp()->createIntLiteralExp(yytext);
        return INT_LITERAL;
    }
    [0-9]*\.[0-9]* {
        yylval.expression = ore::Interpreter::getInp()->createDoubleLiteralExp(yytext);
        return DOUBLE_LITERAL;
    }
    [A-Za-z_][A-Za-z_0-9]* {
        yylval.fixedString = ore::Interpreter::getInp()->createFixedString(yytext);
        return IDENTIFIER;
    }
    "\n"    {gLine++;}
    [ \t]   ;
    .   { return 0;}
}
<COMMENT>{
    "\n"    {gLine++;}
    "@/" {BEGIN(INITIAL);}
    .   ;
}
<LINE_COMMENT>{
    "\n" { gLine++; BEGIN(INITIAL);}
    .   ;
}
<STR_LITERAL_ST_D>{
    "\n"    { gLine++; ore::Interpreter::getInp()->addStringLiteral('\n'); }
    "\""    { BEGIN(INITIAL); return STR_LITERAL; }
    .       { ore::Interpreter::getInp()->addStringLiteral(yytext[0]); }
}
<STR_LITERAL_ST_S>{
    "\n"    { gLine++; ore::Interpreter::getInp()->addStringLiteral('\n'); }
    "\'"    { BEGIN(INITIAL); return STR_LITERAL; }
    .       { ore::Interpreter::getInp()->addStringLiteral(yytext[0]); }
}
%%