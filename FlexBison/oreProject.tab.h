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
    FALSE_T = 260,                 /* FALSE_T  */
    INT_LITERAL = 261,             /* INT_LITERAL  */
    DOUBLE_LITERAL = 262,          /* DOUBLE_LITERAL  */
    STR_LITERAL = 263,             /* STR_LITERAL  */
    LP = 264,                      /* LP  */
    RP = 265,                      /* RP  */
    LC = 266,                      /* LC  */
    RC = 267,                      /* RC  */
    EQ = 268,                      /* EQ  */
    NE = 269,                      /* NE  */
    LE = 270,                      /* LE  */
    GE = 271,                      /* GE  */
    LT = 272,                      /* LT  */
    GT = 273,                      /* GT  */
    SEMICOLON = 274,               /* SEMICOLON  */
    AMP = 275,                     /* AMP  */
    ADD = 276,                     /* ADD  */
    SUB = 277,                     /* SUB  */
    MUL = 278,                     /* MUL  */
    DIV = 279,                     /* DIV  */
    MOD = 280,                     /* MOD  */
    CR = 281,                      /* CR  */
    MULASS = 282,                  /* MULASS  */
    DIVASS = 283,                  /* DIVASS  */
    MODASS = 284,                  /* MODASS  */
    ADDASS = 285,                  /* ADDASS  */
    SUBASS = 286,                  /* SUBASS  */
    AMPASS = 287,                  /* AMPASS  */
    ASS = 288,                     /* ASS  */
    PRINTN = 289,                  /* PRINTN  */
    PRINT = 290,                   /* PRINT  */
    EXPO = 291,                    /* EXPO  */
    IF = 292                       /* IF  */
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
#line 115 "oreProject.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_OREPROJECT_TAB_H_INCLUDED  */
