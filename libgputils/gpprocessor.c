/* GNU PIC processor definitions
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

/* XXXPRO: Need to add a line here for any extra processors.  Please
   keep this list sorted primarily by number, secondarily sorting
   alphabetically. */

static struct px pics[] = {
/*  { PROC_CLASS_SN14, "__30P01A", {"mc30p01a", "30p01a", "m301a"}, 0x3010, 1, 1, 0x03ff, {-1, -1}, {0x2000, 0x2001}, 0x3010, "30p01a.lkr"},*/
/*  { PROC_CLASS_SN14, "__30P01B", {"mc30p01b", "30p01b", "m301b"}, 0x3011, 1, 1, 0x03ff, {-1, -1}, {0x2000, 0x2001}, 0x3011, "30p01a.lkr"},*/
  { PROC_CLASS_SN14, "__30P011", {"mc30p011", "0311", "m3011"}, 0x3012, 1, 1, 0x03ff, {-1, -1}, {0x2000, 0x2001}, 0x3012, "0311.lkr"},
  { PROC_CLASS_SN14, "__30P081", {"mc30p081", "30p081", "m3081"}, 0x3018, 1, 1, 0x03ff, {-1, -1}, {0x2000, 0x2001}, 0x3018, "mc30p081.lkr"},
  { PROC_CLASS_SN13, "__31P11", {"mc31p11", "3111", "m3111"}, 0x3111, 1, 1, 0x03ff, {-1, -1}, {0x8000, 0x8001}, 0x3111, "3111.lkr"},
  { PROC_CLASS_SN13B, "__34P01", {"mc34p01", "3401", "m3401"}, 0x3401, 1, 1, 0x03ff, {-1, -1}, {0x8000, 0x8001}, 0x3401, "3401.lkr"},
  { PROC_CLASS_SN13BB, "__34P01B", {"mc34p01b", "34p01b", "m3402"}, 0x3402, 1, 1, 0x03ff, {-1, -1}, {0x8000, 0x8001}, 0x3402, "mc34p01b.lkr"},
  { PROC_CLASS_SN13C, "__31P5130", {"mc31p5130", "31p5310", "m5130"}, 0x5130, 1, 1, 0x05ff, {-1, -1}, {0x8000, 0x8001}, 0x5130, "mc31p5130.lkr"},
/*  { PROC_CLASS_SN16, "__30P44",  {"mc30p44", "30p44", "m344"},    0x3440, 1, 1, 0x0fff, {-1, -1}, {0x2000, 0x2001}, 0x3440, "30p44.lkr" },*/
  { PROC_CLASS_SN16, "__33P94",  {"mc33p94", "3394", "m394"},    0x3940, 1, 1, 0x1fff, {-1, -1}, {0x8000, 0x8001}, 0x3940, "3394.lkr" },
/*  { PROC_CLASS_SN16, "__33P716", {"mc33p716", "33p716", "m3716"}, 0x3760, 1, 1, 0x1fff, {-1, -1}, {0x2000, 0x2001}, 0x3760, "33p716.lkr"},*/
  { PROC_CLASS_SN16A,"__32P21",  {"mc32p21", "3221", "m3221"},   0x3221, 1, 1, 0x03ff, {-1, -1}, {0x8000, 0x8001}, 0x3221, "3221.lkr" },
  { PROC_CLASS_SN16A,"__32P64",  {"mc32p64", "3264", "m3264"},   0x3264, 1, 1, 0x0fff, {-1, -1}, {0x8000, 0x8001}, 0x3264, "3264.lkr" },
  { PROC_CLASS_SN16A,"__32P821",  {"mc32p821", "32p821", "m3281"},   0x3281, 1, 1, 0x03ff, {-1, -1}, {0x8000, 0x8001}, 0x3281, "mc32p821.lkr" },
  { PROC_CLASS_SN16A,"__33P78",  {"mc33p78", "3378", "m3378"},   0x3378, 1, 1, 0x1fff, {-1, -1}, {0x8000, 0x8001}, 0x3378, "3378.lkr" },
  { PROC_CLASS_SN16A,"__32P5222",  {"mc32p5222", "5222", "m5222"},   0x5222, 1, 1, 0x0fff, {-1, -1}, {0x8000, 0x8001}, 0x5222, "3378.lkr" },
  { PROC_CLASS_SN16B,"__33P116",  {"mc33p116", "3316", "m3316"},   0x3316, 1, 1, 0x3fff, {-1, -1}, {0x8000, 0x8001}, 0x3316, "3316.lkr" },
  { PROC_CLASS_SN16B,"__33P5312",  {"mc32p5312", "5312", "m5312"},   0x5312, 1, 1, 0x1ff0, {-1, -1}, {0x8000, 0x8001}, 0x5312, "3116.lkr" },
  { PROC_CLASS_SN16A,"__32P7022",  {"mc32p7022", "7022", "m7022"},   0x7022, 1, 1, 0x0fdf, {-1, -1}, {0x8000, 0x8001}, 0x7022, "7022.lkr" },
  { PROC_CLASS_SN16C,"__32P7011",  {"mc32p7011", "7011", "m7011"},   0x7011, 1, 1, 0x07ff, {-1, -1}, {0x8000, 0x8001}, 0x7011, "mc32p7011.lkr" },
  { PROC_CLASS_SN16C,"__32P7030",  {"mc32p7030", "7030", "m7030"},   0x7030, 1, 1, 0x03ff, {-1, -1}, {0x8000, 0x8001}, 0x7030, "7030.lkr" },
  { PROC_CLASS_SN16D,"__2K7041",  {"2Kmc32p7041", "2K7041", "2Km7041"},   0x7041, 1, 1, 0x07ff, {-1, -1}, {0x8000, 0x8001}, 0x7041, "2K7041.lkr" },
  { PROC_CLASS_SN16D,"__1K7041",  {"1Kmc32p7041", "1K7041", "1Km7041"},   0x7041, 1, 1, 0x03ff, {-1, -1}, {0x8000, 0x8001}, 0x7041, "1K7041.lkr" },
  { PROC_CLASS_SN16A,"__32T8132",  {"mc32t8132", "8132", "m8132"},   0x8132, 1, 1, 0x0fff, {-1, -1}, {0x8000, 0x8001}, 0x8132, "3221.lkr" },
  { PROC_CLASS_SN14, "__30P6060", {"mc30p6060", "6060", "m6060"}, 0x6060, 1, 1, 0x03ff, {-1, -1}, {0x2000, 0x2001}, 0x6060, "6060.lkr"},
  { PROC_CLASS_SN14, "__1K6220", {"1Kmc30p6220", "1K6220", "1Km6220"}, 0x6220, 1, 1, 0x03ff, {-1, -1}, {0x8000, 0x8001}, 0x6220, "1K6220.lkr"},
  { PROC_CLASS_SN14, "__05K6220", {"05Kmc30p6220", "05K6220", "05Km6220"}, 0x6220, 1, 1, 0x01ff, {-1, -1}, {0x8000, 0x8001}, 0x6220, "05K6220.lkr"},
  { PROC_CLASS_SN16A,"__32P7510",  {"mc32p7510", "7510", "m7510"},   0x7510, 1, 1, 0x03ff, {-1, -1}, {0x8000, 0x8001}, 0x7510, "3221.lkr" },
  { PROC_CLASS_SN16A,"__32P7511",  {"mc32p7511", "7511", "m7511"},   0x7511, 1, 1, 0x07ff, {-1, -1}, {0x8000, 0x8001}, 0x7511, "3221.lkr" },
  { PROC_CLASS_SN16A,"__32P7311",  {"mc32p7311", "7311", "m7311"},   0x7311, 1, 1, 0x07ff, {-1, -1}, {0x8000, 0x8001}, 0x7311, "3221.lkr" },
  { PROC_CLASS_SN16A,"__7212",  {"mc32p7212", "7212", "m7212"},   0x7212, 1, 1, 0x0ff7, {-1, -1}, {0x8000, 0x8001}, 0x7212, "7212.lkr" },
  { PROC_CLASS_SN16E,"__9902",  {"mc9902", "9902", "m9902"},   0x9902, 1, 1, 0x07eb, {-1, -1}, {0x0fec, 0x0fed}, 0x9902, "9902.lkr" },	
  { PROC_CLASS_SN16E,"__7333",  {"mc32t7333", "7333", "m7333"},   0x7333, 1, 1, 0x1fff, {-1, -1}, {0x8000, 0x8001}, 0x7333, "7333.lkr" },
  { PROC_CLASS_SN16E,"__9903",  {"mc9903", "9903", "m9903"},   0x9903, 1, 1, 0xfff, {-1, -1}, {0x8000, 0x8001}, 0x9903, "9903.lkr" },  
  { PROC_CLASS_SN16E,"__9904",  {"mc9904", "9904", "m9904"},   0x9904, 1, 1, 0x1fff, {-1, -1}, {0x8000, 0x8001}, 0x9904, "9904.lkr" },
  { PROC_CLASS_SN16E,"__7122",  {"mc32f7122", "7122", "m7122"},   0x7122, 1, 1, 0x0fff, {-1, -1}, {0x8000, 0x8001}, 0x7122, "7122.lkr" },
  { PROC_CLASS_SN16A,"__8K7323",  {"8Kmc32p7323", "8K7323", "8Km7323"},   0x7323, 1, 1, 0x1fff, {-1, -1}, {0x8000, 0x8001}, 0x7323, "8K7323.lkr" },
  { PROC_CLASS_SN16A,"__4K7323",  {"4Kmc32p7323", "4K7323", "4Km7323"},   0x7323, 1, 1, 0x0fff, {-1, -1}, {0x8000, 0x8001}, 0x7323, "4K7323.lkr" }
};

