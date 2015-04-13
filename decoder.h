#ifndef DECODER_H
#define DECODER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "grid.h"
int forwardProbability(IntGrid g);
IntPoint * probabilities(int *offsetsx, int *offsetsy, int n);

#ifdef __cplusplus
}
#endif
#endif // DECODER_H
