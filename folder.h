#ifndef SM__FOLDER_H
#define SM__FOLDER_H

#ifndef MAIL_H
#include "mail.h"
#endif

struct folder
{
	char *name; /* the name like it is displayed */
	char *path; /* the path of the folder */

	/* the following data is private only */

	/* the array of mails, no list used here because this would slowdown maybe the processing later */
	struct mail **mail_array; /* but don't access this here, use folder_next_mail() */
	int mail_array_allocated; /* how many entries could be in the array */
	int num_mails; /* number of mails */

	int primary_sort;
	int secondary_sort;

	struct mail **sorted_mail_array; /* the sorted mail array, NULL if not sorted */

	/* more will follow */
};

/* the sort modes of the folder */
#define FOLDER_SORT_STATUS		0
#define FOLDER_SORT_AUTHOR		1
#define FOLDER_SORT_SUBJECT	2
#define FOLDER_SORT_REPLY		3
#define FOLDER_SORT_DATE			4
#define FOLDER_SORT_SIZE			5
#define FOLDER_SORT_FILENAME	6
#define FOLDER_SORT_THREAD		7  /* sort by thread, secondary is ignored */
#define FOLDER_SORT_MODEMASK	(0x7)
#define FOLDER_SORT_REVERSE	(0x80) /* reverse sort mode */

int init_folders(void);
void del_folders(void);

int folder_read_mail_infos(struct folder *folder);
int folder_add_mail(struct folder *folder, struct mail *mail);
int folder_add_mail_incoming(struct mail *mail);
void folder_replace_mail(struct folder *folder, struct mail *toreplace, struct mail *newmail);
struct mail *folder_find_mail_by_filename(struct folder *folder, char *filename);

struct folder *folder_add(char *name, char *path);
struct folder *folder_first(void);
struct folder *folder_next(struct folder *f);
struct folder *folder_find(int pos);
struct folder *folder_find_by_name(char *name);
struct folder *folder_find_by_path(char *name);
struct folder *folder_incoming(void);
struct folder *folder_outgoing(void);
struct folder *folder_deleted(void);
int folder_move_mail(struct folder *from_folder, struct folder *dest_folder, struct mail *mail);
int folder_delete_mail(struct folder *from_folder, struct mail *mail);

/* This was a macro, but now is a function. Handle must point to NULL to get the first mail */
struct mail *folder_next_mail(struct folder *folder, void **handle);
int folder_get_primary_sort(struct folder *folder);
void folder_set_primary_sort(struct folder *folder, int sort_mode);
int folder_get_secondary_sort(struct folder *folder);
void folder_set_secondary_sort(struct folder *folder, int sort_mode);

#endif
