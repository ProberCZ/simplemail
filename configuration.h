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

/*
** configuration.h
*/

#ifndef SM__CONFIGURATION_H
#define SM__CONFIGURATION_H

#ifndef SM__LISTS_H
#include "lists.h"
#endif

struct pop3_server;

struct config
{
	char *realname;
	char *email;
	int dst;

	char *smtp_server;
	int smtp_port;
	char *smtp_domain;
	int smtp_ip_as_domain;
	int smtp_auth; /* 1 if SMTP Auth should be used */
	char *smtp_login;
	char *smtp_password;

	int receive_preselection; /* 0 no selection, 1 size selection, 2 full selection */
	int receive_size; /* the size in kb */

  /* list of pop3 servers */
  struct list receive_list;

	int read_wordwrap;
};

struct user
{
	char *name; /* name of the user */
	char *directory; /* the directory where all data is saved */

	char *config_filename; /* path to the the configuration */

	struct config config;
};

int load_config(void);
void save_config(void);
void free_config_pop(void);
void insert_config_pop(struct pop3_server *pop);

extern struct user user; /* the current user */

#endif


