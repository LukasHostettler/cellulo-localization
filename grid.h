#ifndef GRID_H
#define GRID_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "list.h"
#include "neighbours.h"

#define NOPOINT (-1)
typedef struct{
    IntPoint origin;
    int numRows,numCols;
    IntPoint *offset;
    int * pointIndex;
    int * probabilities[4];
    int * max_probablitiy;
}Grid;



typedef struct{
    int numCols;
    int numRows;
    int *data;   //The actual pointer to data.
    int **array; //pointer construct for easy access: IntGrid[numRow][numCol];
}IntGrid;

IntGrid intGridCreateEmpty();
IntGrid intGridCreate(int numCol, int numRow);
IntGrid intGridCopy(IntGrid other);
IntGrid intGridCopySub(IntGrid other,int topRow, int leftCol,int numRows, int numCols);
IntGrid intGridAdd(IntGrid a,IntGrid b);
IntGrid intGridMax(IntGrid a,IntGrid b);
void intGridApplyToEveryElement(IntGrid g,int(*f)(int));
IntGrid intGridLocalSum(IntGrid g,int windowSize);

void intGridFree(IntGrid * g);
void intGridTest();



typedef struct{
    IntGrid prob[4];
}ProbabilityGrid;

//ProbabilityGrid makeProbabilityGrid(IntPoint * pointArray,IntPoint *means,int power,int n);
ProbabilityGrid getBestNxN(ProbabilityGrid g,int n);
ProbabilityGrid calculateProbabilities(Grid grid,IntGrid *maxProb,float offset,int power);
ProbabilityGrid probabilityGridConstrainToBest(ProbabilityGrid other,IntGrid max,int number);
void probabilityGridFree(ProbabilityGrid p);




//reports success
int correctMeanLength(IntPoint * pointArray, IntPoint *means, int power, int n, IntPoint *origin);

Grid makeGrid(IntPoint * pointArray, IntPoint * means, int power, int n, IntPoint origin);
void deleteGrid(Grid g);//frees all arrays
//void calculateProbabilities(Grid grid,float offset,int power);

#ifdef __cplusplus
}
#endif
#endif // GRID_H
