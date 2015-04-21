#ifndef LOCALIZATION_H
#define LOCALIZATION_H

#endif // LOCALIZATION_H

#include "../include/cellulo_localization.h"
#include "decoder.h"
#include "grid.h"
#include "list.h"
#include "neighbours.h"
#include "imgproc.h"

struct _PositionInfo{
    int decoded;
    IntPoint means[2];
    RotationDecoder rotDec;
    IntPoint position;
    IntPoint positionCertainty;
};
