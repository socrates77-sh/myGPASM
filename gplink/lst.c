/* ".LST" file output for gplink
   Copyright (C) 2004, 2005
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
#include "gplink.h"
#include "cod.h"

#ifdef STDC_HEADERS
#include <stdarg.h>
#endif

static gp_boolean list_enabled;
static gp_section_type *line_section;

static void lst_eol(void)
{
  if (state.lst.f)
  {
    fputc('\n', state.lst.f);
    /*state.lst.line_number++;*/
    /*cod_lst_line(COD_NORMAL_LST_LINE);*/
  }
}

static void
lst_line(const char *format, ...)
{
  if (state.lst.f)
  {
    va_list args;
    va_start(args, format);
    vfprintf(state.lst.f, format, args);
    va_end(args);
    fputc('\n', state.lst.f);
  }
}

/* Print part of a line. Output must not contain newline. Needs call
   to lst_eol at end of line. */
static int lst_printf(const char *format, ...)
{
  int r = 0;
  if (state.lst.f)
  {
    va_list args;
    /*lst_check_page_start();*/
    va_start(args, format);
    r = vfprintf(state.lst.f, format, args);
    va_end(args);
  }
  return r;
}

void lst_memory_map(MemBlock *m)
{
#define MEM_IS_USED(m, i) ((m)->memory[i] & BYTE_USED_MASK)

  int i, j, base, row_used, num_per_line, num_per_block;
  unsigned int used;

  lst_line("");
  lst_line("");
  lst_line("MEMORY USAGE MAP ('X' = Used,  '-' = Unused)");
  lst_line("");

  num_per_line = 64;
  num_per_block = 16;

  while (m)
  {
    unsigned int max_mem = MAX_I_MEM;
    assert(m->memory != NULL);

    base = (m->base << I_MEM_BITS);

    for (i = 0; i < max_mem; i += num_per_line)
    {
      row_used = 0;

      for (j = 0; j < num_per_line; j++)
      {
        if (MEM_IS_USED(m, i + j))
        {
          row_used = 1;
          break;
        }
      }

      if (row_used)
      {
        lst_printf("%04X :", i + base);
        for (j = 0; j < num_per_line; j++)
        {
          if ((j % num_per_block) == 0)
          {
            lst_printf(" ");
          }
          lst_printf(MEM_IS_USED(m, i + j) ? "X" : "-");
        }

        lst_eol();
      }
    }

    m = m->next;
  }

  lst_line("");
  lst_line("All other memory blocks unused.");
  lst_line("");

/* it seems that MPASM includes config bytes into program memory usage
   * count for 16 bit cores. See gpasm testsuite:
   * gpasm/testsuite/gpasm.mchip/listfiles/configX.lst */
#define IS_PIC16 (state.class == PROC_CLASS_PIC16 || state.class == PROC_CLASS_PIC16E)

  used = gp_processor_byte_to_org(state.class, (!IS_PIC16 && state.processor) ? b_range_memory_used(state.i_memory, 0, gp_processor_org_to_byte(state.class, state.processor->config_addrs[0])) : b_memory_used(state.i_memory));
  lst_line("Program Memory %s Used: %5i", "Words", used);
  printf("Program Memory Words Used: %5i\n", used);
  /* maxrom is not the program memory size.
  lst_line("Program Memory %s Free: %5d", IS_16BIT_CORE ? "Bytes" : "Words", state.maxrom - used);
  */
  lst_eol();
}

static void
open_src(const char *name, gp_symbol_type *symbol)
{
  struct list_context *new = malloc(sizeof(*new));

  assert(name != NULL);

  new->f = fopen(name, "rt");
  if (new->f)
  {
    new->name = strdup(name);
    new->missing_source = false;
  }
  else
  {
    new->missing_source = true;
  }

  new->symbol = symbol;
  new->line_number = 1;
  new->prev = state.lst.src;

  state.lst.src = new;
}

static void
close_src(void)
{
  struct list_context *old;

  assert(state.lst.src != NULL);

  old = state.lst.src;
  state.lst.src = state.lst.src->prev;
  free(old);
}

