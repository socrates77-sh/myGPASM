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

#ifndef __GPPROCESSOR_H__
#define __GPPROCESSOR_H__

struct px;

struct proc_class {
  /* Instruction used in making initialization data sections */
  int retlw;
  /* Value in COFF header */
  int rom_width;
  /* Bits to shift assembly code address for COFF file byte address */
  unsigned int org_to_byte_shift;
  /* Mask of address bits for bank */
  unsigned int bank_mask;
  /* Bitmask of bits that can be stored in code section address */
  unsigned int core_size;
  /* Get the address for ID location */
  unsigned int (*id_location)(const struct px *processor);

  /* Determine which bank of data memory the address is located */
  int (*check_bank)(unsigned int address);

  /* Set the bank bits, return the number of instructions required. */
  int (*set_bank)(int num_banks,
                  int bank,
                  MemBlock *m,
                  unsigned int address);

  /* Determine which page of program memory the address is located */
  int (*check_page)(unsigned int address);

  /* Set the page bits, return the number of instructions required. */
  int (*set_page)(int num_pages,
                  int page,
                  MemBlock *m,
                  unsigned int address,
                  int use_wreg);

  /* These return the bits to set in instruction for given address */
  int (*reloc_call)(unsigned int address);
  int (*reloc_goto)(unsigned int address);
  int (*reloc_ibanksel)(unsigned int address);
  int (*reloc_f)(unsigned int address);
  int (*reloc_tris)(unsigned int address);

  const struct insn *instructions;
  const int *num_instructions;
  const struct insn *(*find_insn)(const struct proc_class *cls, long int opcode);

  int (*i_memory_get)(MemBlock *m, unsigned int byte_address, unsigned short *word);
  void (*i_memory_put)(MemBlock *m, unsigned int byte_address, unsigned short value);

  void (*patch_strict)(void);
};
typedef const struct proc_class *proc_class_t;

#define PROC_CLASS_UNKNOWN ((proc_class_t)0) /* Unknown device */
extern const struct proc_class proc_class_eeprom8;   /* 8 bit EEPROM */
extern const struct proc_class proc_class_eeprom16;  /* 16 bit EEPROM */
extern const struct proc_class proc_class_generic;   /* 12 bit device */
extern const struct proc_class proc_class_pic12;     /* 12 bit devices */
extern const struct proc_class proc_class_sx;        /* 12 bit devices */
extern const struct proc_class proc_class_pic14;     /* 14 bit devices */
extern const struct proc_class proc_class_pic14e;    /* enhanced 14 bit devices */
extern const struct proc_class proc_class_pic16;     /* 16 bit devices */
extern const struct proc_class proc_class_pic16e;    /* enhanced 16 bit devices */
extern const struct proc_class proc_class_sn14;     /* sinomcu 14 bit devices */
extern const struct proc_class proc_class_sn16;     /* sinomcu 16 bit devices */
extern const struct proc_class proc_class_sn16a;    /* sinomcu 16 bit devices, without MUL */
extern const struct proc_class proc_class_sn16b;    /* sinomcu 16 bit devices, with LCALL/LGOTO */
extern const struct proc_class proc_class_sn16c;    /* Sinomcu 16-bit instruction set with cmpr/cmpi/push/pop/xch/movc */
extern const struct proc_class proc_class_sn16d;    /* add mular compared with sn16c, for 7041 */
extern const struct proc_class proc_class_sn16e;    /* add mular compared with sn16b,with MULAR for A300(MC9902) */
extern const struct proc_class proc_class_sn13;    /* sinomcu 13 bit devices for mc31 */
extern const struct proc_class proc_class_sn13b;    /* sinomcu 13 bit devices for mc34 */
extern const struct proc_class proc_class_sn13bb;    /* sinomcu 13 bit devices for mc34p01_b(one of 33pa4_mlm) */
extern const struct proc_class proc_class_sn13c;    /* sinomcu 13 bit devices for mc31p5130 */

