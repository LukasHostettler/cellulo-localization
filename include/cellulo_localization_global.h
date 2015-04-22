#ifndef CELLULO_LOCALIZATION_GLOBAL_H
#define CELLULO_LOCALIZATION_GLOBAL_H

//#include <QtCore/qglobal.h>

#if defined(CELLULO_LOCALIZATION_LIBRARY)
#  define CELLULO_LOCALIZATIONSHARED_EXPORT Q_DECL_EXPORT
#else
#  define CELLULO_LOCALIZATIONSHARED_EXPORT Q_DECL_IMPORT
#endif

#ifdef __cplusplus
extern "C"{
#endif 

struct _PositionInfo;
typedef struct _PositionInfo PositionInfo;
///
/// image must be a 8-bit grayscale image with width and height specified;

PositionInfo * localize(PositionInfo * previousInfo, unsigned char *image, int rows, int cols);

float getX(PositionInfo * info);   //! returns horizontal position in grid coordinates
float getY(PositionInfo * info);   //! returns vertical position in grid coordinates
float getAngle(PositionInfo *info);//! returns angle in radians with respect to the horizontal in counterclockwise manner [-pi,pi]

float getCertaintyX(PositionInfo *info);
float getCertaintyY(PositionInfo *info);
float getCertaintyAngle(PositionInfo *info);

void release(PositionInfo **info);

#ifdef __cplusplus
}
#endif

#endif // CELLULO_LOCALIZATION_GLOBAL_H