gp_linenum_type *
find_line_number(gp_symbol_type *symbol, int line_number)
{
  gp_section_type *section;
  gp_linenum_type *line = NULL;

  section = state.object->sections;

  /* FIXME: This too slow. */
  while (section != NULL)
  {
    line = section->line_numbers;
    while (line != NULL)
    {
      if ((line->symbol == symbol) &&
          (line->line_number == line_number))
      {
        if (section != line_section)
        {
          /* switching sections, so update was_org with the new
             address */
          state.lst.was_org = line->address;
          line_section = section;
        }
        line->used = 1; // zwr: check macro duplicate lineno
        return line;
      }
      line = line->next;
    }
    section = section->next;
  }

  return NULL;
}

// zwr: check macro duplicate lineno and add to cod file
void write_macro_dup_line()
{
  gp_symbol_type *symbol;
  gp_section_type *section;
  gp_linenum_type *line = NULL;
  int org = 0;
  unsigned int len;

#define LINESIZ 520
  char dasmbuf[LINESIZ];

  symbol = state.lst.src->symbol;
  section = state.object->sections;

  /* FIXME: This too slow. */
  while (section != NULL)
  {
    line = section->line_numbers;
    while (line != NULL)
    {
      if ((line->symbol == symbol) && (line->used == 0))
      {
        state.lst.src->line_number = line->line_number;
        state.cod.emitting = 1;
        org = line->address;
        len = b_memory_get_unlisted_size(line_section->data, org);
        if (0 == len)
        {
          //lst_line("%42s %s", "", linebuf);
          cod_lst_line(COD_NORMAL_LST_LINE);
        }
        else
        {
          if (org & 1 || len < 2)
          {
            /* even address or less then two byts to disassemble: disassemble one byte */
            if (0 != len)
            {
              unsigned char byte;

              b_memory_assert_get(line_section->data, org, &byte);
              gp_disassemble_byte(line_section->data,
                                  org,
                                  state.class,
                                  dasmbuf,
                                  sizeof(dasmbuf));
              // lst_line("%06lx   %02x       %-24s %s",
              //          gp_processor_byte_to_org(state.class, org),
              //          (unsigned short)byte,
              //          expand(dasmbuf),
              //          linebuf);
              b_memory_set_listed(line_section->data, org, 1);
              state.lst.was_org = org;
              cod_lst_line(COD_NORMAL_LST_LINE);
              ++org;
            }
          }
          else
          {
            unsigned short word;
            int num_bytes;

            state.class->i_memory_get(line_section->data, org, &word);
            num_bytes = gp_disassemble_size(line_section->data,
                                            org,
                                            state.class,
                                            dasmbuf,
                                            sizeof(dasmbuf), len);
            // lst_line("%06lx   %04x     %-24s %s",
            //          gp_processor_byte_to_org(state.class, org),
            //          word,
            //          expand(dasmbuf),
            //          linebuf);
            b_memory_set_listed(line_section->data, org, num_bytes);
            state.lst.was_org = org;
            cod_lst_line(COD_NORMAL_LST_LINE);
            org += 2;
            if (num_bytes > 2)
            {
              state.lst.was_org = org;
              state.class->i_memory_get(line_section->data, org, &word);
              // lst_line("%06lx   %04x", gp_processor_byte_to_org(state.class, org), word);
              cod_lst_line(COD_NORMAL_LST_LINE);
              org += 2;
              if (line->next)
              {
                /* skip the line number for the other half of this instruction */
                line = line->next;
              }
            }
          }

          // printf("%02x %02x %04x %04x %01d\n", symbol->number,
          //        0, line->line_number, gp_processor_byte_to_org(state.class, line->address), line->used);
        }
      }
      line = line->next;
    }
    section = section->next;
  }

  return;
}