#define PROC_CLASS_EEPROM8 (&proc_class_eeprom8)
#define PROC_CLASS_EEPROM16 (&proc_class_eeprom16)
#define PROC_CLASS_GENERIC (&proc_class_generic)
#define PROC_CLASS_PIC12 (&proc_class_pic12)
#define PROC_CLASS_SX (&proc_class_sx)
#define PROC_CLASS_PIC14 (&proc_class_pic14)
#define PROC_CLASS_PIC14E (&proc_class_pic14e)
#define PROC_CLASS_PIC16 (&proc_class_pic16)
#define PROC_CLASS_PIC16E (&proc_class_pic16e)
#define PROC_CLASS_SN14 (&proc_class_sn14)
#define PROC_CLASS_SN16 (&proc_class_sn16)
#define PROC_CLASS_SN16A (&proc_class_sn16a)
#define PROC_CLASS_SN16B (&proc_class_sn16b)
#define PROC_CLASS_SN16C (&proc_class_sn16c)
#define PROC_CLASS_SN16D (&proc_class_sn16d)
#define PROC_CLASS_SN16E (&proc_class_sn16e)
#define PROC_CLASS_SN13 (&proc_class_sn13)
#define PROC_CLASS_SN13B (&proc_class_sn13b)
#define PROC_CLASS_SN13BB (&proc_class_sn13bb)
#define PROC_CLASS_SN13C (&proc_class_sn13c)

/* The 16bit core is handled differently in some instances. */
#define IS_16BIT_CORE (state.device.class == PROC_CLASS_PIC16E)

typedef const struct px *pic_processor_t;
#define no_processor ((const struct px *)0)

#define MAX_NAMES 3 /* Maximum number of names a processor can have */
#define MAX_BADROM (1*2) /* Maximum number of BADROM ranges a processor can be */
                       /* initialized with */

struct px {
  proc_class_t class;
  const char *defined_as;
  const char *names[MAX_NAMES];
  unsigned int coff_type;
  int num_pages;
  int num_banks;
  /* These are in org to make it easier to fill from datasheet */
  int maxrom;
  int badrom[MAX_BADROM];
  int config_addrs[2];
  int chip_id;
  const char *script;
};

/* CONFIG addresses for the 18xx parts */
#define CONFIG1L  0x300000
#define CONFIG1H  0x300001
#define CONFIG2L  0x300002
#define CONFIG2H  0x300003
#define CONFIG3L  0x300004
#define CONFIG3H  0x300005
#define CONFIG4L  0x300006
#define CONFIG4H  0x300007
#define CONFIG5L  0x300008
#define CONFIG5H  0x300009
#define CONFIG6L  0x30000a
#define CONFIG6H  0x30000b
#define CONFIG7L  0x30000c
#define CONFIG7H  0x30000d
#define DEVID1    0x3ffffe
#define DEVID2    0x3fffff

/* ID Locations for the 18xx parts */
#define IDLOC0    0x200000
#define IDLOC1    0x200001
#define IDLOC2    0x200002
#define IDLOC3    0x200003
#define IDLOC4    0x200004
#define IDLOC5    0x200005
#define IDLOC6    0x200006
#define IDLOC7    0x200007

/* Config address for everything else */
#define CONFIG_17CXX 0xfe00
#define CONFIG_ADDRESS_14  0x2007
#define CONFIG_ADDRESS_12  0x0fff

void gp_dump_processor_list(gp_boolean list_all, proc_class_t class);
const struct px *gp_find_processor(const char *name);
proc_class_t gp_processor_class(pic_processor_t);
int gp_processor_bsr_boundary(pic_processor_t processor);
unsigned long gp_processor_coff_type(pic_processor_t processor);
int gp_processor_num_pages(pic_processor_t processor);
int gp_processor_num_banks(pic_processor_t processor);
pic_processor_t gp_processor_coff_proc(unsigned long coff_type);
const char *gp_processor_name(pic_processor_t processor, unsigned int choice);
const char *gp_processor_coff_name(unsigned long coff_type, unsigned int choice);
const char *gp_processor_script(pic_processor_t processor);
unsigned int gp_processor_id_location(pic_processor_t processor);
int gp_processor_rom_width(proc_class_t class);
int gp_processor_check_page(proc_class_t class, unsigned int address);
int gp_processor_check_bank(proc_class_t class, unsigned int address);
int gp_processor_set_page(proc_class_t class,
                          int num_pages,
                          int page,
                          MemBlock *m,
                          unsigned int address,
                          int use_wreg);
int gp_processor_set_bank(proc_class_t class,
                          int num_banks,
                          int bank,
                          MemBlock *m,
                          unsigned int address);

int gp_processor_retlw(proc_class_t class);

int gp_processor_org_to_byte(proc_class_t class, int org);
int gp_processor_byte_to_org(proc_class_t class, int byte);

int gp_org_to_byte(unsigned shift, int org);
int gp_byte_to_org(unsigned shift, int byte);

#endif
