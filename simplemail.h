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
#ifndef SM__SIMPLEMAIL_H
#define SM__SIMPLEMAIL_H

void callback_read_mail(void);
void callback_delete_mails(void);
void callback_new_mail(void);
void callback_change_mail(void);
void callback_fetch_mails(void);
void callback_send_mails(void);
void callback_addressbook(void);
void callback_folder_active(void);

void callback_write_mail_to(struct addressbook_entry *address);

void callback_maildrop(struct folder *dest_folder);

void callback_new_mail_arrived(struct mail *mail);
void callback_new_mail_written(struct mail *mail);

#endif
