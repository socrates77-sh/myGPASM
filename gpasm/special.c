/* Implements special instruction mnemonics
   Copyright (C) 2001, 2002, 2003, 2004, 2005
   Craig Franklin

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
#include "gperror.h"
#include "special.h"
#include "directive.h"
#include "evaluate.h"

extern struct pnode *mk_constant(int value);
extern struct pnode *mk_list(struct pnode *head, struct pnode *tail);

struct pnode *make_constant_list(int value1, int value2)
{
  return mk_list(mk_constant(value1), mk_list(mk_constant(value2), NULL));
}

struct pnode *add_symbol_constant(struct pnode *parms, int value)
{
  return mk_list(HEAD(parms), mk_list(mk_constant(value), NULL));
}

/**************************************************************************/

static gpasmVal do_addcf(gpasmVal r,
                         char *name,
                         int arity,
                         struct pnode *parms)
{

  gpmessage(GPM_SPECIAL_MNEMONIC, NULL);

  do_insn("btfsc", make_constant_list(3, 0));
  do_insn("incf", parms);

  return r;
}

static gpasmVal do_adddcf(gpasmVal r,
                          char *name,
                          int arity,
                          struct pnode *parms)
{

  do_insn("btfsc", make_constant_list(3, 1));
  do_insn("incf", parms);

  return r;
}

static gpasmVal do_b(gpasmVal r,
                     char *name,
                     int arity,
                     struct pnode *parms)
{

  do_insn("goto", parms);

  return r;
}

static gpasmVal do_bc(gpasmVal r,
                      char *name,
                      int arity,
                      struct pnode *parms)
{

  do_insn("btfsc", make_constant_list(3, 0));
  do_insn("goto", parms);

  return r;
}

static gpasmVal do_bdc(gpasmVal r,
                       char *name,
                       int arity,
                       struct pnode *parms)
{

  do_insn("btfsc", make_constant_list(3, 1));
  do_insn("goto", parms);

  return r;
}

static gpasmVal do_bz(gpasmVal r,
                      char *name,
                      int arity,
                      struct pnode *parms)
{

  do_insn("btfsc", make_constant_list(3, 2));
  do_insn("goto", parms);

  return r;
}

static gpasmVal do_bnc(gpasmVal r,
                       char *name,
                       int arity,
                       struct pnode *parms)
{

  do_insn("btfss", make_constant_list(3, 0));
  do_insn("goto", parms);

  return r;
}

static gpasmVal do_bndc(gpasmVal r,
                        char *name,
                        int arity,
                        struct pnode *parms)
{

  do_insn("btfss", make_constant_list(3, 1));
  do_insn("goto", parms);

  return r;
}

static gpasmVal do_bnz(gpasmVal r,
                       char *name,
                       int arity,
                       struct pnode *parms)
{

  do_insn("btfss", make_constant_list(3, 2));
  do_insn("goto", parms);

  return r;
}

static gpasmVal do_clrc(gpasmVal r,
                        char *name,
                        int arity,
                        struct pnode *parms)
{

  if (arity)
  {
    gperror(GPE_TOO_MANY_ARGU, NULL);
  }

  do_insn("bcf", make_constant_list(3, 0));

  return r;
}

static gpasmVal do_clrdc(gpasmVal r,
                         char *name,
                         int arity,
                         struct pnode *parms)
{

  if (arity)
  {
    gperror(GPE_TOO_MANY_ARGU, NULL);
  }

  do_insn("bcf", make_constant_list(3, 1));

  return r;
}

static gpasmVal do_clrz(gpasmVal r,
                        char *name,
                        int arity,
                        struct pnode *parms)
{

  if (arity)
  {
    gperror(GPE_TOO_MANY_ARGU, NULL);
  }

  do_insn("bcf", make_constant_list(3, 2));

  return r;
}

static gpasmVal do_lcall(gpasmVal r,
                         char *name,
                         int arity,
                         struct pnode *parms)
{

