

#ifdef TYPE
#ifdef TYPED_NAME

#include "assert.h"


TYPED_NAME(List) TYPED_NAME(ListCreate)(){
    TYPED_NAME(List) ans;
    ans.list=0;
    ans.numAlloc=0;
    ans.numElements=0;
    return ans;
}


void TYPED_NAME(ListPush)(TYPED_NAME(List) * dyn, TYPE element){
    assert(dyn);
    if (dyn->numElements >= dyn->numAlloc)
    {
        assert(dyn->numElements == dyn->numAlloc);
        int newSize = (dyn->numAlloc + 2) * 2;
        TYPE * newList = (TYPE *)realloc(dyn->list, newSize * sizeof(TYPE));
        if (newList == 0){
            ;//...deal with out of memory condition...
        }
        dyn->numAlloc = newSize;
        dyn->list      = newList;
    }
    dyn->list[dyn->numElements++]=element;

}

TYPE * TYPED_NAME(ListGetPointer)(TYPED_NAME(List) *dyn,int n){
    if(n<dyn->numElements)
        return dyn->list+sizeof(TYPE)*n;
    return 0;
}

void TYPED_NAME(ListShrink) (TYPED_NAME(List) * dyn){
    TYPE *newList = (TYPE *)realloc(dyn->list,dyn->numElements*sizeof(TYPE));
    if (newList!=NULL)
        dyn->list=newList;
}
TYPE * TYPED_NAME(ListGetStart) ( TYPED_NAME(List) * dyn){
    return dyn->list;
}
TYPE * TYPED_NAME(ListGetEnd)( TYPED_NAME(List) * dyn){
    return dyn->list+dyn->numElements*sizeof(TYPE);
}
void TYPED_NAME(ListFree) ( TYPED_NAME(List) * dyn){
    free(dyn->list);
    dyn->list=NULL;
    dyn->numAlloc=0;
    dyn->numElements=0;
}
#endif
#endif