#define NUM_PICS        (sizeof(pics) / sizeof(pics[0]))

/*
 * Display a list of the processor names
 */
void gp_dump_processor_list(gp_boolean list_all, proc_class_t class)
{
#define COLUMNS  6
#define SPACE_BETWEEN 2   /* number of chars between columns */
#define FAVORITE 1        /* there are 3 names to choose from */

  int i;
  int j;
  int length;
  int num = 0;
  int longest = 0;

  for(i = 0; i < NUM_PICS; i++) {
    length = strlen(pics[i].names[FAVORITE]);
    if (length > longest)
      longest = length;
  }

  for(i = 0; i < NUM_PICS; i++) {
    if (list_all || (pics[i].class == class)) {
      num++;
      printf("%s", pics[i].names[FAVORITE]);
      length = longest + SPACE_BETWEEN - strlen(pics[i].names[FAVORITE]);
      for(j = 0; j < length; j++) {
        putchar(' ');
      }
      if ((num % COLUMNS) == 0) {
        putchar('\n');
      }
    }
  }

  if ((num % COLUMNS) != 0) {
    putchar('\n');
  }

}

const struct px *
gp_find_processor(const char *name)
{
  int i, j;

  for (i = 0; i < NUM_PICS; i++) {
    for (j = 0; (j < MAX_NAMES) && (pics[i].names[j] != NULL); j++) {
      if (strcasecmp(name, pics[i].names[j]) == 0) {
        return &pics[i];
      }
    }
  }

  return NULL;
}

