#ifdef TYPED_NAME
#ifdef TYPE
typedef struct
{
    int numElements;
    int numAlloc;
    TYPE * list;
} TYPED_NAME(List) ;

TYPED_NAME(List) TYPED_NAME(ListCreate)();


void TYPED_NAME(ListPush)(TYPED_NAME(List) * dyn, TYPE element);

TYPE * TYPED_NAME(ListGetPointer)(TYPED_NAME(List) *dyn,int n);

void TYPED_NAME(ListShrink) (TYPED_NAME(List) * dyn);
TYPE * TYPED_NAME(ListGetStart) ( TYPED_NAME(List) * dyn);

TYPE * TYPED_NAME(ListGetEnd)( TYPED_NAME(List) * dyn);

void TYPED_NAME(ListFree) ( TYPED_NAME(List) * dyn);
#endif
#endif