  do_insn("pagesel", parms);
  do_insn("call", parms);

  return r;
}

static gpasmVal do_lgoto(gpasmVal r,
                         char *name,
                         int arity,
                         struct pnode *parms)
{

  do_insn("pagesel", parms);
  do_insn("goto", parms);

  return r;
}

static gpasmVal do_movfw(gpasmVal r,
                         char *name,
                         int arity,
                         struct pnode *parms)
{

  if (enforce_arity(arity, 1))
  {
    do_insn("movf", add_symbol_constant(parms, 0));
  }

  return r;
}

static gpasmVal do_negf(gpasmVal r,
                        char *name,
                        int arity,
                        struct pnode *parms)
{

  if ((arity == 1) || (arity == 2))
  {
    do_insn("comf", add_symbol_constant(parms, 1));
    do_insn("incf", parms);
  }
  else
  {
    enforce_arity(arity, 2);
  }

  return r;
}

static gpasmVal do_setc(gpasmVal r,
                        char *name,
                        int arity,
                        struct pnode *parms)
{

  if (arity)
  {
    gperror(GPE_TOO_MANY_ARGU, NULL);
  }

  do_insn("bsf", make_constant_list(3, 0));

  return r;
}

static gpasmVal do_setdc(gpasmVal r,
                         char *name,
                         int arity,
                         struct pnode *parms)
{

  if (arity)
  {
    gperror(GPE_TOO_MANY_ARGU, NULL);
  }

  do_insn("bsf", make_constant_list(3, 1));

  return r;
}

static gpasmVal do_setz(gpasmVal r,
                        char *name,
                        int arity,
                        struct pnode *parms)
{

  if (arity)
  {
    gperror(GPE_TOO_MANY_ARGU, NULL);
  }

  do_insn("bsf", make_constant_list(3, 2));

  return r;
}

static gpasmVal do_skpc(gpasmVal r,
                        char *name,
                        int arity,
                        struct pnode *parms)
{

  if (arity)
  {
    gperror(GPE_TOO_MANY_ARGU, NULL);
  }

  do_insn("btfss", make_constant_list(3, 0));

  return r;
}

static gpasmVal do_skpdc(gpasmVal r,
                         char *name,
                         int arity,
                         struct pnode *parms)
{

  if (arity)
  {
    gperror(GPE_TOO_MANY_ARGU, NULL);
  }

  do_insn("btfss", make_constant_list(3, 1));

  return r;
}

static gpasmVal do_skpz(gpasmVal r,
                        char *name,
                        int arity,
                        struct pnode *parms)
{

  if (arity)
  {
    gperror(GPE_TOO_MANY_ARGU, NULL);
  }

  do_insn("btfss", make_constant_list(3, 2));

  return r;
}

static gpasmVal do_skpnc(gpasmVal r,
                         char *name,
                         int arity,
                         struct pnode *parms)
{

  if (arity)
  {
    gperror(GPE_TOO_MANY_ARGU, NULL);
  }

  do_insn("btfsc", make_constant_list(3, 0));

  return r;
}

static gpasmVal do_skpndc(gpasmVal r,
                          char *name,
                          int arity,
                          struct pnode *parms)
{

  if (arity)
  {
    gperror(GPE_TOO_MANY_ARGU, NULL);
  }

  do_insn("btfsc", make_constant_list(3, 1));

  return r;
}

static gpasmVal do_skpnz(gpasmVal r,
                         char *name,
                         int arity,
                         struct pnode *parms)
{

  if (arity)
  {
    gperror(GPE_TOO_MANY_ARGU, NULL);
  }

  do_insn("btfsc", make_constant_list(3, 2));

  return r;
}

static gpasmVal do_subcf(gpasmVal r,
                         char *name,
                         int arity,
                         struct pnode *parms)
{

  do_insn("btfsc", make_constant_list(3, 0));
  do_insn("decf", parms);

