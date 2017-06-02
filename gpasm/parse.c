
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 1 "parse.y"

/* Parser for gpasm
   Copyright (C) 1998, 1999, 2000, 2001, 2002, 2003, 2004, 2005
   James Bowman, Craig Franklin

This file is part of gputils.

gputils is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

gputils is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with gputils; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#include "stdhdr.h"

#include "libgputils.h"
#include "gpasm.h"
#include "evaluate.h"
#include "gperror.h"
#include "directive.h"
#include "lst.h"
#include "macro.h"
#include "coff.h"
#include "scan.h"

void yyerror(char *message)
{
  gperror(103, message);
}

int yylex(void);

/************************************************************************/

/* Some simple functions for building parse trees */

static struct pnode *mk_pnode(enum pnode_tag tag)
{
  struct pnode *new = malloc(sizeof(*new));
  new->tag = tag;
  return new;
}

struct pnode *mk_constant(int value)
{
  struct pnode *new = mk_pnode(constant);
  new->value.constant = value;
  return new;
}

struct pnode *mk_offset(struct pnode *p)
{
  struct pnode *new = mk_pnode(offset);
  new->value.offset = p;
  return new;
}

static struct pnode *mk_symbol(char *value)
{
  struct pnode *new = mk_pnode(symbol);
  new->value.symbol = value;
  return new;
}

static struct pnode *mk_string(char *value)
{
  struct pnode *new = mk_pnode(string);
  new->value.string = value;
  return new;
}

struct pnode *mk_list(struct pnode *head, struct pnode *tail)
{
  struct pnode *new = mk_pnode(list);
  new->value.list.head = head;
  new->value.list.tail = tail;
  return new;
}

static struct pnode *mk_2op(int op, struct pnode *p0, struct pnode *p1)
{
  struct pnode *new = mk_pnode(binop);
  new->value.binop.op = op;
  new->value.binop.p0 = p0;
  new->value.binop.p1 = p1;
  return new;
}

static struct pnode *mk_1op(int op, struct pnode *p0)
{
  struct pnode *new = mk_pnode(unop);  new->value.unop.op = op;
  new->value.unop.p0 = p0;
  return new;
}

/************************************************************************/
/* shared functions */

gpasmVal set_label(char *label, struct pnode *parms)
{
  gpasmVal value = 0;

  if (asm_enabled()) {
    value = do_or_append_insn("set", parms);
    if (!state.mac_prev) {
      set_global(label, value, TEMPORARY, gvt_constant);
    }
  }

  return value;
}

int return_op(int operation);

void next_line(int value)
{
  char l[BUFSIZ];
  char *e = l;

  if ((state.src->type == src_macro) ||
      (state.src->type == src_while)) {
    /* while loops can be defined inside a macro or nested */
    if (state.mac_prev) {
      state.lst.line.linetype = none;
      if (state.mac_body)
        state.mac_body->src_line = strdup(state.src->lst.m->src_line);
    }

    if (((state.src->type == src_while) || (state.lst.expand)) &&
        (state.pass == 2)) {
      assert(state.src->lst.m->src_line != NULL);
      lst_format_line(state.src->lst.m->src_line, value);
    }

    if (state.src->lst.m->next) {
      state.src->lst.m = state.src->lst.m->next;
    }
  } else if ((state.src->type == src_file) &&
             (state.src->lst.f != NULL)) {
    fgets(l, BUFSIZ, state.src->lst.f);
    l[strlen(l) - 1] = '\0';    /* Eat the trailing newline */

    if (state.mac_prev) {
      state.lst.line.linetype = none;
      if (state.mac_body)
        state.mac_body->src_line = strdup(l);
    }

    if (state.pass == 2) {
      lst_format_line(e, value);
    }
  }

  state.src->line_number++;

  switch (state.next_state) {
    case state_exitmacro:
      execute_exitm();
      break;

    case state_include:
      open_src(state.next_buffer.file, 1);
      free(state.next_buffer.file);
      break;

    case state_macro:
      /* push the label for local directive */
      state.stTop = push_macro_symbol_table(state.stTop);
      execute_macro(state.next_buffer.macro, 0);
      break;

    case state_section:
      /* create a new coff section */
      coff_new_section(state.obj.new_sec_name,
                       state.obj.new_sec_addr,
                       state.obj.new_sec_flags);
      break;

    case state_while:
      execute_macro(state.next_buffer.macro, 1);
      break;

    default:
      break;
  }

}


/************************************************************************/



/* Line 189 of yacc.c  */
#line 276 "parse.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


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

/* Line 214 of yacc.c  */
#line 205 "parse.y"

  gpasmVal i;
  char *s;
  struct pnode *p;



