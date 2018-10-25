/*
 * frames.h
 *
 *  Created on: 2018. 10. 27.
 *      Author: finegs
 */

#ifndef INCLUDE_FRAMES_H_
#define INCLUDE_FRAMES_H_

#include <list.h>


int allocFrame(List *frames);
int freeFrame(List *frames, int frameNumbers);



#endif /* INCLUDE_FRAMES_H_ */
