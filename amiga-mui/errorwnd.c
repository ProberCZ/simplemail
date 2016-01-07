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

/**
 * @file errorwnd.c
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <libraries/iffparse.h> /* MAKE_ID */
#include <libraries/mui.h>
#include <mui/NListview_mcc.h>
#include <clib/alib_protos.h>
#include <proto/exec.h>
#include <proto/intuition.h>
#include <proto/muimaster.h>

#include "debug.h"
#include "lists.h"
#include "logging.h"
#include "smintl.h"
#include "support_indep.h"

#include "errorwnd.h"
#include "muistuff.h"
#include "timesupport.h"

struct error_node
{
	struct node node;
	char *text;
	unsigned int date;
	unsigned int id;
};

static struct list error_list;

static Object *error_wnd;
static Object *all_errors_list;
static Object *text_list;
static Object *delete_button;
static Object *close_button;

/**
 * Callback for selecting the currently displayed error message.
 */
static void error_select(void)
{
	int num = xget(all_errors_list, MUIA_NList_Active);
	struct error_node *node = (struct error_node *)list_find(&error_list,num);

	set(text_list, MUIA_NList_Quiet, TRUE);
	DoMethod(text_list, MUIM_NList_Clear);

	if (node)
	{
		DoMethod(text_list, MUIM_NList_InsertSingleWrap, (ULONG)node->text, MUIV_NList_Insert_Bottom, WRAPCOL0, ALIGN_LEFT);
	}

	set(text_list, MUIA_NList_Quiet, FALSE);
}

/**
 * Delete all messages.
 */
static void delete_messages(void)
{
	struct error_node *node;

	set(error_wnd, MUIA_Window_Open, FALSE);

	DoMethod(all_errors_list, MUIM_NList_Clear);
	while ((node = (struct error_node *)list_remove_tail(&error_list)))
	{
		if (node->text) free(node->text);
		free(node);
	}

}

STATIC ASM SAVEDS VOID error_display(REG(a0,struct Hook *h),REG(a2,Object *obj),REG(a1,struct NList_DisplayMessage *msg))
{
	struct error_node *error = (struct error_node*)msg->entry;
	if (!error)
	{
		msg->strings[0] = _("Time");
		msg->strings[1] = _("Message");
		return;
	}
	msg->strings[0] = sm_get_time_str(error->date);
	msg->strings[1] = error->text;
}

/**
 * Initialize the error window.
 */
static void init_error(void)
{
	static struct Hook error_display_hook;

	if (!MUIMasterBase) return;

	init_hook(&error_display_hook, (HOOKFUNC)error_display);

	list_init(&error_list);

	error_wnd = WindowObject,
		MUIA_Window_ID, MAKE_ID('E','R','R','O'),
    MUIA_Window_Title, "SimpleMail - Error",
    WindowContents, VGroup,
			Child, NListviewObject,
				MUIA_CycleChain, 1,
				MUIA_NListview_NList, all_errors_list = NListObject,
					MUIA_NList_Format, ",",
					MUIA_NList_DisplayHook2, &error_display_hook,
					MUIA_NList_Title, TRUE,
					End,
				End,
			Child, BalanceObject, End,
			Child, NListviewObject,
				MUIA_Weight, 33,
				MUIA_CycleChain, 1,
				MUIA_NListview_NList, text_list = NListObject,
					MUIA_NList_TypeSelect, MUIV_NList_TypeSelect_Char,
					End,
				End,
			Child, HGroup,
				Child, delete_button = MakeButton("_Delete messages"),
				Child, close_button = MakeButton("_Close window"),
				End,
			End,
		End;

	if (error_wnd)
	{
		DoMethod(App, OM_ADDMEMBER, (ULONG)error_wnd);
		DoMethod(error_wnd, MUIM_Notify, MUIA_Window_CloseRequest, TRUE, (ULONG)error_wnd, 3, MUIM_Set, MUIA_Window_Open, FALSE);
		DoMethod(delete_button, MUIM_Notify, MUIA_Pressed, FALSE, (ULONG)delete_button, 3, MUIM_CallHook, (ULONG)&hook_standard, (ULONG)delete_messages);
		DoMethod(close_button, MUIM_Notify, MUIA_Pressed, FALSE, (ULONG)error_wnd, 3, MUIM_Set, MUIA_Window_Open, FALSE);
		DoMethod(all_errors_list, MUIM_Notify, MUIA_NList_Active, MUIV_EveryTime, (ULONG)all_errors_list, 3, MUIM_CallHook, (ULONG)&hook_standard, (ULONG)error_select);
	}
}

/*****************************************************************************/

/**
 * Add the logg entry to the error list.
 *
 * @param l
 */
static void error_window_add_logg(logg_t l)
{
	struct error_node *node;

	if (!(node = malloc(sizeof(*node))))
		return;

	memset(node, 0, sizeof(*node));
	node->id = logg_id(l);
	node->date = logg_seconds(l);
	if (!(node->text = mystrdup(logg_text(l))))
	{
		free(node);
		return;
	}
	list_insert_tail(&error_list, &node->node);
}

/**
 * Update the error window log with the information of the logg.
 */
static void error_window_update_logg(void)
{
	struct error_node *enode;
	logg_t l;

	set(all_errors_list, MUIA_NList_Quiet, TRUE);
	DoMethod(all_errors_list, MUIM_NList_Clear);

	enode = (struct error_node*)list_first(&error_list);
	l = logg_next(NULL);

	while (enode && l)
	{
		struct error_node *cur = enode;

		enode = (struct error_node*)node_next(&enode->node);

		/* Keep no logg-ones (i.e., added via error_add_message()) */
		if (cur->id == ~0) continue;

		/* Keep id-matching one (they represent the same entry) */
		if (cur->id == logg_id(l))
		{
			l = logg_next(l);
			continue;
		}

		/* If id of the view log list is smaller it is an obsolete one,
		 * remove it.
		 */
		if (cur->id < logg_id(l))
		{
			node_remove(&cur->node);
			free(cur->text);
			free(cur);
			continue;
		}
	}

	if (l)
	{
		/* Add the remaining logg entries */
		do
		{
			error_window_add_logg(l);
		} while((l = logg_next(l)));
	}

	/* Populate the view again */
	enode = (struct error_node*)list_first(&error_list);
	while (enode)
	{
		DoMethod(all_errors_list, MUIM_NList_InsertSingle, enode, MUIV_NList_Insert_Bottom);
		enode = (struct error_node*)node_next(&enode->node);
	}

	set(all_errors_list, MUIA_NList_Quiet, FALSE);
}

/*****************************************************************************/

void error_window_open(void)
{
	if (!error_wnd)
	{
		init_error();

		if (!error_wnd) return;
	}

	error_window_update_logg();

	set(error_wnd, MUIA_Window_Open, TRUE);
}
