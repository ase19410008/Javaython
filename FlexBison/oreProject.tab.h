/* A Bison parser, made by GNU Bison 3.7.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_OREPROJECT_TAB_H_INCLUDED
# define YY_YY_OREPROJECT_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 14 "oreProject.y"

#include "../oreProject/proc.h"

#line 53 "oreProject.tab.h"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    IDENTIFIER = 258,              /* IDENTIFIER  */
    TRUE_T = 259,                  /* TRUE_T  */
    INT_LITERAL = 260,             /* INT_LITERAL  */
    DOUBLE_LITERAL = 261,          /* DOUBLE_LITERAL  */
    STR_LITERAL = 262,             /* STR_LITERAL  */
    LP = 263,                      /* LP  */
    RP = 264,                      /* RP  */
    LC = 265,                      /* LC  */
    RC = 266,                      /* RC  */
    SEMICOLON = 267,               /* SEMICOLON  */
    ADD = 268,                     /* ADD  */
    SUB = 269,                     /* SUB  */
    MUL = 270,                     /* MUL  */
    DIV = 271,                     /* DIV  */
    MOD = 272,                     /* MOD  */
    CR = 273,                      /* CR  */
    MULASS = 274,                  /* MULASS  */
    DIVASS = 275,                  /* DIVASS  */
    MODASS = 276,                  /* MODASS  */
    ADDASS = 277,                  /* ADDASS  */
    SUBASS = 278,                  /* SUBASS  */
    ASS = 279,                     /* ASS  */
    PRINTN = 280,                  /* PRINTN  */
    PRINT = 281,                   /* PRINT  */
    EXPO = 282,                    /* EXPO  */
    IF = 283                       /* IF  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 17 "oreProject.y"

    const char* fixedString;
    ore::Expression* expression;
    ore::Statement* statement;
    ore::StatementList* statementList;
    ore::Root* root;

#line 106 "oreProject.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_OREPROJECT_TAB_H_INCLUDED  */
