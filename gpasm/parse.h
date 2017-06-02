
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
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


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     LABEL = 258,
     IDENTIFIER = 259,
     CBLOCK = 260,
     DEBUG_LINE = 261,
     ENDC = 262,
     ERRORLEVEL = 263,
     FILL = 264,
     LIST = 265,
     NUMBER = 266,
     PROCESSOR = 267,
     STRING = 268,
     DEFINE = 269,
     UPPER = 270,
     HIGH = 271,
     LOW = 272,
     LSH = 273,
     RSH = 274,
     GREATER_EQUAL = 275,
     LESS_EQUAL = 276,
     EQUAL = 277,
     NOT_EQUAL = 278,
     LOGICAL_AND = 279,
     LOGICAL_OR = 280,
     ASSIGN_PLUS = 281,
     ASSIGN_MINUS = 282,
     ASSIGN_MULTIPLY = 283,
     ASSIGN_DIVIDE = 284,
     ASSIGN_MODULUS = 285,
     ASSIGN_LSH = 286,
     ASSIGN_RSH = 287,
     ASSIGN_AND = 288,
     ASSIGN_OR = 289,
     ASSIGN_XOR = 290,
     INCREMENT = 291,
     DECREMENT = 292,
     POSTINCREMENT = 293,
     POSTDECREMENT = 294,
     INDFOFFSET = 295,
     TBL_NO_CHANGE = 296,
     TBL_POST_INC = 297,
     TBL_POST_DEC = 298,
     TBL_PRE_INC = 299,
     CONCAT = 300,
     VAR = 301,
     VARLAB_BEGIN = 302,
     VAR_BEGIN = 303,
     VAR_END = 304
   };
#endif
/* Tokens.  */
#define LABEL 258
#define IDENTIFIER 259
#define CBLOCK 260
#define DEBUG_LINE 261
#define ENDC 262
#define ERRORLEVEL 263
#define FILL 264
#define LIST 265
#define NUMBER 266
#define PROCESSOR 267
#define STRING 268
#define DEFINE 269
#define UPPER 270
#define HIGH 271
#define LOW 272
#define LSH 273
#define RSH 274
#define GREATER_EQUAL 275
#define LESS_EQUAL 276
#define EQUAL 277
#define NOT_EQUAL 278
#define LOGICAL_AND 279
#define LOGICAL_OR 280
#define ASSIGN_PLUS 281
#define ASSIGN_MINUS 282
#define ASSIGN_MULTIPLY 283
#define ASSIGN_DIVIDE 284
#define ASSIGN_MODULUS 285
#define ASSIGN_LSH 286
#define ASSIGN_RSH 287
#define ASSIGN_AND 288
#define ASSIGN_OR 289
#define ASSIGN_XOR 290
#define INCREMENT 291
#define DECREMENT 292
#define POSTINCREMENT 293
#define POSTDECREMENT 294
#define INDFOFFSET 295
#define TBL_NO_CHANGE 296
#define TBL_POST_INC 297
#define TBL_POST_DEC 298
#define TBL_PRE_INC 299
#define CONCAT 300
#define VAR 301
#define VARLAB_BEGIN 302
#define VAR_BEGIN 303
#define VAR_END 304




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 205 "parse.y"

  gpasmVal i;
  char *s;
  struct pnode *p;



/* Line 1676 of yacc.c  */
#line 158 "parse.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