  return r;
}

static gpasmVal do_subdcf(gpasmVal r,
                          char *name,
                          int arity,
                          struct pnode *parms)
{

  do_insn("btfsc", make_constant_list(3, 1));
  do_insn("decf", parms);

  return r;
}

static gpasmVal do_tstf(gpasmVal r,
                        char *name,
                        int arity,
                        struct pnode *parms)
{

  if (enforce_arity(arity, 1))
  {
    do_insn("movf", add_symbol_constant(parms, 1));
  }

  return r;
}

static gpasmVal do_mode(gpasmVal r,
                        char *name,
                        int arity,
                        struct pnode *parms)
{
  if (enforce_arity(arity, 1))
  {
    struct pnode *val = HEAD(parms);
    if ((val->tag == constant) &&
        (val->value.constant > 0x1f))
    {
      gpwarning(GPW_RANGE, NULL);
      val->value.constant &= 0x1f;
    }
    do_insn("movlw", parms);
    do_insn("movwm", NULL);
  }
  return r;
}

/* PIC 12-bit and 14-bit "Special" instruction set */
struct insn special[] = {
    {"addcf", 0, 0, INSN_CLASS_FUNC, 0, do_addcf},
    {"adddcf", 0, 0, INSN_CLASS_FUNC, 0, do_adddcf},
    {"b", 0, 0, INSN_CLASS_FUNC, 0, do_b},
    {"bc", 0, 0, INSN_CLASS_FUNC, 0, do_bc},
    {"bdc", 0, 0, INSN_CLASS_FUNC, 0, do_bdc},
    {"bz", 0, 0, INSN_CLASS_FUNC, 0, do_bz},
    {"bnc", 0, 0, INSN_CLASS_FUNC, 0, do_bnc},
    {"bndc", 0, 0, INSN_CLASS_FUNC, 0, do_bndc},
    {"bnz", 0, 0, INSN_CLASS_FUNC, 0, do_bnz},
    {"clrc", 0, 0, INSN_CLASS_FUNC, 0, do_clrc},
    {"clrdc", 0, 0, INSN_CLASS_FUNC, 0, do_clrdc},
    {"clrz", 0, 0, INSN_CLASS_FUNC, 0, do_clrz},
    {"lcall1", 0, 0, INSN_CLASS_FUNC, 0, do_lcall},
    {"lgoto1", 0, 0, INSN_CLASS_FUNC, 0, do_lgoto},
    {"movfw", 0, 0, INSN_CLASS_FUNC, 0, do_movfw},
    {"negf", 0, 0, INSN_CLASS_FUNC, 0, do_negf},
    {"setc", 0, 0, INSN_CLASS_FUNC, 0, do_setc},
    {"setdc", 0, 0, INSN_CLASS_FUNC, 0, do_setdc},
    {"setz", 0, 0, INSN_CLASS_FUNC, 0, do_setz},
    {"skpc", 0, 0, INSN_CLASS_FUNC, 0, do_skpc},
    {"skpdc", 0, 0, INSN_CLASS_FUNC, 0, do_skpdc},
    {"skpz", 0, 0, INSN_CLASS_FUNC, 0, do_skpz},
    {"skpnc", 0, 0, INSN_CLASS_FUNC, 0, do_skpnc},
    {"skpndc", 0, 0, INSN_CLASS_FUNC, 0, do_skpndc},
    {"skpnz", 0, 0, INSN_CLASS_FUNC, 0, do_skpnz},
    {"subcf", 0, 0, INSN_CLASS_FUNC, 0, do_subcf},
    {"subdcf", 0, 0, INSN_CLASS_FUNC, 0, do_subdcf},
    {"tstf", 0, 0, INSN_CLASS_FUNC, 0, do_tstf}};

const int num_op_special = TABLE_SIZE(special);

struct insn op_sx_mode = {"mode", 0, 0, INSN_CLASS_FUNC, 0, do_mode};
