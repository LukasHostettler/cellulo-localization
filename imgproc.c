#include "imgproc.h"
#include <stdlib.h>
#define NOSEGMENT 0xFFF
imgSegList segmentImage(unsigned char * imgPointer, int nRows,int nCols,unsigned char thresholdValue){
    intList equivalences=intListCreate();
    imgSegList segments=imgSegListCreate();


    int i,j;
    int n=0;
    unsigned char* p;


    //init oldline
    short * oldline=(short *)calloc(nCols,sizeof(short));
    short * newline=(short *)calloc(nCols,sizeof(short));
    short * tmpline =0;

    for (j=0;j<nCols;++j){
        oldline[j]=NOSEGMENT;
    }

    for( i = 0; i < nRows; ++i)
    {
        p = imgPointer+i*nCols; //point to next row

        //Special case for first pixel of row
        if(p[0]<thresholdValue){
            short up=oldline[0];
            if(up!=NOSEGMENT){
                newline[0]=up;
                imgSegmentAddPixel(segments.list+up,0,i);
            }
            else
            {
                ImgSegment tmp=imgSegmentCreate(0,i,1);
                imgSegListPush(&segments,tmp);
                newline[0]=n;
                intListPush(&equivalences,n++);
            }
        }
        else
            newline[0]=NOSEGMENT;


        for ( j = 1; j < nCols; ++j)
        {
            if(p[j]<thresholdValue){
                short last=newline[j-1];
                short up=oldline[j];
                if(up!=NOSEGMENT){
                    newline[j]=up;
                    imgSegmentAddPixel(segments.list+up,j,i);
                    if(last!=NOSEGMENT && last!=up){
                        int greater=last>up?last:up;
                        int smaller=last<up?last:up;
                        if (equivalences.list[greater]>smaller)
                            equivalences.list[greater]=smaller;
                    }
                }
                else if(last!=NOSEGMENT){
                    newline[j]=last;
                    imgSegmentAddPixel(segments.list+last,j,i);
                }
                else{
                    newline[j]=n;
                    intListPush(&equivalences,n++);
                    imgSegListPush(&segments,imgSegmentCreate(j,i,1));
                }
            }
            else
                newline[j]=NOSEGMENT;
        }
        tmpline=oldline;
        oldline=newline;
        newline=tmpline;
        tmpline=0;
    }
    free(oldline);
    free(newline);

    //Resolve Equivalences
    imgSegList completeSegments=imgSegListCreate();
    for(i=equivalences.numElements-1;i>=0;i--){
        j=equivalences.list[i];
        if(i!=j)
            imgSegmentAdd(segments.list+j,segments.list[i]);
        else if(segments.list[i].n>8){
            imgSegListPush(&completeSegments,segments.list[i]);
        }
    }
    imgSegListFree(&segments);
    intListFree(&equivalences);
    imgSegListShrink(&completeSegments);
    return completeSegments;
}