proc_class_t
gp_processor_class(pic_processor_t processor)
{
  if (processor)
    return processor->class;
  return PROC_CLASS_UNKNOWN;
}

/* 18xx bsr boundary location */

int
gp_processor_bsr_boundary(pic_processor_t processor)
{
  if (processor && processor->class == PROC_CLASS_PIC16E)
    return processor->num_banks;
  return 0;
}

unsigned long
gp_processor_coff_type(pic_processor_t processor)
{
  if (processor)
    return processor->coff_type;
  return 0;
}

int
gp_processor_num_pages(pic_processor_t processor)
{
  if (processor)
    return processor->num_pages;
  return 0;
}

int
gp_processor_num_banks(pic_processor_t processor)
{
  if (processor && processor->class != PROC_CLASS_PIC16E)
    return processor->num_banks;
  return 0;
}

pic_processor_t
gp_processor_coff_proc(unsigned long coff_type)
{
  int i;
  pic_processor_t processor = no_processor;

  for (i = 0; i < NUM_PICS; i++) {
    if (pics[i].coff_type == coff_type) {
      processor = &pics[i];
      break;
    }
  }

  return processor;
}

const char *
gp_processor_name(pic_processor_t processor, unsigned int choice)
{
  assert(!(choice > MAX_NAMES - 1));

  if (processor)
    return processor->names[choice];

  return NULL;
}

const char *
gp_processor_coff_name(unsigned long coff_type, unsigned int choice)
{
  int i;

  if (coff_type == 0)
    return NULL;

  assert(!(choice > MAX_NAMES - 1));

  for (i = 0; i < NUM_PICS; i++) {
    if (pics[i].coff_type == coff_type) {
      return pics[i].names[choice];
    }
  }

  return NULL;
}

const char *
gp_processor_script(pic_processor_t processor)
{
  if (processor)
    return processor->script;
  return NULL;
}

unsigned int
gp_processor_id_location(pic_processor_t processor)
{
  if (processor->class->id_location)
    return processor->class->id_location(processor);
  return 0;
}

int
gp_processor_rom_width(proc_class_t class)
{
  assert(class->rom_width > 0);
  return class->rom_width;
}

/* Set the page bits, return the number of instructions required. */

int
gp_processor_set_page(proc_class_t class,
                      int num_pages,
                      int page,
                      MemBlock *m,
                      unsigned int address,
                      int use_wreg)
{
  return class->set_page(num_pages, page, m, address, use_wreg);
}

/* Set the bank bits, return the number of instructions required. */

int
gp_processor_set_bank(proc_class_t class,
                      int num_banks,
                      int bank,
                      MemBlock *m,
                      unsigned int address)
{
  return class->set_bank(num_banks, bank, m, address);
}

/* determine the value for retlw */

int
gp_processor_retlw(proc_class_t class)
{
  assert(class->retlw >= 0);
  return class->retlw;
}

int
gp_processor_org_to_byte(proc_class_t class, int org)
{
  /* FIXME: In some places we use this value before we know what the
     processor is. Rather than fix those now, I'll just return some
     value. */
  if (!class)
    return org;

  return org << class->org_to_byte_shift;
}

int
gp_processor_byte_to_org(proc_class_t class, int byte)
{
  /* FIXME: In some places we use this value before we know what the
     processor is. Rather than fix those now, I'll just return some
     value. */
  if (!class)
    return byte;

  return byte >> class->org_to_byte_shift;
}

int
gp_org_to_byte(unsigned shift, int org)
{
  return org << shift;
}

int
gp_byte_to_org(unsigned shift, int byte)
{
  return byte >> shift;
}

/* determine which page of program memory the byte address is located */
int
gp_processor_check_page(proc_class_t class, unsigned int org)
{
  return class->check_page(org);
}

/* determine which bank of data memory the address is located */
int
gp_processor_check_bank(proc_class_t class, unsigned int address)
{
  return class->check_bank(address);
}

/* When unsupported on the class */
static int
gp_processor_check_page_unsupported(unsigned int org)
{
  assert(0);
  return 0;
}

static int
gp_processor_check_bank_unsupported(unsigned int address)
{
  assert(0);
  return 0;
}

static int
gp_processor_set_page_unsupported(int num_pages,
                                  int page,
                                  MemBlock *m,
                                  unsigned int address,
                                  int use_wreg)
{
    assert(0);
    return 0;
}

static int
gp_processor_set_bank_unsupported(int num_banks,
                                  int bank,
                                  MemBlock *m,
                                  unsigned int address)
{
  assert(0);
  return 0;
}

static int reloc_unsupported(unsigned int address)
{
  assert(0);
  return 0;
}

/* Common to most */

static const struct insn *
find_insn_generic(proc_class_t cls, long int opcode)
{
  const struct insn *base = cls->instructions;
  int count = base == NULL ? 0 : *cls->num_instructions;
  int i;
  for(i = 0; i < count; i++) {
    if((base[i].mask & opcode) == base[i].opcode) {
      return &base[i];
    }
  }
  return NULL;
}

/* Common to PIC12 and PIC14 */

