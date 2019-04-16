/*
 * list.h
 *
 *  Created on: 2018. 10. 27.
 *      Author: finegs
 */

#ifndef INCLUDE_LIST_H_
#define INCLUDE_LIST_H_

#include <stdlib.h>

typedef struct ListElement_ {
	void *data;
	struct ListElement_ *next;
} ListElement;

typedef struct List_ {
	int size;
	int	(*match)(const void *key1, const void *key2);
	void	(*destroy)(void *data);

	ListElement_ *head;
	ListElement_ *tail;
} List;

void	listInit(List *list, void (*destroy)(void *data));
void	listDestroy(List *list);
int		listInsNext(List *list, ListElement *element, const void *data);
int		listRemoveNext(List *list, ListElement *element, void **data);
#define	listSize(list) ((list)->size)
#define listHead(list) ((list)->head)
#define listTail(list) ((list)->tail)
#define listIsHead(list, element) ((list)->head == (element) ? 1 : 0)
#define listIsTail(list, element) ((list)->tail == (element) ? 1 : 0)
#define	listData(element)	((element)->data)
#define	listNext(element)	((element)->next)

#endif /* INCLUDE_LIST_H_ */
