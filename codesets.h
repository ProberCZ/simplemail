/***************************************************************************
 SimpleMail - Copyright (C) 2000 Hynek Schlawack and Sebastian Bauer

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
***************************************************************************/

#ifndef SM__CODESETS_H
#define SM__CODESETS_H

#ifndef SM__LISTS_H
#include "lists.h"
#endif

typedef unsigned char	utf8;

struct single_convert
{
	unsigned char code; /* the code in this representation */
	char utf8[8]; /* the utf8 string, first byte is alway the length of the string */
	unsigned int ucs4; /* the full 32 bit unicode */
};

struct codeset
{
	struct node node;
	char *name;
	struct single_convert table[256];
	struct single_convert table_sorted[256];
};

char **codesets_supported(void);
int codesets_init(void);
void codesets_cleanup(void);
struct codeset *codesets_find(char *name);
struct codeset *codesets_find_best(char *text, int text_len);

#define utf8size(s) ((s)?(strlen(s)):(0))
#define utf8cpy(dest,src) ((utf8*)strcpy(dest,src))
#define utf8cat(dest,src) ((utf8*)strcat(dest,src))

int uft8len(const utf8 *str);
utf8 *uft8ncpy(utf8 *to, const utf8 *from, int n);
utf8 *utf8create(void *from, char *charset);
int utf8tostr(utf8 *str, char *dest, int dest_size, struct codeset *codeset);
int utf8tochar(utf8 *str, unsigned int *chr, struct codeset *codeset);

#endif
