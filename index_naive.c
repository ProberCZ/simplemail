/**
 * index_naive.c - a naive string index implementation for SimpleMail.
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

/**
 * @file index_naive.c
 */

#include <stdlib.h>
#include <string.h>

#include "lists.h"

#include "index.h"
#include "index_private.h"
#include "index_naive.h"

struct document_node
{
	struct node node;
	int did;
	char *txt;
};

struct index_naive
{
	struct index index;
	struct list document_list;
};


struct index *index_naive_create(const char *filename)
{
	struct index_naive *idx;

	if (!(idx = (struct index_naive*)malloc(sizeof(*idx))))
		return NULL;

	memset(idx,0,sizeof(*idx));
	list_init(&idx->document_list);

	return &idx->index;
}

void index_naive_dispose(struct index *index)
{
	struct document_node *d;
	struct index_naive *idx;

	idx = (struct index_naive*)index;

	while ((d = (struct document_node *)list_remove_tail(&idx->document_list)))
	{
		free(d->txt);
		free(d);
	}

	free(index);
}

int index_naive_put_document(struct index *index, int did, const char *text)
{
	struct document_node *d;
	struct index_naive *idx;

	idx = (struct index_naive*)index;

	if (!(d = (struct document_node*)malloc(sizeof(*d))))
		return 0;
	memset(d,0,sizeof(*d));

	d->did = did;
	if (!(d->txt = strdup(text)))
	{
		free(d);
		return 0;
	}

	list_insert_tail(&idx->document_list,&d->node);
	return 1;
}

int index_naive_remove_document(struct index *index, int did)
{
	return 0;
}

int index_naive_find_documents(struct index *index, int (*callback)(int did, void *userdata), void *userdata, int num_substrings, va_list substrings)
{
	return 0;
}

/*****************************************************/

struct index_algorithm index_naive =
{
		index_naive_create,
		index_naive_dispose,
		index_naive_put_document,
		index_naive_remove_document,
		index_naive_find_documents
};
