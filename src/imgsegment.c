#include "imgsegment.h"

ImgSegment imgSegmentCreate(int x,int y, int n)
{
    ImgSegment ans;
    ans.n=n;
    ans.x=x;
    ans.y=y;
    return ans;
}

void imgSegmentAdd(ImgSegment * seg1,ImgSegment seg2){
    seg1->n+=seg2.n;
    seg1->x+=seg2.x;
    seg1->y+=seg2.y;
}

int imgSegmentGetCenter(ImgSegment * seg,float *x, float *y){
    if(seg->n==0) //no division by 0
        return 0;
    *x=seg->x/(float)(seg->n);
    *y=seg->y/(float)(seg->n);
    return 1;
}

void imgSegmentAddPixel(ImgSegment * seg,int x, int y){
    seg->x+=x;
    seg->y+=y;
    seg->n++;
}


