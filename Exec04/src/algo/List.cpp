/*
 * List.cpp
 *
 *  Created on: 2018. 10. 27.
 *      Author: finegs
 */
#include <list.h>
#include <stdlib.h>
#include <string.h>



void listInit(List *list, void (*destroy)(void *data)) {
	list->size = 0;
	list->destroy = destroy;
	list->head = NULL;
	list->tail	= NULL;

	return;
}

void listDestory(List *list) {
	void	*data;

	while(listSize(list) > 0) {
		if(listRemoveNext(list, NULL, (void**)&data) == 0
				&& list->destroy != NULL) {
			list->destroy(data);
		}
	}

	memset(list, 0, sizeof(List));
	return;
}

int listInsNext(List *list,  ListElement *element, const void *data) {
	ListElement *newElement;

	if((newElement = (ListElement*)malloc(sizeof(ListElement))) == NULL)
		return -1;

	newElement->data = (void*)data;

	if(element == NULL) {
		if(listSize(list) == 0)
			list->tail = newElement;

		newElement->next = list->head;
		list->head = newElement;
	}
	else {
		if(element->next == NULL)
			list->tail = newElement;

		newElement->next = element->next;
		element->next = newElement;
	}

	list->size++;

	return 0;
}

int listRemoveNext(List *list, ListElement *element, void **data) {
	ListElement *oldElement;

	if(listSize(list) == 0) return -1;

	if(NULL == element) {
		*data = (oldElement = listHead(list))->data;
		list->head = oldElement->next;

		if(listSize(list) == 1)
			list->tail = NULL;
	}
	else {
		if(element->next == NULL)
			return -1;

		*data = (oldElement = listNext(element))->data;
		element->next = oldElement->next;

		if(listNext(element)==NULL)
			list->tail = element;

		free(oldElement);

		list->size--;

	}
	return 0;
}


