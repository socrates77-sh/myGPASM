/* General system functions
   Copyright (C) 2003, 2004, 2005
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

#ifdef HAVE_WINDOWS_H
#include <windows.h>
#endif

#define HEADER_PATH_SN "./header"
#define LKR_PATH_SN "./lkr"
#define LIB_PATH_SN "./library"

char *gp_header_path;
char *gp_lkr_path;
char *gp_lib_path;

gp_boolean absolute_path_warning = true;

/* initialize the library */

void gp_init(void)
{

#ifdef USE_DEFAULT_PATHS
  /* load the environmental variables */
  char *env_path;
  env_path = getenv("SN_PATH");
  if (env_path != NULL)
  {
    gp_header_path = (char *)malloc(BUFSIZ);
    gp_lkr_path = (char *)malloc(BUFSIZ);
    gp_lib_path = (char *)malloc(BUFSIZ);
    sprintf(gp_header_path, "%s\\header", env_path);
    sprintf(gp_lkr_path, "%s\\lkr", env_path);
    sprintf(gp_lib_path, "%s\\lib", env_path);
  }
  else
  {
    gp_header_path = NULL;
    gp_lkr_path = NULL;
    gp_lib_path = NULL;
  }

#ifndef HAVE_DOS_BASED_FILE_SYSTEM
  if (gp_header_path == NULL)
  {
    gp_header_path = strdup(HEADER_PATH_SN);
  }
  if (gp_lkr_path == NULL)
  {
    gp_lkr_path = strdup(LKR_PATH_SN);
  }
  if (gp_lib_path == NULL)
  {
    gp_lib_path = strdup(LIB_PATH_SN);
  }
#endif

#else
  gp_header_path = NULL;
  gp_lkr_path = NULL;
  gp_lib_path = NULL;
#endif
}

/* little endian functions */

void gp_fputl16(short data, FILE *fp)
{
  fputc(data & 255, fp);
  fputc((data >> 8) & 255, fp);

  return;
}

void gp_fputl32(long data, FILE *fp)
{
  fputc(data & 255, fp);
  fputc((data >> 8) & 255, fp);
  fputc((data >> 16) & 255, fp);
  fputc((data >> 24) & 255, fp);

  return;
}

void gp_fputvar(const void *data_, int number, FILE *fp)
{
  const unsigned char *data = data_;
  int i;

  for (i = 0; i < number; i++)
    fputc(data[i], fp);

  return;
}

short gp_getl16(const unsigned char *addr)
{
  short value;

  value = addr[0];
  value |= addr[1] << 8;

  return value;
}

unsigned short
gp_getu16(const unsigned char *addr)
{
  return (unsigned short)gp_getl16(addr);
}

long gp_getl32(const unsigned char *addr)
{
  long value;

  value = addr[0];
  value |= addr[1] << 8;
  value |= addr[2] << 16;
  value |= addr[3] << 24;

  return value;
}

void gp_putl16(unsigned char *addr, short data)
{
  addr[1] = (data >> 8) & 0xff;
  addr[0] = data & 0xff;

  return;
}

void gp_putl32(unsigned char *addr, long data)
{
  addr[0] = data & 0xff;
  addr[1] = (data >> 8) & 0xff;
  addr[2] = (data >> 16) & 0xff;
  addr[3] = (data >> 24) & 0xff;

  return;
}

/* big endian functions */

long gp_getb32(const unsigned char *addr)
{
  long value;

  value = addr[0] << 24;
  value |= addr[1] << 16;
  value |= addr[2] << 8;
  value |= addr[3];

  return value;
}

void gp_putb32(unsigned char *addr, long data)
{
  addr[0] = (data >> 24) & 0xff;
  addr[1] = (data >> 16) & 0xff;
  addr[2] = (data >> 8) & 0xff;
  addr[3] = data & 0xff;

  return;
}

void gp_date_string(char *buffer, size_t sizeof_buffer)
{
  time_t now;
  struct tm *now_tm;

  time(&now);
  now_tm = localtime(&now);
  snprintf(buffer, sizeof_buffer,
           "%d-%d-%d  %02d:%02d:%02d",
           1900 + now_tm->tm_year,
           now_tm->tm_mon + 1,
           now_tm->tm_mday,
           now_tm->tm_hour,
           now_tm->tm_min,
           now_tm->tm_sec);

  return;
}

char *
gp_lower_case(const char *name)
{
  char *new;
  char *ptr;

  ptr = new = strdup(name);

  while (*ptr != '\0')
  {
    *ptr = tolower(*ptr);
    ptr++;
  }

  return new;
}

char *
gp_upper_case(const char *name)
{
  char *new;
  char *ptr;

  ptr = new = strdup(name);

  while (*ptr != '\0')
  {
    *ptr = toupper(*ptr);
    ptr++;
  }

  return new;
}

/* linked list functions */

gp_linked_list *
gp_list_make(void)
{
  gp_linked_list *new;

  new = malloc(sizeof(*new));
  new->annotation = NULL;
  new->prev = NULL;
  new->next = NULL;

  return new;
}

void gp_list_annotate(gp_linked_list *link, void *a)
{
  link->annotation = a;
}

void *
gp_list_get(gp_linked_list *link)
{
  return link->annotation;
}

/* fetch the absolute path of the filename */
char *
gp_absolute_path(char *file_name)
{
#ifdef HAVE_WINDOWS_H

/* It would be better to test for GetFullPathName, but the test won't
     work with a cross compiler. So if windows.h exists, we assume that
     GetFullPathName is available. */

#define FILE_BUFFER_SIZE 512
  char file_buffer[FILE_BUFFER_SIZE];
  char *file_ptr;
  int num_chars;

  num_chars = GetFullPathName(file_name,
                              FILE_BUFFER_SIZE,
                              file_buffer,
                              &file_ptr);
  if (num_chars == 0)
  {
    gp_error("can't fetch full path of %s", file_name);
    return file_name;
  }
  else
  {
    return strdup(file_buffer);
  }
#else

  if (absolute_path_warning)
  {
    gp_warning("host system does not support absolute paths");
    absolute_path_warning = false;
  }

  return file_name;

#endif
}
