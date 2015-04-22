#ifndef NEIGHBOURS_H
#define NEIGHBOURS_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "list.h"

typedef struct _IntPoint{
 int x;
 int y;
}IntPoint;

void intPointMul(IntPoint * toMultiply,int factor);

typedef struct _Neighbours{
    int neigbourIdx[4];
    int isBorderPoint;
}Neighbours;
typedef struct _Edge{
    int start;
    int end;
    int dx;
    int dy;
    int direction; //-1
}Edge;

void intPointAdd(IntPoint *p,int x,int y);
static inline int intPointSquareDist(IntPoint p1,IntPoint p2){
    return (p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y);
}

int findCross(Neighbours * neigh, IntPoint *coords,int numNeigh,IntPoint * crossCoordinates);


Neighbours * establishNeighbourhood(IntPoint * pointArray,int num);
IntPoint * sortPointArray(imgSegList list, int multiplicator);
Edge * extractGoodEdges(Neighbours * neigh, IntPoint *coords,int num, int *numEdges);
void initMeans(IntPoint * means,int r);
void linkedKMeans(Edge * edges, IntPoint *oldMean, int num);
int intListMedian(intList list);

#ifdef __cplusplus
}
#endif

#endif // NEIGHBOURS_H
