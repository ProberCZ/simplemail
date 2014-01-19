/**
 * index_unittest - a simple test for the simple string index interface for SimpleMail.
 * Copyright (C) 2012  Sebastian Bauer
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <CUnit/Basic.h>

#include "index.h"
#include "index_naive.h"
#include "index_external.h"

static const char zauberlehrling[] =
	"Hat der alte Hexenmeister\n"
	"sich doch einmal wegbegeben!\n"
	"Und nun sollen seine Geister\n"
	"auch nach meinem Willen leben.\n"
	"Seine Wort und Werke\n"
	"merkt ich und den Brauch,\n"
	"und mit Geistesst�rke\n"
	"tu ich Wunder auch.\n\n"
	"Walle! walle\n"
	"Manche Strecke,\n"
	"da�, zum Zwecke,\n"
	"Wasser flie�e\n"
	"und mit reichem, vollem Schwalle\n"
	"zu dem Bade sich ergie�e.\n\n"
	"Und nun komm, du alter Besen!\n"
	"Nimm die schlechten Lumpenh�llen;\n"
	"bist schon lange Knecht gewesen:\n"
	"nun erf�lle meinen Willen!\n"
	"Auf zwei Beinen stehe,\n"
	"oben sei ein Kopf,\n"
	"eile nun und gehe\n"
	"mit dem Wassertopf!\n\n"
	"Walle! walle\n"
	"manche Strecke,\n"
	"da�, zum Zwecke,\n"
	"Wasser flie�e\n"
	"und mit reichem, vollem Schwalle\n"
	"zu dem Bade sich ergie�e.\n\n"
	"Seht, er l�uft zum Ufer nieder,\n"
	"Wahrlich! ist schon an dem Flusse,\n"
	"und mit Blitzesschnelle wieder\n"
	"ist er hier mit raschem Gusse.\n"
	"Schon zum zweiten Male!\n"
	"Wie das Becken schwillt!\n"
	"Wie sich jede Schale\n"
	"voll mit Wasser f�llt!\n\n"
	"Stehe! stehe!\n"
	"denn wir haben\n"
	"deiner Gaben\n"
	"vollgemessen! -\n"
	"Ach, ich merk es! Wehe! wehe!\n"
	"Hab ich doch das Wort vergessen!\n\n"
	"Ach, das Wort, worauf am Ende\n"
	"er das wird, was er gewesen.\n"
	"Ach, er l�uft und bringt behende!\n"
	"W�rst du doch der alte Besen!\n"
	"Immer neue G�sse\n"
	"bringt er schnell herein,\n"
	"Ach! und hundert Fl�sse\n"
	"st�rzen auf mich ein.\n\n"
	"Nein, nicht l�nger\n"
	"kann ichs lassen;\n"
	"will ihn fassen.\n"
	"Das ist T�cke!\n"
	"Ach! nun wird mir immer b�nger!\n"
	"Welche Miene! welche Blicke!\n\n"
	"O du Ausgeburt der H�lle!\n"
	"Soll das ganze Haus ersaufen?\n"
	"Seh ich �ber jede Schwelle\n"
	"doch schon Wasserstr�me laufen.\n"
	"Ein verruchter Besen,\n"
	"der nicht h�ren will!\n"
	"Stock, der du gewesen,\n"
	"steh doch wieder still!\n\n"
	"Willst am Ende\n"
	"gar nicht lassen?\n"
	"Will dich fassen,\n"
	"will dich halten\n"
	"und das alte Holz behende\n"
	"mit dem scharfen Beile spalten.\n\n"
	"Seht da kommt er schleppend wieder!\n"
	"Wie ich mich nur auf dich werfe,\n"
	"gleich, o Kobold, liegst du nieder;\n"
	"krachend trifft die glatte Sch�rfe.\n"
	"Wahrlich, brav getroffen!\n"
	"Seht, er ist entzwei!\n"
	"Und nun kann ich hoffen,\n"
	"und ich atme frei!\n\n"
	"Wehe! wehe!\n"
	"Beide Teile\n"
	"stehn in Eile\n"
	"schon als Knechte\n"
	"v�llig fertig in die H�he!\n"
	"Helft mir, ach! ihr hohen M�chte!\n\n"
	"Und sie laufen! Na� und n�sser\n"
	"wirds im Saal und auf den Stufen.\n"
	"Welch entsetzliches Gew�sser!\n"
	"Herr und Meister! h�r mich rufen! -\n"
	"Ach, da kommt der Meister!\n"
	"Herr, die Not ist gro�!\n"
	"Die ich rief, die Geister\n"
	"werd ich nun nicht los.\n\n"
	"\"In die Ecke,\n"
	"Besen, Besen!\n"
	"Seids gewesen.\n"
	"Denn als Geister\n"
	"ruft euch nur zu diesem Zwecke,\n"
	"erst hervor der alte Meister.\"";

/*******************************************************/

static int test_index_naive_callback_called;

static int test_index_naive_callback(int did, void *userdata)
{
	CU_ASSERT(did==4);
	test_index_naive_callback_called = 1;
}

static int test_index_naive_callback2(int did, void *userdata)
{
	CU_ASSERT(0);
}

static void test_index_for_algorithm(struct index_algorithm *alg, const char *name)
{
	struct index *index;
	int ok;
	int nd;

	test_index_naive_callback_called = 0;

	index = index_create(alg, name);
	CU_ASSERT(index != NULL);

	ok = index_put_document(index,4,"This is a very long text.");
	CU_ASSERT(ok != 0);

	ok = index_put_document(index,12,"This is a short text.");
	CU_ASSERT(ok != 0);

	ok = index_put_document(index,20,zauberlehrling);
	CU_ASSERT(ok != 0);

	nd = index_find_documents(index,test_index_naive_callback,NULL,1,"very");
	CU_ASSERT(test_index_naive_callback_called == 1);
	CU_ASSERT(nd == 1);

	ok = index_remove_document(index,4);
	CU_ASSERT(ok != 0);

	nd = index_find_documents(index,test_index_naive_callback2,NULL,1,"very");
	CU_ASSERT(nd == 0);

	index_dispose(index);
}

/*******************************************************/

/* @Test */
void test_index_naive(void)
{
	test_index_for_algorithm(&index_naive, "naive-index.dat");
}

/*******************************************************/

/* @Test */
void test_index_external(void)
{
	test_index_for_algorithm(&index_external, "/tmp/external-index.dat");
}