static int
gp_processor_set_page_pic12_14(int num_pages,
                               int page,
                               MemBlock *m,
                               unsigned int address,
                               int use_wreg,
                               int bcf_insn,
                               int bsf_insn,
                               int movlw_insn,
                               int movwf_insn,
                               int location,
                               int page0,
                               int page1)
{
  unsigned int data;
  int count = 4;
  if (num_pages == 1) {
    return 0;
  }

  if (use_wreg) {
    data = movlw_insn | page;
    i_memory_put_le(m, address, data);
    data = movwf_insn | location;
    i_memory_put_le(m, address + 2, data);
  }
  else {
    /* page low bit */
    data = (page & 1 ? bsf_insn : bcf_insn) | page0 | location;
    i_memory_put_le(m, address, data);

    /* page high bit */
    if (num_pages == 4) {
      data = (page & 2 ? bsf_insn : bcf_insn) | page1 | location;
      i_memory_put_le(m, address + 2, data);
    }

    if (num_pages == 2) {
      count = 2;
    } else {
      count = 4;
    }
  }
  return count;
}

static int
gp_processor_set_bank_pic12_14(int num_banks,
                               int bank,
                               MemBlock *m,
                               unsigned int address,
                               int bcf_insn,
                               int bsf_insn,
                               int location,
                               int bank0,
                               int bank1)
{
  unsigned int data;
  int count;
  if (num_banks == 1) {
    return 0;
  }

  switch(bank) {
  case 0:
    /* bcf location, bank0 */
    data = bcf_insn | bank0 | location;
    i_memory_put_le(m, address, data);
    if (num_banks == 4) {
      /* bcf location, bank1 */
      data = bcf_insn | bank1 | location;
      i_memory_put_le(m, address + 2, data);
    }
    break;
  case 1:
    /* bsf location, bank0 */
    data = bsf_insn | bank0 | location;
    i_memory_put_le(m, address, data);
    if (num_banks == 4) {
      /* bcf location, bank1 */
      data = bcf_insn | bank1 | location;
      i_memory_put_le(m, address + 2, data);
    }
    break;
  case 2:
    /* bcf location, bank0 */
    data = bcf_insn | bank0 | location;
    i_memory_put_le(m, address, data);
    if (num_banks == 4) {
      /* bsf location, bank1 */
      data = bsf_insn | bank1 | location;
      i_memory_put_le(m, address + 2, data);
    }
    break;
  case 3:
    /* bsf location, bank0 */
    data = bsf_insn | bank0 | location;
    i_memory_put_le(m, address, data);
    if (num_banks == 4) {
      /* bsf location, bank1 */
      data = bsf_insn | bank1 | location;
      i_memory_put_le(m, address + 2, data);
    }
    break;
  default:
    assert(0);
    break;
  }

  if (num_banks == 2) {
    count = 2;
  } else {
    count = 4;
  }

  return count;
}

/* PIC12 */

static unsigned int
id_location_pic12(const struct px *processor)
{
  if (processor->maxrom > 0) {
    /* Sometimes the last hex digit is e due to some calibration
       instruction or something. Therefore we look for the next
       multiple of 16. */
    /* We carry org in the struct px, but return byte address. */
    return ((processor->maxrom + 0xf) & ~0xf) << 1;
  }
  return 0;
}

static int
gp_processor_check_page_pic12(unsigned int org)
{
  return (org >> 9) & 0x3;
}

static int
gp_processor_check_bank_pic12(unsigned int address)
{
  return (address >> 5) & 0x3;
}

static int
gp_processor_set_page_pic12(int num_pages,
                            int page,
                            MemBlock *m,
                            unsigned int address,
                            int use_wreg)

{
  return gp_processor_set_page_pic12_14(num_pages,
                                        page,
                                        m,
                                        address,
                                        use_wreg,
                                        0x400,
                                        0x500,
                                        0xc00,
                                        0x020,
                                        0x3,
                                        5 << 5,
                                        6 << 5);
}

static int
gp_processor_set_bank_pic12(int num_banks,
                            int bank,
                            MemBlock *m,
                            unsigned int address)
{
  return gp_processor_set_bank_pic12_14(num_banks, bank, m, address,
                                        0x400,
                                        0x500,
                                        0x4,
                                        5 << 5,
                                        6 << 5);
}

static int reloc_call_pic12(unsigned int org)
{
  return org & 0xff;
}
static int reloc_goto_pic12(unsigned int org)
{
  return org & 0x1ff;
}
static int reloc_f_pic12(unsigned int address)
{
  return address & 0x1f;
}
static int reloc_tris_pic12(unsigned int address)
{
  /* TODO This is not accurate, for example PIC12F510/16F506 only has
     three bits and allowed values of 6 and 7. MPASM 5.34 has
     Error[126]  : Argument out of range (0000 not between 0005 and 0009)
  */
  return address & 0x1f;
}

/* PIC14 */

static unsigned int
id_location_pic14(const struct px *processor)
{
  if (processor->config_addrs[0] > 0)
    /* I only know of 0x2007 to 0x2000 and 0x8007 to 0x8000 and this
       returns correct for both. */
    /* We carry org in the struct px, but return byte address. */
    return (processor->config_addrs[0] & ~0xFFF) << 1;
  return 0;
}

static int
gp_processor_check_page_pic14(unsigned int org)
{
  return (org >> 11) & 0x3;
}

static int
gp_processor_check_bank_pic14(unsigned int address)
{
  return (address >> 7) & 0x3;
}

