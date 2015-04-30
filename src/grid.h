#ifndef GRID_H
#define GRID_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "list.h"
#include "neighbours.h"
#include "intgrid.h"




//reports success
int correctMeanLength(IntPoint * pointArray, IntPoint *means, int power, int n, IntPoint *origin);
typedef struct _DotInformation{
    int numElements;
    int gridMaxRows;
    int gridMaxCols;
    IntPoint gridOrigin;
    int * xoffsets;
    int * yoffsets;
    IntPoint * gridCoordinate;
    //Prob?
}DotInformation;
DotInformation dotInfoInit(IntPoint * pointArray,IntPoint * means,int power,int n,IntPoint origin);
void dotInfoFree(DotInformation * d);

typedef struct _ProbabilityGrids{
    IntGrid maxProb;
    IntGrid prob1;
    IntGrid prob2;
}ProbabilityGrids;
void probabilityGridsTurn(ProbabilityGrids * grids, int numRightTurns);
ProbabilityGrids makeProbabilityGrids(DotInformation dotInfo);
void probabilityGridsFree(ProbabilityGrids * grid);

#ifdef __cplusplus
}
#endif
#endif // GRID_H
