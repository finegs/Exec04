/*
 * frames.cpp
 *
 *  Created on: 2018. 10. 27.
 *      Author: finegs
 */
#include <frames.h>
#include <list.h>
#include <stdlib.h>

int allocFrame(List *frames) {
	int 	frameNumber,
			*data;

	if(listSize(frames) == 0) {
		return -1;
	}
	else {
		if(listRemoveNext(frames, NULL, (void**)&data) != 0) {
			return -1;
		}
		else {
			frameNumber = *data;
			free(data);
		}
	}
	return frameNumber;
}

int freeFrame(List *frames, int frameNumber) {
	int	*data;

	if((data = (int*)malloc(sizeof(int))) == NULL)
		return -1;

	*data = frameNumber;
	if(listInsNext(frames, NULL, data) != 0)
		return -1;

	return 0;
}