static int
gp_processor_set_page_pic14(int num_pages,
                            int page,
                            MemBlock *m,
                            unsigned int address,
                            int use_wreg)
{
  return gp_processor_set_page_pic12_14(num_pages,
                                        page,
                                        m,
                                        address,
                                        use_wreg,
                                        0x1000,
                                        0x1400,
                                        0x3000,
                                        0x0080,
                                        0xa,
                                        3 << 7,
                                        4 << 7);
}

static int
gp_processor_set_bank_pic14(int num_banks,
                            int bank,
                            MemBlock *m,
                            unsigned int address)
{
  return gp_processor_set_bank_pic12_14(num_banks, bank, m, address,
                                        0x1000,
                                        0x1400,
                                        0x3,
                                        5 << 7,
                                        6 << 7);
}

static int reloc_call_pic14(unsigned int org)
{
  return org & 0x7ff;
}
static int reloc_goto_pic14(unsigned int org)
{
  return org & 0x7ff;
}
static int reloc_ibanksel_pic14(unsigned int address)
{
  if (address < 0x100) {
    /* bcf 0x3, 0x7 */
    return 0x1383;
  } else {
    /* bsf 0x3, 0x7 */
    return 0x1783;
  }
}
static int reloc_f_pic14(unsigned int address)
{
  return address & 0x7f;
}
static int reloc_tris_pic14(unsigned int address)
{
  return address & 0x7f;
}

static void patch_strict_pic14(void)
{
  int i, j = 0;
  for (i = 0; i < num_op_16cxx && j < num_op_16cxx_strict_mask; ++i) {
    if (!strcasecmp(op_16cxx[i].name, op_16cxx_strict_mask[j].name)) {
      op_16cxx[i].mask = op_16cxx_strict_mask[j].mask;
      ++j;
    }
  }
}

/* PIC14E */

static int
gp_processor_check_page_pic14e(unsigned int org)
{
  return (org >> 8) & 0x7f;
}

static int
gp_processor_check_bank_pic14e(unsigned int address)
{
  return (address >> 7) & 0x1f;
}

static int
gp_processor_set_page_pic14e(int num_pages,
                             int page,
                             MemBlock *m,
                             unsigned int address,
                             int use_wreg)
{
  unsigned int data;
  if (num_pages == 1) {
    return 0;
  }

  if (use_wreg) {
    /* page is in bits 3:6 of PCLATH */
    data = 0x3000 | (page & 0x74); /* movlw */
    i_memory_put_le(m, address, data);
    data = 0x0080 | 0xa; /* movwf 0xa*/
    i_memory_put_le(m, address + 2, data);
    return 4;
  }
  else {
    data = 0x3180 | (page & 0x7f); /* movlp */
    i_memory_put_le(m, address, data);
    return 2;
  }
}

static int
gp_processor_set_bank_pic14e(int num_banks,
                             int bank,
                             MemBlock *m,
                             unsigned int address)
{
  unsigned int data;
  data = 0x0020 | (bank & 0x1f);
  i_memory_put_le(m, address, data);
  return 2;
}

static const struct insn *
find_insn_pic14e(proc_class_t cls, long int opcode)
{
  int i;
  /* might be from the enganced instruction set */
  for(i = 0; i < num_op_16cxx_enh; i++) {
    if((op_16cxx_enh[i].mask & opcode) == op_16cxx_enh[i].opcode) {
      return &op_16cxx_enh[i];
    }
  }
  for(i = 0; i < num_op_16cxx; i++) {
    if((op_16cxx[i].mask & opcode) == op_16cxx[i].opcode) {
      return &op_16cxx[i];
    }
  }
  return NULL;
}

/* PIC16 */

static int
gp_processor_check_page_pic16(unsigned int org)
{
  return (org >> 8) & 0xff;
}


static int
gp_processor_check_bank_pic16(unsigned int address)
{
  if ((address & 0xFF) < 0x20)
    return (address >> 8) & 0xff;
  else
    /* 0x200 turns MOVLB to MOVLR for setting GPR RAM bank in
       set_bank */
    return 0x200 + ((address >> 8) & 0xff);
}


static int
gp_processor_set_page_pic16(int num_pages,
                            int page,
                            MemBlock *m,
                            unsigned int address,
                            int use_wreg)
{
  unsigned int data;
  /* movlw <page> */
  data = 0xb000 | (page & 0xff);
  i_memory_put_le(m, address, data);
  /* movwf 0x3 */
  data = 0x0100 | 0x3;
  i_memory_put_le(m, address + 2, data);
  return 4;
}

static int
gp_processor_set_bank_pic16(int num_banks,
                            int bank,
                            MemBlock *m,
                            unsigned int address)
{
  /* movlb bank */
  i_memory_put_le(m, address,
                  0xb800 | bank);
  return 2;
}

static int reloc_call_pic16(unsigned int org)
{
  return org & 0x1fff;
}
static int reloc_goto_pic16(unsigned int org)
{
  return org & 0xffff;
}
static int reloc_ibanksel_pic16(unsigned int address)
{
  return 0xb800 | gp_processor_check_bank_pic16(address);
}
static int reloc_f_pic16(unsigned int address)
{
  return address & 0xfff; //zwr
}

/* PIC16E */

static unsigned int
id_location_pic16e(const struct px *processor)
{
  return IDLOC0;
}

static int
gp_processor_set_bank_pic16e(int num_banks,
                             int bank,
                             MemBlock *m,
                             unsigned int address)
{
  /* movlb bank */
  i_memory_put_le(m, address,
                  0x0100 | (bank & 0xff));
  return 2;
}

static int reloc_call_pic16e(unsigned int org)
{
  return (org >> 1) & 0xff;
}
static int reloc_goto_pic16e(unsigned int org)
{
  return (org >> 1) & 0xff;
}

