#ifndef IMGPROC_H
#define IMGPROC_H

#include "list.h"
#ifdef __cplusplus
extern "C"
{
#endif

//creates a list of image segments. The user is responsible to free the list by himself
imgSegList segmentImage(unsigned char * imgPointer, int nRows,int nCols,unsigned char thresholdValue);


#ifdef __cplusplus
}
#endif

#endif // IMGPROC_H