// zwr: for debug
void print_all_lineno(gp_symbol_type *symbol)
{
  gp_section_type *section;
  gp_linenum_type *line = NULL;

  section = state.object->sections;

  /* FIXME: This too slow. */
  while (section != NULL)
  {
    line = section->line_numbers;
    while (line != NULL)
    {
      if ((line->symbol == symbol))
      {

        printf("%02x %02x %04x %04x %01d\n", symbol->number,
               0, line->line_number, gp_processor_byte_to_org(state.class, line->address), line->used);
      }
      line = line->next;
    }
    section = section->next;
  }

  return;
}

static char *
expand(const char *buf)
{
  int is, id;
  static char dest[520], c;

  for (is = 0, id = 0; (c = buf[is]) != '\0' && id < sizeof(dest) - 2; ++is)
  {
    if (c == '\t')
    {
      unsigned int n = 8 - (id % 8);
      while (n-- && id < sizeof(dest) - 2)
        dest[id++] = ' ';
    }
    else
      dest[id++] = c;
  }
  dest[id] = '\0';
  return dest;
}

static void
write_src(int last_line)
{
#define LINESIZ 520
  char linebuf[LINESIZ];
  char dasmbuf[LINESIZ];
  char *pc;
  gp_linenum_type *line = NULL;
  gp_boolean first_time;
  int org = 0;

  /* if the source file wasn't found, can't write it to the list file */
  if (state.lst.src->missing_source)
    return;

  while (1)
  {

    /* when last_line is 0 print all lines, else print to last_line */
    if ((last_line) && (state.lst.src->line_number > last_line))
      break;

    if (fgets(linebuf, LINESIZ, state.lst.src->f) == NULL)
      break;

    state.lst.was_org = org;

    if (list_enabled)
    {

      /* Eat the trailing newline */
      pc = strrchr(linebuf, '\n');
      if (pc)
        *pc = 0;

      first_time = true;

      line = find_line_number(state.lst.src->symbol,
                              state.lst.src->line_number);
      while (line)
      {
        unsigned int len;

        /* print all instructions generated by this line of the source */

        if (line->line_number != state.lst.src->line_number)
          break;

        if (first_time == false)
        {
          /* only print the source line the first time */
          linebuf[0] = '\0';
        }
        state.cod.emitting = 1;
        org = line->address;
        len = b_memory_get_unlisted_size(line_section->data, org);
        if (0 == len)
        {
          lst_line("%42s %s", "", linebuf);
          cod_lst_line(COD_NORMAL_LST_LINE);
        }
        else
        {
          if (org & 1 || len < 2)
          {
            /* even address or less then two byts to disassemble: disassemble one byte */
            if (0 != len)
            {
              unsigned char byte;

              b_memory_assert_get(line_section->data, org, &byte);
              gp_disassemble_byte(line_section->data,
                                  org,
                                  state.class,
                                  dasmbuf,
                                  sizeof(dasmbuf));
              lst_line("%06lx   %02x       %-24s %s",
                       gp_processor_byte_to_org(state.class, org),
                       (unsigned short)byte,
                       expand(dasmbuf),
                       linebuf);
              b_memory_set_listed(line_section->data, org, 1);
              state.lst.was_org = org;
              cod_lst_line(COD_NORMAL_LST_LINE);
              ++org;
            }
          }
          else
          {
            unsigned short word;
            int num_bytes;

            state.class->i_memory_get(line_section->data, org, &word);
            num_bytes = gp_disassemble_size(line_section->data,
                                            org,
                                            state.class,
                                            dasmbuf,
                                            sizeof(dasmbuf), len);
            lst_line("%06lx   %04x     %-24s %s",
                     gp_processor_byte_to_org(state.class, org),
                     word,
                     expand(dasmbuf),
                     linebuf);
            b_memory_set_listed(line_section->data, org, num_bytes);
            state.lst.was_org = org;
            cod_lst_line(COD_NORMAL_LST_LINE);
            org += 2;
            if (num_bytes > 2)
            {
              state.lst.was_org = org;
              state.class->i_memory_get(line_section->data, org, &word);
              lst_line("%06lx   %04x", gp_processor_byte_to_org(state.class, org), word);
              cod_lst_line(COD_NORMAL_LST_LINE);
              org += 2;
              if (line->next)
              {
                /* skip the line number for the other half of this instruction */
                line = line->next;
              }
            }
          }
        }
        first_time = false;
        line = line->next;
      }

      if (first_time)
      {
        lst_line("%42s %s", "", linebuf);
        state.cod.emitting = 0;
        cod_lst_line(COD_NORMAL_LST_LINE);
      }
    }

    state.lst.src->line_number++;
  }
}