static const struct insn *
find_insn_pic16e(proc_class_t cls, long int opcode)
{
  int i;
  if (gp_decode_mnemonics) {
    for(i = 0; i < num_op_18cxx_sp; i++) {
      if((op_18cxx_sp[i].mask & opcode) == op_18cxx_sp[i].opcode) {
        return &op_18cxx_sp[i];
      }
    }
  }
  for(i = 0; i < num_op_18cxx; i++) {
    if((op_18cxx[i].mask & opcode) == op_18cxx[i].opcode) {
      return &op_18cxx[i];
    }
  }
  if (gp_decode_extended) {
    /* might be from the extended instruction set */
    for(i = 0; i < num_op_18cxx_ext; i++) {
      if((op_18cxx_ext[i].mask & opcode) == op_18cxx_ext[i].opcode) {
        return &op_18cxx_ext[i];
      }
    }
  }
  return NULL;
}

const struct proc_class proc_class_eeprom8 = {
  -1,                                   /* retlw */
  8,                                    /* rom_width */
  0,                                    /* org_to_byte_shift */
  0,                                    /* bank_mask */
  (1<<8)-1,                             /* core_size */
  0,                                    /* id_location */
  gp_processor_check_bank_unsupported,
  gp_processor_set_bank_unsupported,
  gp_processor_check_page_unsupported,
  gp_processor_set_page_unsupported,
  reloc_unsupported,
  reloc_unsupported,
  reloc_unsupported,
  reloc_unsupported,
  reloc_unsupported,
  NULL, NULL,
  NULL,
  i_memory_get_le, i_memory_put_le,
  NULL,
};

const struct proc_class proc_class_eeprom16 = {
  -1,                                   /* retlw */
  16,                                   /* rom_width */
  0,                                    /* org_to_byte_shift */
  0,                                    /* bank_mask */
  (1<<16)-1,                            /* core_size */
  0,                                    /* id_location */
  gp_processor_check_bank_unsupported,
  gp_processor_set_bank_unsupported,
  gp_processor_check_page_unsupported,
  gp_processor_set_page_unsupported,
  reloc_unsupported,
  reloc_unsupported,
  reloc_unsupported,
  reloc_unsupported,
  reloc_unsupported,
  NULL, NULL,
  NULL,
  i_memory_get_be, i_memory_put_be,
  NULL,
};

const struct proc_class proc_class_generic = {
  -1,                                   /* retlw */
  12,                                   /* rom_width */
  1,                                    /* org_to_byte_shift */
  ~0x1fu,                               /* bank_mask */
  (1<<12)-1,                            /* core_size */
  id_location_pic12,                    /* id_location */
  gp_processor_check_bank_pic12,
  gp_processor_set_bank_pic12,
  gp_processor_check_page_pic12,
  gp_processor_set_page_pic12,
  reloc_unsupported,
  reloc_unsupported,
  reloc_unsupported,
  reloc_unsupported,
  reloc_unsupported,
  NULL, NULL,
  NULL,
  i_memory_get_le, i_memory_put_le,
  NULL,
};

const struct proc_class proc_class_pic12 = {
  0x800,                                /* retlw */
  12,                                   /* rom_width */
  1,                                    /* org_to_byte_shift */
  ~0x1fu,                               /* bank_mask */
  (1<<12)-1,                            /* core_size */
  id_location_pic12,                    /* id_location */
  gp_processor_check_bank_pic12,
  gp_processor_set_bank_pic12,
  gp_processor_check_page_pic12,
  gp_processor_set_page_pic12,
  reloc_call_pic12,
  reloc_goto_pic12,
  reloc_unsupported,
  reloc_f_pic12,
  reloc_tris_pic12,
  op_12c5xx, &num_op_12c5xx,
  find_insn_generic,
  i_memory_get_le, i_memory_put_le,
  NULL,
};

const struct proc_class proc_class_sx = {
  0x800,                                /* retlw */
  12,                                   /* rom_width */
  1,                                    /* org_to_byte_shift */
  ~0x1fu,                               /* bank_mask */
  (1<<12)-1,                            /* core_size */
  id_location_pic12,                    /* id_location */
  gp_processor_check_bank_pic12,
  gp_processor_set_bank_pic12,
  gp_processor_check_page_pic12,
  gp_processor_set_page_pic12,
  reloc_call_pic12,
  reloc_goto_pic12,
  reloc_unsupported,
  reloc_f_pic12,
  reloc_tris_pic12,
  op_sx, &num_op_sx,
  find_insn_generic,
  i_memory_get_le, i_memory_put_le,
  NULL,
};

const struct proc_class proc_class_pic14 = {
  0x3400,                               /* retlw */
  14,                                   /* rom_width */
  1,                                    /* org_to_byte_shift */
  ~0x7fu,                               /* bank_mask */
  (1<<14)-1,                            /* core_size */
  id_location_pic14,                    /* id_location */
  gp_processor_check_bank_pic14,
  gp_processor_set_bank_pic14,
  gp_processor_check_page_pic14,
  gp_processor_set_page_pic14,
  reloc_call_pic14,
  reloc_goto_pic14,
  reloc_ibanksel_pic14,
  reloc_f_pic14,
  reloc_tris_pic14,
  op_16cxx, &num_op_16cxx,
  find_insn_generic,
  i_memory_get_le, i_memory_put_le,
  patch_strict_pic14,
};

