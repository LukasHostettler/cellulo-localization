#ifndef IMGSEGMENT_H
#define IMGSEGMENT_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct _ImgSegment
{
    int n; //number of pixels added
    int x; //sum of all x values
    int y; //sum of all y values
}ImgSegment;

ImgSegment imgSegmentCreate(int x,int y, int n);
void imgSegmentAdd(ImgSegment * seg1,ImgSegment seg2);
int imgSegmentGetCenter(ImgSegment * seg,float *x, float *y);
void imgSegmentAddPixel(ImgSegment * seg,int x, int y);






#ifdef __cplusplus
}
#endif
#endif // IMGSEGMENT_H
