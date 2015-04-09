
#include "list.h"
#include <stdlib.h>
#include "assert.h"

#ifdef TYPE
#undef TYPE
#endif

#ifdef TYPED_NAME
#undef TYPED_NAME
#endif

#define TYPE int
#define TYPED_NAME(x) int##x
#include "list_impl.c"
#undef TYPE
#undef TYPED_NAME

#define TYPE ImgSegment
#define TYPED_NAME(x) imgSeg##x
#include "list_impl.c"
#undef TYPE
#undef TYPED_NAME