const struct proc_class proc_class_pic14e = {
  0x3400,                               /* retlw */
  14,                                   /* rom_width */
  1,                                    /* org_to_byte_shift */
  0,                                    /* bank_mask */
  (1<<14)-1,                            /* core_size */
  id_location_pic14,                    /* id_location */
  gp_processor_check_bank_pic14e,
  gp_processor_set_bank_pic14e,
  gp_processor_check_page_pic14e,
  gp_processor_set_page_pic14e,
  reloc_call_pic14,
  reloc_goto_pic14,
  reloc_ibanksel_pic14,
  reloc_f_pic14,
  reloc_tris_pic14,
  op_16cxx, &num_op_16cxx,
  find_insn_pic14e,
  i_memory_get_le, i_memory_put_le,
  patch_strict_pic14,
};

const struct proc_class proc_class_pic16 = {
  0xb600,                               /* retlw */
  16,                                   /* rom_width */
  1,                                    /* org_to_byte_shift */
  ~0xffu,                               /* bank_mask */
  (1<<16)-1,                            /* core_size */
  0,                                    /* id_location */
  gp_processor_check_bank_pic16,
  gp_processor_set_bank_pic16,
  gp_processor_check_page_pic16,
  gp_processor_set_page_pic16,
  reloc_call_pic16,
  reloc_goto_pic16,
  reloc_ibanksel_pic16,
  reloc_f_pic16,
  reloc_unsupported,
  op_17cxx, &num_op_17cxx,
  find_insn_generic,
  i_memory_get_le, i_memory_put_le,
  NULL,
};

const struct proc_class proc_class_pic16e = {
  0x0c00,                               /* retlw */
  8,                                    /* rom_width */
  0,                                    /* org_to_byte_shift */
  0,                                    /* bank_mask */
  (1<<16)-1,                            /* core_size */
  id_location_pic16e,                   /* id_location */
  gp_processor_check_bank_pic16,        /* Same as for pic16 */
  gp_processor_set_bank_pic16e,
  gp_processor_check_page_unsupported,
  gp_processor_set_page_unsupported,
  reloc_call_pic16e,
  reloc_goto_pic16e,
  reloc_unsupported,
  reloc_f_pic16,        /* Same as for pic16 */
  reloc_unsupported,
  op_18cxx, &num_op_18cxx,
  find_insn_pic16e,
  i_memory_get_le, i_memory_put_le,
  NULL,
};

const struct proc_class proc_class_sn14 = {
  0x0a00,                               /* retai opcode */
  14,                                   /* rom_width */
  1,                                    /* org_to_byte_shift */
  ~0x7fu,                               /* bank_mask */
  (1<<14)-1,                            /* core_size */
  id_location_pic14,                    /* id_location */
  gp_processor_check_bank_pic14,
  gp_processor_set_bank_pic14,
  gp_processor_check_page_pic14,
  gp_processor_set_page_pic14,
  reloc_call_pic14,
  reloc_goto_pic14,
  reloc_ibanksel_pic14,
  reloc_f_pic14,
  reloc_tris_pic14,
  op_sn14, &num_op_sn14,
  find_insn_generic,
  i_memory_get_le, i_memory_put_le,
  patch_strict_pic14,
};

const struct proc_class proc_class_sn16 = {
  0x3b00,                               /* retai opcode*/
  16,                                   /* rom_width */
  1,                                    /* org_to_byte_shift */
  ~0x1ffu,                               /* bank_mask */
  (1<<16)-1,                            /* core_size */
  0,                                    /* id_location */
  gp_processor_check_bank_pic16,
  gp_processor_set_bank_pic16,
  gp_processor_check_page_pic16,
  gp_processor_set_page_pic16,
  reloc_call_pic16,
  reloc_goto_pic16,
  reloc_ibanksel_pic16,
  reloc_f_pic16,
  reloc_unsupported,
  op_sn16, &num_op_sn16,
  find_insn_generic,
  i_memory_get_le, i_memory_put_le,
  NULL,
};

const struct proc_class proc_class_sn16a = {
  0x3b00,                               /* retai opcode*/
  16,                                   /* rom_width */
  1,                                    /* org_to_byte_shift */
  ~0x1ffu,                               /* bank_mask */
  (1<<16)-1,                            /* core_size */
  0,                                    /* id_location */
  gp_processor_check_bank_pic16,
  gp_processor_set_bank_pic16,
  gp_processor_check_page_pic16,
  gp_processor_set_page_pic16,
  reloc_call_pic16,
  reloc_goto_pic16,
  reloc_ibanksel_pic16,
  reloc_f_pic16,
  reloc_unsupported,
  op_sn16a, &num_op_sn16a,
  find_insn_generic,
  i_memory_get_le, i_memory_put_le,
  NULL,
};


const struct proc_class proc_class_sn16b = {
  0x3b00,                               /* retai opcode*/
  16,                                   /* rom_width */
  1,                                    /* org_to_byte_shift */
  ~0x1ffu,                               /* bank_mask */
  (1<<16)-1,                            /* core_size */
  0,                                    /* id_location */
  gp_processor_check_bank_pic16,
  gp_processor_set_bank_pic16,
  gp_processor_check_page_pic16,
  gp_processor_set_page_pic16,
  reloc_call_pic16,
  reloc_goto_pic16,
  reloc_ibanksel_pic16,
  reloc_f_pic16,
  reloc_unsupported,
  op_sn16b, &num_op_sn16b,
  find_insn_generic,
  i_memory_get_le, i_memory_put_le,
  NULL,
};

