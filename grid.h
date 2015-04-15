#ifndef GRID_H
#define GRID_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "list.h"
#include "neighbours.h"

#define NOPOINT -1

typedef struct _IntGrid{
    int numCols;
    int numRows;
    int *data;   //!<The actual pointer to data.
    int **array; //! Pointers construct for easy access: IntGrid[numRow][numCol];
}IntGrid; //! a dynamic array

IntGrid intGridCreateEmpty(); //!Creates an empty 0x0 IntGrid. There is no data allocation.
IntGrid intGridCreate(int numCol, int numRow); //!Creates an empty IntGrid of size numCol x numRow.
IntGrid intGridCopy(IntGrid other);//!Deep Copy of an IntGrid
IntGrid intGridCopySub(IntGrid other,int topRow, int leftCol,int numRows, int numCols);//!DeepCopy of a SubGrid
IntGrid intGridAdd(IntGrid a,IntGrid b);//!Adds Elementwise a+b. Raises Error when dimensions do not correspond.
IntGrid intGridMax(IntGrid a,IntGrid b);//!Find Elementwise maximum. Raises error when dimensions disagree.
void intGridApplyToEveryElement(IntGrid g,int(*f)(int)); //!Generic Function to apply functions to every Element
IntGrid intGridLocalSum(IntGrid g,int windowSize); //! makes a convolution with a square window of size windowsize.
void intGridFindBestNxN(IntGrid grid,int *nCol,int *nRow,int n);

void intGridFree(IntGrid * g);//!Frees the memory of an Intgrid
void intGridTest(); //! Test function to see wheter intGrid works.



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

ProbabilityGrids makeProbabilityGrids(DotInformation dotInfo);
void probabilityGridsFree(ProbabilityGrids * grid);

#ifdef __cplusplus
}
#endif
#endif // GRID_H
