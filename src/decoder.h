#ifndef DECODER_H
#define DECODER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "grid.h"
int forwardProbability(IntGrid g, int startRow, int startCol);
int forwardProbability2(IntGrid g, int startRow, int startCol, int numRows, int numCols);

int downwardProbability(IntGrid g,int startRow,int startCol);
IntPoint * probabilities(int *offsetsx, int *offsetsy, int n);

typedef IntPoint RotationDecoder;

void rotationDecoderReset(RotationDecoder * rot);
void rotationDecoderUpdate(RotationDecoder * rot, int rotProb1,int   rotProb2);
int rotationDecoderUpdateMeans(RotationDecoder * rot, IntPoint * means, DotInformation *dotInfo);
void rotationDecoderDiminuish(RotationDecoder * rot);

IntPoint decodePos(ProbabilityGrids probGrids,int startRow,int startCol);


#ifdef __cplusplus
}
#endif
#endif // DECODER_H