const struct proc_class proc_class_sn16c = {
  0x3b00,                               /* retai opcode*/
  16,                                   /* rom_width */
  1,                                    /* org_to_byte_shift */
  ~0x1ffu,                               /* bank_mask */
  (1<<16)-1,                            /* core_size */
  0,                                    /* id_location */
  gp_processor_check_bank_pic16,
  gp_processor_set_bank_pic16,
  gp_processor_check_page_pic16,
  gp_processor_set_page_pic16,
  reloc_call_pic16,
  reloc_goto_pic16,
  reloc_ibanksel_pic16,
  reloc_f_pic16,
  reloc_unsupported,
  op_sn16c, &num_op_sn16c,
  find_insn_generic,
  i_memory_get_le, i_memory_put_le,
  NULL,
};


const struct proc_class proc_class_sn16d = {
  0x3b00,                               /* retai opcode*/
  16,                                   /* rom_width */
  1,                                    /* org_to_byte_shift */
  ~0x1ffu,                               /* bank_mask */
  (1<<16)-1,                            /* core_size */
  0,                                    /* id_location */
  gp_processor_check_bank_pic16,
  gp_processor_set_bank_pic16,
  gp_processor_check_page_pic16,
  gp_processor_set_page_pic16,
  reloc_call_pic16,
  reloc_goto_pic16,
  reloc_ibanksel_pic16,
  reloc_f_pic16,
  reloc_unsupported,
  op_sn16d, &num_op_sn16d,
  find_insn_generic,
  i_memory_get_le, i_memory_put_le,
  NULL,
};

const struct proc_class proc_class_sn16e = {
  0x3b00,                               /* retai opcode*/
  16,                                   /* rom_width */
  1,                                    /* org_to_byte_shift */
  ~0x1ffu,                               /* bank_mask */
  (1<<16)-1,                            /* core_size */
  0,                                    /* id_location */
  gp_processor_check_bank_pic16,
  gp_processor_set_bank_pic16,
  gp_processor_check_page_pic16,
  gp_processor_set_page_pic16,
  reloc_call_pic16,
  reloc_goto_pic16,
  reloc_ibanksel_pic16,
  reloc_f_pic16,
  reloc_unsupported,
  op_sn16e, &num_op_sn16e,
  find_insn_generic,
  i_memory_get_le, i_memory_put_le,
  NULL,
};

const struct proc_class proc_class_sn13 = {
  0x0300,                               /* retai opcode */
  13,                                   /* rom_width */
  1,                                    /* org_to_byte_shift */
  ~0x7fu,                               /* bank_mask */
  (1<<14)-1,                            /* core_size */
  id_location_pic14,                    /* id_location */
  gp_processor_check_bank_pic14,
  gp_processor_set_bank_pic14,
  gp_processor_check_page_pic14,
  gp_processor_set_page_pic14,
  reloc_call_pic14,
  reloc_goto_pic14,
  reloc_ibanksel_pic14,
  reloc_f_pic14,
  reloc_tris_pic14,
  op_sn13, &num_op_sn13,
  find_insn_generic,
  i_memory_get_le, i_memory_put_le,
  patch_strict_pic14,
};

const struct proc_class proc_class_sn13b = {
  0x0300,                               /* retai opcode */
  13,                                   /* rom_width */
  1,                                    /* org_to_byte_shift */
  ~0x7fu,                               /* bank_mask */
  (1<<14)-1,                            /* core_size */
  id_location_pic14,                    /* id_location */
  gp_processor_check_bank_pic14,
  gp_processor_set_bank_pic14,
  gp_processor_check_page_pic14,
  gp_processor_set_page_pic14,
  reloc_call_pic14,
  reloc_goto_pic14,
  reloc_ibanksel_pic14,
  reloc_f_pic14,
  reloc_tris_pic14,
  op_sn13b, &num_op_sn13b,
  find_insn_generic,
  i_memory_get_le, i_memory_put_le,
  patch_strict_pic14,
};


const struct proc_class proc_class_sn13bb = {
  0x0300,                               /* retai opcode */
  13,                                   /* rom_width */
  1,                                    /* org_to_byte_shift */
  ~0x7fu,                               /* bank_mask */
  (1<<14)-1,                            /* core_size */
  id_location_pic14,                    /* id_location */
  gp_processor_check_bank_pic14,
  gp_processor_set_bank_pic14,
  gp_processor_check_page_pic14,
  gp_processor_set_page_pic14,
  reloc_call_pic14,
  reloc_goto_pic14,
  reloc_ibanksel_pic14,
  reloc_f_pic14,
  reloc_tris_pic14,
  op_sn13bb, &num_op_sn13bb,
  find_insn_generic,
  i_memory_get_le, i_memory_put_le,
  patch_strict_pic14,
};

const struct proc_class proc_class_sn13c = {
  0x0300,                               /* retai opcode */
  13,                                   /* rom_width */
  1,                                    /* org_to_byte_shift */
  ~0x7fu,                               /* bank_mask */
  (1<<14)-1,                            /* core_size */
  id_location_pic14,                    /* id_location */
  gp_processor_check_bank_pic14,
  gp_processor_set_bank_pic14,
  gp_processor_check_page_pic14,
  gp_processor_set_page_pic14,
  reloc_call_pic14,
  reloc_goto_pic14,
  reloc_ibanksel_pic14,
  reloc_f_pic14,
  reloc_tris_pic14,
  op_sn13c, &num_op_sn13c,
  find_insn_generic,
  i_memory_get_le, i_memory_put_le,
  patch_strict_pic14,
};