/*
 * lst_init - initialize the lst file
 */

static void
lst_init(void)
{

  if (state.lstfile != named)
  {
    snprintf(state.lstfilename, sizeof(state.lstfilename),
             "%s.lst", state.basefilename);
  }

  if ((gp_num_errors) || (state.lstfile == suppress))
  {
    state.lst.f = NULL;
    state.lst.enabled = false;
    unlink(state.lstfilename);
  }
  else
  {
    state.lst.f = fopen(state.lstfilename, "wt");
    //state.lst.f = stdout;
    if (state.lst.f == NULL)
    {
      perror(state.lstfilename);
      exit(1);
    }
    state.lst.enabled = true;
  }

  if (!state.lst.enabled)
    return;

  state.lst.was_org = 0;
  state.cod.emitting = 0;

  lst_line("%s", GPLINK_VERSION_STRING);
  //lst_line("%s", GPUTILS_COPYRIGHT_STRING);
  lst_line("Listing File Generated: %s", state.startdate);
  lst_line(" ");
  lst_line(" ");
  lst_line("Address  Value    Disassembly              Source");
  lst_line("-------  -----    -----------              ------");
}

void write_lst(void)
{
  gp_symbol_type *symbol = state.object->symbols;
  gp_aux_type *aux;
  gp_boolean first_time = true;

  lst_init();

  if (!state.lst.enabled)
    return;

  list_enabled = true;
  state.lst.src = NULL;

  /* scan through the file symbols */
  while (symbol != NULL)
  {
    //print_all_lineno(symbol);
    if (symbol->class == C_FILE)
    {
      /* open a new file */
      aux = symbol->aux_list;
      assert(aux != NULL);
      if (aux->_aux_symbol._aux_file.line_number)
      {
        /* it is an include file, so print the current file
           until the line number is reached */
        assert(state.lst.src != NULL);
        write_src(aux->_aux_symbol._aux_file.line_number);
      }
      else
      {
        /* it is not an include, so enable listing */
        list_enabled = true;
      }
      open_src(aux->_aux_symbol._aux_file.filename, symbol);
      if (first_time)
      {
        /* write the line numbers for the lst file header */
        cod_lst_line(COD_FIRST_LST_LINE);
        cod_lst_line(COD_NORMAL_LST_LINE);
        cod_lst_line(COD_NORMAL_LST_LINE);
        cod_lst_line(COD_NORMAL_LST_LINE);
        cod_lst_line(COD_NORMAL_LST_LINE);
        cod_lst_line(COD_NORMAL_LST_LINE);
        cod_lst_line(COD_NORMAL_LST_LINE);
        first_time = false;
      }
    }
    else if (symbol->class == C_EOF)
    {
      /* print the rest of the current file then, close it */
      write_src(0);
      write_macro_dup_line(); // zwr: check macro duplicate lineno and add to cod file
      // print_all_lineno(state.lst.src->symbol);
      close_src();
    }
    else if (symbol->class == C_LIST)
    {
      if (strcasecmp(symbol->name, ".list") == 0)
      {
        write_src(symbol->value);
        list_enabled = true;
      }
      else if (strcasecmp(symbol->name, ".nolist") == 0)
      {
        write_src(symbol->value);
        list_enabled = false;
      }
      else
      {
        assert(0);
      }
    }

    symbol = symbol->next;
  }

  lst_memory_map(state.i_memory);

  fclose(state.lst.f);
  printf("(FF) %s\n", state.lstfilename);
}