/* Line 214 of yacc.c  */
#line 418 "parse.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 430 "parse.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   481

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  71
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  41
/* YYNRULES -- Number of rules.  */
#define YYNRULES  130
/* YYNRULES -- Number of states.  */
#define YYNSTATES  197

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   304

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      65,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    63,     2,     2,    69,    62,    26,     2,
      70,    66,    60,    58,    67,    59,     2,    61,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    68,     2,
      24,    31,    25,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    56,     2,    57,    28,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    27,     2,    64,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    29,
      30,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     5,     9,    13,    18,    23,    27,
      31,    34,    36,    38,    40,    42,    43,    48,    51,    52,
      57,    58,    63,    68,    72,    75,    78,    82,    89,    97,
      98,   106,   107,   114,   119,   120,   123,   125,   128,   131,
     135,   137,   141,   143,   147,   149,   151,   153,   155,   157,
     159,   161,   163,   165,   167,   172,   174,   177,   181,   184,
     186,   189,   192,   195,   198,   200,   204,   206,   208,   212,
     214,   216,   220,   222,   224,   228,   230,   232,   234,   236,
     240,   242,   244,   246,   248,   250,   252,   254,   258,   260,
     262,   264,   268,   270,   272,   274,   278,   280,   282,   284,
     286,   289,   291,   293,   295,   297,   299,   301,   303,   305,
     307,   309,   311,   315,   319,   321,   323,   325,   327,   329,
     331,   333,   337,   341,   343,   347,   351,   353,   357,   358,
     363
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      72,     0,    -1,    -1,    -1,    72,    73,    74,    -1,    72,
       1,    65,    -1,   108,    86,    88,    65,    -1,   108,    31,
      88,    65,    -1,   108,    43,    65,    -1,   108,    42,    65,
      -1,   108,    76,    -1,    76,    -1,     8,    -1,     6,    -1,
      65,    -1,    -1,    12,    77,     4,    65,    -1,    10,    65,
      -1,    -1,    10,    78,   109,    65,    -1,    -1,    75,    79,
      87,    65,    -1,    14,     4,    13,    65,    -1,    14,     4,
      65,    -1,    14,    65,    -1,     4,    65,    -1,     4,    87,
      65,    -1,     9,     4,    66,    67,    88,    65,    -1,     9,
       4,    87,    66,    67,    88,    65,    -1,    -1,     5,    88,
      65,    80,    82,     7,    65,    -1,    -1,     5,    65,    81,
      82,     7,    65,    -1,     5,     1,     7,    65,    -1,    -1,
      82,    83,    -1,    65,    -1,    84,    65,    -1,   108,    65,
      -1,   108,    88,    65,    -1,    85,    -1,    84,    67,    85,
      -1,   107,    -1,   107,    68,    88,    -1,    32,    -1,    33,
      -1,    34,    -1,    35,    -1,    36,    -1,    37,    -1,    38,
      -1,    39,    -1,    40,    -1,    41,    -1,    88,    56,    88,
      57,    -1,    88,    -1,    88,    67,    -1,    88,    67,    87,
      -1,    67,    87,    -1,    67,    -1,    42,    87,    -1,    43,
      87,    -1,    87,    42,    -1,    87,    43,    -1,    90,    -1,
      88,    89,    90,    -1,    31,    -1,    92,    -1,    90,    91,
      92,    -1,    30,    -1,    94,    -1,    92,    93,    94,    -1,
      29,    -1,    96,    -1,    94,    95,    96,    -1,    26,    -1,
      27,    -1,    28,    -1,    98,    -1,    96,    97,    98,    -1,
      24,    -1,    25,    -1,    22,    -1,    23,    -1,    20,    -1,
      21,    -1,   100,    -1,    98,    99,   100,    -1,    18,    -1,
      19,    -1,   102,    -1,   100,   101,   102,    -1,    58,    -1,
      59,    -1,   104,    -1,   102,   103,   104,    -1,    60,    -1,
      61,    -1,    62,    -1,   106,    -1,   105,   106,    -1,    15,
      -1,    16,    -1,    17,    -1,    59,    -1,    63,    -1,    64,
      -1,    58,    -1,   107,    -1,    11,    -1,    69,    -1,    13,
      -1,    70,    88,    66,    -1,    56,    88,    57,    -1,    60,
      -1,    48,    -1,    49,    -1,    50,    -1,    42,    -1,    43,
      -1,     4,    -1,    54,    88,    66,    -1,    54,    88,    55,
      -1,     3,    -1,    53,    88,    66,    -1,    53,    88,    55,
      -1,   110,    -1,   110,    67,   109,    -1,    -1,     4,   111,
      89,    90,    -1,    90,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   306,   306,   310,   309,   315,   322,   334,   343,   352,
     361,   434,   477,   477,   480,   493,   493,   500,   505,   505,
     512,   512,   519,   525,   530,   535,   540,   545,   561,   578,
     577,   596,   595,   613,   619,   621,   628,   630,   637,   646,
     657,   659,   663,   670,   678,   678,   678,   679,   679,   679,
     679,   680,   680,   680,   683,   689,   694,   699,   704,   709,
     714,   719,   724,   729,   736,   738,   746,   749,   751,   759,
     762,   764,   772,   775,   777,   785,   785,   785,   788,   790,
     798,   798,   798,   798,   798,   798,   801,   803,   811,   811,
     814,   816,   824,   824,   827,   829,   837,   837,   837,   840,
     842,   849,   849,   849,   849,   849,   849,   849,   852,   857,
     862,   867,   872,   877,   882,   887,   892,   897,   902,   907,
     914,   919,   924,   932,   937,   945,   955,   960,   968,   967,
     979
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "LABEL", "IDENTIFIER", "CBLOCK",
  "DEBUG_LINE", "ENDC", "ERRORLEVEL", "FILL", "LIST", "NUMBER",
  "PROCESSOR", "STRING", "DEFINE", "UPPER", "HIGH", "LOW", "LSH", "RSH",
  "GREATER_EQUAL", "LESS_EQUAL", "EQUAL", "NOT_EQUAL", "'<'", "'>'", "'&'",
  "'|'", "'^'", "LOGICAL_AND", "LOGICAL_OR", "'='", "ASSIGN_PLUS",
  "ASSIGN_MINUS", "ASSIGN_MULTIPLY", "ASSIGN_DIVIDE", "ASSIGN_MODULUS",
  "ASSIGN_LSH", "ASSIGN_RSH", "ASSIGN_AND", "ASSIGN_OR", "ASSIGN_XOR",
  "INCREMENT", "DECREMENT", "POSTINCREMENT", "POSTDECREMENT", "INDFOFFSET",
  "TBL_NO_CHANGE", "TBL_POST_INC", "TBL_POST_DEC", "TBL_PRE_INC", "CONCAT",
  "VAR", "VARLAB_BEGIN", "VAR_BEGIN", "VAR_END", "'['", "']'", "'+'",
  "'-'", "'*'", "'/'", "'%'", "'!'", "'~'", "'\\n'", "')'", "','", "':'",
  "'$'", "'('", "$accept", "program", "$@1", "line", "decimal_ops",
  "statement", "$@2", "$@3", "$@4", "$@5", "$@6", "const_block",
  "const_line", "const_def_list", "const_def", "assign_equal_ops",
  "parameter_list", "expr", "e9op", "e8", "e8op", "e7", "e7op", "e6",
  "e6op", "e5", "e5op", "e4", "e4op", "e3", "e3op", "e2", "e2op", "e1",
  "e1op", "e0", "cidentifier", "label_concat", "list_block", "list_expr",
  "$@7", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,    60,    62,    38,   124,    94,   279,
     280,    61,   281,   282,   283,   284,   285,   286,   287,   288,
     289,   290,   291,   292,   293,   294,   295,   296,   297,   298,
     299,   300,   301,   302,   303,   304,    91,    93,    43,    45,
      42,    47,    37,    33,   126,    10,    41,    44,    58,    36,
      40
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    71,    72,    73,    72,    72,    74,    74,    74,    74,
      74,    74,    75,    75,    76,    77,    76,    76,    78,    76,
      79,    76,    76,    76,    76,    76,    76,    76,    76,    80,
      76,    81,    76,    76,    82,    82,    83,    83,    83,    83,
      84,    84,    85,    85,    86,    86,    86,    86,    86,    86,
      86,    86,    86,    86,    87,    87,    87,    87,    87,    87,
      87,    87,    87,    87,    88,    88,    89,    90,    90,    91,
      92,    92,    93,    94,    94,    95,    95,    95,    96,    96,
      97,    97,    97,    97,    97,    97,    98,    98,    99,    99,
     100,   100,   101,   101,   102,   102,   103,   103,   103,   104,
     104,   105,   105,   105,   105,   105,   105,   105,   106,   106,
     106,   106,   106,   106,   106,   106,   106,   106,   106,   106,
     107,   107,   107,   108,   108,   108,   109,   109,   111,   110,
     110
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     0,     3,     3,     4,     4,     3,     3,
       2,     1,     1,     1,     1,     0,     4,     2,     0,     4,
       0,     4,     4,     3,     2,     2,     3,     6,     7,     0,
       7,     0,     6,     4,     0,     2,     1,     2,     2,     3,
       1,     3,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     4,     1,     2,     3,     2,     1,
       2,     2,     2,     2,     1,     3,     1,     1,     3,     1,
       1,     3,     1,     1,     3,     1,     1,     1,     1,     3,
       1,     1,     1,     1,     1,     1,     1,     3,     1,     1,
       1,     3,     1,     1,     1,     3,     1,     1,     1,     1,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     1,     1,     1,     1,     1,     1,
       1,     3,     3,     1,     3,     3,     1,     3,     0,     4,
       1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,     0,     0,     5,   123,     0,     0,    13,
      12,     0,    18,    15,     0,     0,    14,     4,    20,    11,
       0,   120,   109,   111,   101,   102,   103,   118,   119,   115,
     116,   117,     0,     0,   107,   104,   114,   105,   106,    25,
      59,   110,     0,     0,    55,    64,    67,    70,    73,    78,
      86,    90,    94,     0,    99,   108,     0,   118,   119,    31,
       0,     0,    17,     0,     0,     0,    24,     0,     0,     0,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
       0,     0,    10,     0,    60,    61,     0,     0,    58,     0,
      62,    63,    26,    66,     0,    56,     0,    69,     0,    72,
       0,    75,    76,    77,     0,    84,    85,    82,    83,    80,
      81,     0,    88,    89,     0,    92,    93,     0,    96,    97,
      98,     0,   100,     0,    34,    29,     0,     0,   120,   130,
       0,   126,     0,     0,    23,   125,   124,     0,     0,     9,
       8,     0,   122,   121,   113,   112,     0,    57,    65,    68,
      71,    74,    79,    87,    91,    95,    33,     0,    34,     0,
       0,     0,    19,     0,    16,    22,    21,     7,     6,    54,
       0,    36,    35,     0,    40,    42,     0,     0,     0,     0,
       0,   127,    32,    37,     0,     0,    38,     0,     0,    27,
       0,   129,    41,    43,    39,    30,    28
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     4,    17,    18,    19,    64,    63,    68,   158,
     124,   157,   172,   173,   174,    83,    43,    44,    96,    45,
      98,    46,   100,    47,   104,    48,   111,    49,   114,    50,
     117,    51,   121,    52,    53,    54,    55,   176,   130,   131,
     161
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -149
static const yytype_int16 yypact[] =
{
    -149,    87,  -149,   -51,   151,  -149,  -149,    67,     0,  -149,
    -149,    23,   -34,  -149,    16,   277,  -149,  -149,  -149,  -149,
     416,  -149,  -149,  -149,  -149,  -149,  -149,   192,   192,  -149,
    -149,  -149,   277,   277,  -149,  -149,  -149,  -149,  -149,  -149,
     192,  -149,   277,    64,   -23,    17,    28,   156,   190,    86,
      54,   -39,  -149,   340,  -149,  -149,    30,  -149,  -149,  -149,
     -12,   131,  -149,   300,    73,   -10,  -149,    45,   192,   277,
    -149,  -149,  -149,  -149,  -149,  -149,  -149,  -149,  -149,  -149,
       3,    24,  -149,   277,   124,   124,    48,    -5,   124,   -21,
    -149,  -149,  -149,  -149,   277,   192,   277,  -149,   277,  -149,
     277,  -149,  -149,  -149,   277,  -149,  -149,  -149,  -149,  -149,
    -149,   277,  -149,  -149,   277,  -149,  -149,   277,  -149,  -149,
    -149,   277,  -149,    33,  -149,  -149,    41,    96,    89,    17,
      53,    55,    59,    68,  -149,  -149,  -149,   107,     1,  -149,
    -149,     7,  -149,  -149,  -149,  -149,    10,   124,    17,    28,
     156,   190,    86,    54,   -39,  -149,  -149,   358,  -149,   277,
      74,   112,  -149,   300,  -149,  -149,  -149,  -149,  -149,  -149,
      80,  -149,  -149,   -37,  -149,    85,   215,   364,     9,   277,
     277,  -149,  -149,  -149,     8,   277,  -149,    20,    93,  -149,
      63,    17,  -149,   112,  -149,  -149,  -149
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -149,  -149,  -149,  -149,  -149,   144,  -149,  -149,  -149,  -149,
    -149,    11,  -149,  -149,   -14,  -149,   -22,    -8,    14,   -61,
    -149,    78,  -149,    88,  -149,    82,  -149,    81,  -149,    79,
    -149,    61,  -149,    97,  -149,   146,  -148,   198,    43,  -149,
    -149
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -129
static const yytype_int16 yytable[] =
{
      60,    56,   129,   133,    21,    84,    85,    67,    93,   175,
      93,    22,    21,    23,     5,    24,    25,    26,    88,    93,
      65,   118,   119,   120,    86,    87,    93,    61,   183,   175,
     184,    62,    93,    94,    89,   148,   175,   123,    93,   127,
      93,    93,    57,    58,    95,   145,   137,    97,    29,    30,
      31,    93,   144,   125,    32,   134,    33,    99,    34,    35,
      36,   138,    32,    37,    38,    59,   167,   169,   139,    41,
      42,    21,   168,   147,   189,   141,    93,   132,    22,    93,
      23,    66,    24,    25,    26,   194,   146,     2,     3,   140,
      -3,    -3,    -3,    -3,    93,    -3,    -3,    -3,   156,    -3,
     135,    -3,   129,   142,   112,   113,    90,    91,   159,    27,
      28,   136,   115,   116,   143,    29,    30,    31,   162,   191,
    -128,    32,   163,    33,   164,    34,    35,    36,   196,    92,
      37,    38,    39,   165,    40,    21,    41,    42,    90,    91,
      -3,   179,    22,    93,    23,   182,    24,    25,    26,    90,
      91,   178,    -3,   185,     6,     7,     8,     9,   195,    10,
      11,    12,   160,    13,    82,    14,    90,    91,   187,   177,
     192,   190,   166,    27,    28,   180,   149,   193,   154,    29,
      30,    31,   101,   102,   103,    32,   151,    33,   150,    34,
      35,    36,   152,   153,    37,    38,    21,   126,    40,   122,
      41,    42,    20,    22,    15,    23,   181,    24,    25,    26,
     105,   106,   107,   108,   109,   110,    16,     0,   155,    21,
       0,     0,     0,     0,     0,     0,    22,     0,    23,     0,
      24,    25,    26,     0,    27,    28,     0,     0,     0,     0,
      29,    30,    31,     0,     0,     0,    32,     0,    33,     0,
      34,    35,    36,     0,     0,    37,    38,    57,    58,    40,
       0,    41,    42,    29,    30,    31,     0,     0,     0,    32,
       0,    33,     0,    34,    35,    36,     0,     0,    37,    38,
     186,    21,     0,     0,    41,    42,     0,     0,    22,     0,
      23,     0,    24,    25,    26,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   128,     0,     0,     0,     0,     0,
       0,    22,     0,    23,     0,    24,    25,    26,     0,    57,
      58,     0,     0,     0,     0,    29,    30,    31,     0,     0,
       0,    32,     0,    33,     0,    34,    35,    36,     0,     0,
      37,    38,    57,    58,    21,     0,    41,    42,    29,    30,
      31,    22,     0,    23,    32,     0,    33,     0,    34,    35,
      36,     6,    21,    37,    38,   170,     0,     6,    21,    41,
      42,   188,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,    58,     0,     0,     0,     0,    29,    30,
      31,     0,     0,     0,    32,     0,    33,     0,     0,     0,
      36,     0,     0,     0,     0,     0,     0,     0,     0,    41,
      42,    15,    32,     0,     0,     0,     0,    15,    32,     0,
       7,     8,     9,   171,    10,    11,    12,     0,    13,   171,
      14,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    16
};

static const yytype_int16 yycheck[] =
{
       8,     1,    63,    13,     4,    27,    28,    15,    31,   157,
      31,    11,     4,    13,    65,    15,    16,    17,    40,    31,
       4,    60,    61,    62,    32,    33,    31,     4,    65,   177,
      67,    65,    31,    56,    42,    96,   184,     7,    31,    61,
      31,    31,    42,    43,    67,    66,    68,    30,    48,    49,
      50,    31,    57,    65,    54,    65,    56,    29,    58,    59,
      60,    69,    54,    63,    64,    65,    65,    57,    65,    69,
      70,     4,    65,    95,    65,    83,    31,     4,    11,    31,
      13,    65,    15,    16,    17,    65,    94,     0,     1,    65,
       3,     4,     5,     6,    31,     8,     9,    10,    65,    12,
      55,    14,   163,    55,    18,    19,    42,    43,    67,    42,
      43,    66,    58,    59,    66,    48,    49,    50,    65,   180,
      31,    54,    67,    56,    65,    58,    59,    60,    65,    65,
      63,    64,    65,    65,    67,     4,    69,    70,    42,    43,
      53,    67,    11,    31,    13,    65,    15,    16,    17,    42,
      43,   159,    65,    68,     3,     4,     5,     6,    65,     8,
       9,    10,    66,    12,    20,    14,    42,    43,   176,   158,
     184,   179,    65,    42,    43,   161,    98,   185,   117,    48,
      49,    50,    26,    27,    28,    54,   104,    56,   100,    58,
      59,    60,   111,   114,    63,    64,     4,    66,    67,    53,
      69,    70,     4,    11,    53,    13,   163,    15,    16,    17,
      20,    21,    22,    23,    24,    25,    65,    -1,   121,     4,
      -1,    -1,    -1,    -1,    -1,    -1,    11,    -1,    13,    -1,
      15,    16,    17,    -1,    42,    43,    -1,    -1,    -1,    -1,
      48,    49,    50,    -1,    -1,    -1,    54,    -1,    56,    -1,
      58,    59,    60,    -1,    -1,    63,    64,    42,    43,    67,
      -1,    69,    70,    48,    49,    50,    -1,    -1,    -1,    54,
      -1,    56,    -1,    58,    59,    60,    -1,    -1,    63,    64,
      65,     4,    -1,    -1,    69,    70,    -1,    -1,    11,    -1,
      13,    -1,    15,    16,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     4,    -1,    -1,    -1,    -1,    -1,
      -1,    11,    -1,    13,    -1,    15,    16,    17,    -1,    42,
      43,    -1,    -1,    -1,    -1,    48,    49,    50,    -1,    -1,
      -1,    54,    -1,    56,    -1,    58,    59,    60,    -1,    -1,
      63,    64,    42,    43,     4,    -1,    69,    70,    48,    49,
      50,    11,    -1,    13,    54,    -1,    56,    -1,    58,    59,
      60,     3,     4,    63,    64,     7,    -1,     3,     4,    69,
      70,     7,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    42,    43,    -1,    -1,    -1,    -1,    48,    49,
      50,    -1,    -1,    -1,    54,    -1,    56,    -1,    -1,    -1,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    69,
      70,    53,    54,    -1,    -1,    -1,    -1,    53,    54,    -1,
       4,     5,     6,    65,     8,     9,    10,    -1,    12,    65,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    72,     0,     1,    73,    65,     3,     4,     5,     6,
       8,     9,    10,    12,    14,    53,    65,    74,    75,    76,
     108,     4,    11,    13,    15,    16,    17,    42,    43,    48,
      49,    50,    54,    56,    58,    59,    60,    63,    64,    65,
      67,    69,    70,    87,    88,    90,    92,    94,    96,    98,
     100,   102,   104,   105,   106,   107,     1,    42,    43,    65,
      88,     4,    65,    78,    77,     4,    65,    88,    79,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    76,    86,    87,    87,    88,    88,    87,    88,
      42,    43,    65,    31,    56,    67,    89,    30,    91,    29,
      93,    26,    27,    28,    95,    20,    21,    22,    23,    24,
      25,    97,    18,    19,    99,    58,    59,   101,    60,    61,
      62,   103,   106,     7,    81,    65,    66,    87,     4,    90,
     109,   110,     4,    13,    65,    55,    66,    87,    88,    65,
      65,    88,    55,    66,    57,    66,    88,    87,    90,    92,
      94,    96,    98,   100,   102,   104,    65,    82,    80,    67,
      66,   111,    65,    67,    65,    65,    65,    65,    65,    57,
       7,    65,    83,    84,    85,   107,   108,    82,    88,    67,
      89,   109,    65,    65,    67,    68,    65,    88,     7,    65,
      88,    90,    85,    88,    65,    65,    65
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 3:

/* Line 1455 of yacc.c  */
#line 310 "parse.y"
    {
          state.lst.line.was_org = state.org;
          state.lst.line.linetype = none;
          state.next_state = state_nochange;
        }
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 316 "parse.y"
    {
          next_line(0);
        }
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 323 "parse.y"
    {
          struct pnode *parms;
          int exp_result;

          exp_result = do_insn("set", mk_list((yyvsp[(3) - (4)].p), NULL));
          parms = mk_list(mk_2op(return_op((yyvsp[(2) - (4)].i)),
                                 mk_symbol((yyvsp[(1) - (4)].s)),
                                 mk_constant(exp_result)), NULL);
          next_line(set_label((yyvsp[(1) - (4)].s), parms));
        }
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 335 "parse.y"
    {
          struct pnode *parms;

          /* implements i = 6 + 1 */
          parms = mk_list((yyvsp[(3) - (4)].p), NULL);
          next_line(set_label((yyvsp[(1) - (4)].s), parms));
        }
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 344 "parse.y"
    {
          struct pnode *parms;

          /* implements i-- */
          parms = mk_list(mk_1op(DECREMENT, mk_symbol((yyvsp[(1) - (3)].s))), NULL);
          next_line(set_label((yyvsp[(1) - (3)].s), parms));
        }
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 353 "parse.y"
    {
          struct pnode *parms;

          /* implements i++ */
          parms = mk_list(mk_1op(INCREMENT, mk_symbol((yyvsp[(1) - (3)].s))), NULL);
          next_line(set_label((yyvsp[(1) - (3)].s), parms));
        }
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 362 "parse.y"
    {
          if (asm_enabled() && (state.lst.line.linetype == none)) {
            if (IS_RAM_ORG) {
              /* alias to next definition */
              state.lst.line.linetype = res;
            }
            else
              state.lst.line.linetype = insn;
          }

          if (asm_enabled()) {
            if (state.mac_head) {
              /* This is a macro definition.  Set it up */
              struct symbol *mac;
              struct macro_head *h = NULL;

              mac = get_symbol(state.stMacros, (yyvsp[(1) - (2)].s));
              if (mac)
                h = get_symbol_annotation(mac);

              /* It's not an error if macro was defined on pass 1 and
                 we're in pass 2. */
              if (h &&
                  !((h->pass == 1) && (state.pass == 2))) {
                gperror(GPE_DUPLICATE_MACRO, NULL);
              } else {
                if (!mac)
                  mac = add_symbol(state.stMacros, (yyvsp[(1) - (2)].s));
                annotate_symbol(mac, state.mac_head);
                h = state.mac_head;
                h->line_number = state.src->line_number;
                h->file_symbol = state.src->file_symbol;
              }
              h->pass = state.pass;

              /* The macro is defined so allow calls. */
              if (state.pass == 2)
                h->defined = 1;

              state.mac_head = NULL;
            } else if (!state.mac_prev) {
              /* Outside a macro, just define the label. */
              switch (state.lst.line.linetype) {
              case sec:
                strncpy(state.obj.new_sec_name, (yyvsp[(1) - (2)].s), 78);
                break;
              case set:
                set_global((yyvsp[(1) - (2)].s), (yyvsp[(2) - (2)].i), TEMPORARY, gvt_constant);
                break;
              case org:
              case equ:
                set_global((yyvsp[(1) - (2)].s), (yyvsp[(2) - (2)].i), PERMANENT, gvt_constant);
                break;
              case insn:
              case data:
              case res:
                if (IS_RAM_ORG)
                  set_global((yyvsp[(1) - (2)].s), (yyvsp[(2) - (2)].i), PERMANENT, gvt_static);
                else
                  set_global((yyvsp[(1) - (2)].s), (yyvsp[(2) - (2)].i), PERMANENT, gvt_address);
                break;
              case dir:
                gperror(GPE_ILLEGAL_LABEL, NULL);
                break;
              default:
                break;
              }
            }
          }
          next_line((yyvsp[(2) - (2)].i));
        }
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 435 "parse.y"
    {
          if (state.mac_head) {
            /* This is a macro definition, but the label was missing */
            state.mac_head = NULL;
            gperror(GPE_NO_MACRO_NAME, NULL);
          } else {
            if (state.found_end) {
              switch (state.src->type) {
              case src_while:
                gperror(GPE_EXPECTED, "Expected (ENDW)");
                break;

              case src_macro:
                gperror(GPW_EXPECTED,"Expected (ENDM)");
                /* fall through */

              default:
                break;
              }

              if (state.astack != NULL) {
                struct amode *old;

                while (state.astack) {
                  old = state.astack;
                  state.astack = state.astack->prev;
                  free(old);
                }
                gpwarning(GPW_EXPECTED, "Expected (ENDIF)");
              }
            }

            next_line(0);

            if (state.found_end) {
              found_end();
              YYACCEPT;
            }
          }
        }
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 481 "parse.y"
    {
          if (!state.mac_prev) {
            if (!IS_RAM_ORG)
              /* We want to have r as the value to assign to label */
              (yyval.i) = gp_processor_byte_to_org(state.device.class, state.org);
            else
              (yyval.i) = state.org;
          } else {
            macro_append();
          }
        }
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 493 "parse.y"
    {  force_ident = 1; }
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 495 "parse.y"
    {
          (yyval.i) = do_or_append_insn((yyvsp[(1) - (4)].s), mk_list(mk_symbol((yyvsp[(3) - (4)].s)), NULL));
          force_ident = 0;
        }
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 501 "parse.y"
    {
          (yyval.i) = do_or_append_insn((yyvsp[(1) - (2)].s), NULL);
        }
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 505 "parse.y"
    {  force_decimal = 1; }
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 507 "parse.y"
    {
          (yyval.i) = do_or_append_insn((yyvsp[(1) - (4)].s), (yyvsp[(3) - (4)].p));
          force_decimal = 0;
        }
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 512 "parse.y"
    {  force_decimal = 1; }
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 514 "parse.y"
    {
          (yyval.i) = do_or_append_insn((yyvsp[(1) - (4)].s), (yyvsp[(3) - (4)].p));
          force_decimal = 0;
        }
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 520 "parse.y"
    {
          (yyval.i) = do_or_append_insn((yyvsp[(1) - (4)].s), mk_list(mk_string((yyvsp[(2) - (4)].s)),
              mk_list(mk_string((yyvsp[(3) - (4)].s)), NULL)));
        }
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 526 "parse.y"
    {
          (yyval.i) = do_or_append_insn((yyvsp[(1) - (3)].s), mk_list(mk_string((yyvsp[(2) - (3)].s)), NULL));
        }
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 531 "parse.y"
    {
          (yyval.i) = do_or_append_insn((yyvsp[(1) - (2)].s), NULL);
        }
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 536 "parse.y"
    {
          (yyval.i) = do_or_append_insn((yyvsp[(1) - (2)].s), NULL);
        }
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 541 "parse.y"
    {
          (yyval.i) = do_or_append_insn((yyvsp[(1) - (3)].s), (yyvsp[(2) - (3)].p));
        }
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 546 "parse.y"
    {
          int number;
          int i;

          if (!state.mac_prev) {
            number = eval_fill_number((yyvsp[(5) - (6)].p));

            for (i = 0; i < number; i++) {
              (yyval.i) = do_insn((yyvsp[(2) - (6)].s), NULL);
            }
          } else {
            macro_append();
          }
        }
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 562 "parse.y"
    {
          int number;
          int i;

          if (!state.mac_prev) {
            number = eval_fill_number((yyvsp[(6) - (7)].p));

            for (i = 0; i < number; i++) {
              (yyval.i) = do_insn((yyvsp[(2) - (7)].s), (yyvsp[(3) - (7)].p));
            }
          } else {
            macro_append();
          }
        }
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 578 "parse.y"
    {
          if (!state.mac_prev) {
            begin_cblock((yyvsp[(2) - (3)].p));
          } else {
            macro_append();
          }
          next_line(0);
        }
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 588 "parse.y"
    {
          if (state.mac_prev) {
            macro_append();
          }
          (yyval.i) = 0;
        }
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 596 "parse.y"
    {
          if (!state.mac_prev) {
            continue_cblock();
          } else {
            macro_append();
          }
          next_line(0);
        }
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 606 "parse.y"
    {
          if (state.mac_prev) {
            macro_append();
          }
          (yyval.i) = 0;
        }
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 614 "parse.y"
    {
          (yyval.i) = 0;
        }
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 622 "parse.y"
    {
          next_line(0);
        }
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 631 "parse.y"
    {
          if (state.mac_prev) {
            macro_append();
          }
        }
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 638 "parse.y"
    {
          if (!state.mac_prev) {
            cblock_expr(mk_symbol((yyvsp[(1) - (2)].s)));
          } else {
            macro_append();
          }
        }
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 647 "parse.y"
    {
          if (!state.mac_prev) {
            cblock_expr_incr(mk_symbol((yyvsp[(1) - (3)].s)), (yyvsp[(2) - (3)].p));
          } else {
            macro_append();
          }
        }
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 664 "parse.y"
    {
          if (!state.mac_prev) {
            cblock_expr((yyvsp[(1) - (1)].p));
          }
        }
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 671 "parse.y"
    {
          if (!state.mac_prev) {
            cblock_expr_incr((yyvsp[(1) - (3)].p), (yyvsp[(3) - (3)].p));
          }
        }
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 684 "parse.y"
    {
          (yyval.p) = mk_list(mk_constant(INDFOFFSET),
                       mk_list((yyvsp[(3) - (4)].p), (yyvsp[(1) - (4)].p)));
        }
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 690 "parse.y"
    {
          (yyval.p) = mk_list((yyvsp[(1) - (1)].p), NULL);
        }
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 695 "parse.y"
    {
          (yyval.p) = mk_list((yyvsp[(1) - (2)].p), mk_list(mk_symbol(""), NULL));
        }
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 700 "parse.y"
    {
          (yyval.p) = mk_list((yyvsp[(1) - (3)].p), (yyvsp[(3) - (3)].p));
        }
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 705 "parse.y"
    {
          (yyval.p) = mk_list(mk_symbol(""), (yyvsp[(2) - (2)].p));
        }
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 710 "parse.y"
    {
          (yyval.p) = mk_list(mk_symbol(""), mk_list(mk_symbol(""), NULL));
        }
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 715 "parse.y"
    {
          (yyval.p) = mk_list(mk_constant((yyvsp[(1) - (2)].i)), (yyvsp[(2) - (2)].p));
        }
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 720 "parse.y"
    {
          (yyval.p) = mk_list(mk_constant((yyvsp[(1) - (2)].i)), (yyvsp[(2) - (2)].p));
        }
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 725 "parse.y"
    {
          (yyval.p) = mk_list(mk_constant(POSTINCREMENT), (yyvsp[(1) - (2)].p));
        }
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 730 "parse.y"
    {
          (yyval.p) = mk_list(mk_constant(POSTDECREMENT), (yyvsp[(1) - (2)].p));
        }
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 739 "parse.y"
    {
          coerce_str1((yyvsp[(1) - (3)].p));
          coerce_str1((yyvsp[(3) - (3)].p));
          (yyval.p) = mk_2op((yyvsp[(2) - (3)].i), (yyvsp[(1) - (3)].p), (yyvsp[(3) - (3)].p));
        }
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 752 "parse.y"
    {
          coerce_str1((yyvsp[(1) - (3)].p));
          coerce_str1((yyvsp[(3) - (3)].p));
          (yyval.p) = mk_2op((yyvsp[(2) - (3)].i), (yyvsp[(1) - (3)].p), (yyvsp[(3) - (3)].p));
        }
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 765 "parse.y"
    {
          coerce_str1((yyvsp[(1) - (3)].p));
          coerce_str1((yyvsp[(3) - (3)].p));
          (yyval.p) = mk_2op((yyvsp[(2) - (3)].i), (yyvsp[(1) - (3)].p), (yyvsp[(3) - (3)].p));
        }
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 778 "parse.y"
    {
          coerce_str1((yyvsp[(1) - (3)].p));
          coerce_str1((yyvsp[(3) - (3)].p));
          (yyval.p) = mk_2op((yyvsp[(2) - (3)].i), (yyvsp[(1) - (3)].p), (yyvsp[(3) - (3)].p));
        }
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 791 "parse.y"
    {
          coerce_str1((yyvsp[(1) - (3)].p));
          coerce_str1((yyvsp[(3) - (3)].p));
          (yyval.p) = mk_2op((yyvsp[(2) - (3)].i), (yyvsp[(1) - (3)].p), (yyvsp[(3) - (3)].p));
        }
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 804 "parse.y"
    {
          coerce_str1((yyvsp[(1) - (3)].p));
          coerce_str1((yyvsp[(3) - (3)].p));
          (yyval.p) = mk_2op((yyvsp[(2) - (3)].i), (yyvsp[(1) - (3)].p), (yyvsp[(3) - (3)].p));
        }
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 817 "parse.y"
    {
          coerce_str1((yyvsp[(1) - (3)].p));
          coerce_str1((yyvsp[(3) - (3)].p));
          (yyval.p) = mk_2op((yyvsp[(2) - (3)].i), (yyvsp[(1) - (3)].p), (yyvsp[(3) - (3)].p));
        }
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 830 "parse.y"
    {
          coerce_str1((yyvsp[(1) - (3)].p));
          coerce_str1((yyvsp[(3) - (3)].p));
          (yyval.p) = mk_2op((yyvsp[(2) - (3)].i), (yyvsp[(1) - (3)].p), (yyvsp[(3) - (3)].p));
        }
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 843 "parse.y"
    {
          coerce_str1((yyvsp[(2) - (2)].p));
          (yyval.p) = mk_1op((yyvsp[(1) - (2)].i), (yyvsp[(2) - (2)].p));
        }
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 853 "parse.y"
    {
          (yyval.p) = (yyvsp[(1) - (1)].p);
        }
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 858 "parse.y"
    {
          (yyval.p) = mk_constant((yyvsp[(1) - (1)].i));
        }
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 863 "parse.y"
    {
          (yyval.p) = mk_symbol("$");
        }
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 868 "parse.y"
    {
          (yyval.p) = mk_string((yyvsp[(1) - (1)].s));
        }
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 873 "parse.y"
    {
          (yyval.p) = (yyvsp[(2) - (3)].p);
        }
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 878 "parse.y"
    {
          (yyval.p) = mk_offset((yyvsp[(2) - (3)].p));
        }
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 883 "parse.y"
    {
          (yyval.p) = mk_constant(TBL_NO_CHANGE);
        }
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 888 "parse.y"
    {
          (yyval.p) = mk_constant((yyvsp[(1) - (1)].i));
        }
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 893 "parse.y"
    {
          (yyval.p) = mk_constant((yyvsp[(1) - (1)].i));
        }
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 898 "parse.y"
    {
          (yyval.p) = mk_constant((yyvsp[(1) - (1)].i));
        }
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 903 "parse.y"
    {
          (yyval.p) = mk_constant((yyvsp[(1) - (1)].i));
        }
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 908 "parse.y"
    {
          (yyval.p) = mk_constant((yyvsp[(1) - (1)].i));
        }
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 915 "parse.y"
    {
          (yyval.p) = mk_symbol((yyvsp[(1) - (1)].s));
        }
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 920 "parse.y"
    {
          (yyval.p) = mk_2op(CONCAT, mk_symbol((yyvsp[(1) - (3)].s)), mk_1op(VAR, (yyvsp[(2) - (3)].p)));
        }
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 925 "parse.y"
    {
          (yyval.p) = mk_2op(CONCAT, mk_symbol((yyvsp[(1) - (3)].s)),
                        mk_2op(CONCAT, mk_1op(VAR, (yyvsp[(2) - (3)].p)), mk_symbol((yyvsp[(3) - (3)].s))));
        }
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 933 "parse.y"
    {
          (yyval.s) = (yyvsp[(1) - (1)].s);
        }
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 938 "parse.y"
    {
          if (asm_enabled() && !state.mac_prev) {
            (yyval.s) = evaluate_concatenation(mk_2op(CONCAT,  mk_symbol((yyvsp[(1) - (3)].s)),
                           mk_1op(VAR, (yyvsp[(2) - (3)].p))));
          }
        }
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 946 "parse.y"
    {
          if (asm_enabled() && !state.mac_prev) {
            (yyval.s) = evaluate_concatenation(mk_2op(CONCAT,  mk_symbol((yyvsp[(1) - (3)].s)),
                      mk_2op(CONCAT, mk_1op(VAR, (yyvsp[(2) - (3)].p)), mk_symbol((yyvsp[(3) - (3)].s)))));
          }
        }
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 956 "parse.y"
    {
          (yyval.p) = mk_list((yyvsp[(1) - (1)].p), NULL);
        }
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 961 "parse.y"
    {
          (yyval.p) = mk_list((yyvsp[(1) - (3)].p), (yyvsp[(3) - (3)].p));
        }
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 968 "parse.y"
    {
          if ((strcasecmp((yyvsp[(1) - (1)].s), "p") == 0) || (strcasecmp((yyvsp[(1) - (1)].s), "pe") == 0)) {
            force_ident = 1;
          }
        }
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 974 "parse.y"
    {
          (yyval.p) = mk_2op((yyvsp[(3) - (4)].i), mk_symbol((yyvsp[(1) - (4)].s)), (yyvsp[(4) - (4)].p));
          force_ident = 0;
        }
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 980 "parse.y"
    {
          (yyval.p) = (yyvsp[(1) - (1)].p);
        }
    break;



/* Line 1455 of yacc.c  */
#line 2780 "parse.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 985 "parse.y"


int return_op(int operation)
{
  /* returns an operator for the replacement of i+=1 with i=i+1*/
  switch(operation) {
  case ASSIGN_PLUS:     return '+';
  case ASSIGN_MINUS:    return '-';
  case ASSIGN_MULTIPLY: return '*';
  case ASSIGN_DIVIDE:   return '/';
  case ASSIGN_MODULUS:  return '%';
  case ASSIGN_LSH:      return LSH;
  case ASSIGN_RSH:      return RSH;
  case ASSIGN_AND:      return '&';
  case ASSIGN_OR:       return '|';
  case ASSIGN_XOR:      return '^';
  default:
    assert(0); /* Unhandled operator */
  }

  return 0;
}

