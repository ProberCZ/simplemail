/*
** dlwnd.c
*/

#include <stdio.h>
#include <libraries/iffparse.h> /* MAKE_ID */
#include <libraries/mui.h>
#include <clib/alib_protos.h>
#include <proto/muimaster.h>
#include <proto/intuition.h>

#include "muistuff.h"

#include "subthreads.h"
#include "transwndclass.h"

static Object *win_dl;

int dl_window_init(void)
{
	int rc;
	
	if(create_transwnd_class())
	{
		rc = TRUE;
	}
	
	return(rc);
}

void dl_set_title(char *str)
{
	set(win_dl, MUIA_Window_Title, str);
}

void dl_set_status(char *str)
{
	set(win_dl, MUIA_transwnd_Status, str);
}

void dl_init_gauge_mail(int amm)
{
	static char str[256];
	
	sprintf(str, "Mail %%ld/%d", amm);
	
	set(win_dl, MUIA_transwnd_Gauge1_Str, str);
	set(win_dl, MUIA_transwnd_Gauge1_Max, amm);
	set(win_dl, MUIA_transwnd_Gauge1_Val, 0);
}

void dl_set_gauge_mail(int current)
{
	set(win_dl, MUIA_transwnd_Gauge1_Val, current);
}

void dl_init_gauge_byte(int size)
{
	static char str[256];
	
	sprintf(str, "%%ld/%d bytes", size);

	set(win_dl, MUIA_transwnd_Gauge2_Str, str);
	set(win_dl, MUIA_transwnd_Gauge2_Max, size);
	set(win_dl, MUIA_transwnd_Gauge2_Val, 0);
}

void dl_set_gauge_byte(int current)
{
	set(win_dl, MUIA_transwnd_Gauge2_Val, current);
}

void dl_abort(void)
{
	thread_abort();
}

int dl_window_open(void)
{
	int rc;
	
	rc = FALSE;

	if (!win_dl)
	{
		win_dl = transwndObject,
			MUIA_Window_ID,	MAKE_ID('T','R','D','L'),
		End;

		if (win_dl)
		{
			DoMethod(win_dl, MUIM_Notify, MUIA_transwnd_Aborted, TRUE, win_dl, 3, MUIM_CallHook, &hook_standard, dl_abort);
			DoMethod(App, OM_ADDMEMBER, win_dl);
		}
	}
	
	if(win_dl != NULL)
	{
		set(win_dl, MUIA_Window_Open, TRUE);
		rc = TRUE;
	}
	
	return(rc);
}

void dl_window_close(void)
{
	if(win_dl != NULL)
	{
		set(win_dl, MUIA_Window_Open, FALSE);
	}
}

int dl_checkabort(void)
{
	return 0; /* unneccessary in the amiga client, because it supports subthreading */
/*
	int rc;
	
	if(GetAttr(MUIA_transwnd_Aborted, win_dl, (ULONG *) &rc) == FALSE)
	{
		rc = FALSE;
	}
	
	return(rc);
*/
}