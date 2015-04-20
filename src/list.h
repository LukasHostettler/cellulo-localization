//This file merely includes lists of different types.


#ifndef LIST_H
#define LIST_H

#include "imgsegment.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define TYPE int
#define TYPED_NAME(x) int##x
#include "list_impl.h"
#undef TYPE
#undef TYPED_NAME

#define TYPE ImgSegment
#define TYPED_NAME(x) imgSeg##x
#include "list_impl.h"
#undef TYPE
#undef TYPED_NAME

#ifdef __cplusplus
}
#endif

#endif // LIST_H

