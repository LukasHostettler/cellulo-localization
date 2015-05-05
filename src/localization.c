#include <math.h>
#include "localization.h"
#include "stdlib.h"

//for col-major images...
void specialInitMeans(IntPoint * means, int val){
    means[0].x=0;
    means[0].y=val;
    means[1].x=val;
    means[1].y=0;
}

PositionInfo * positionInfoInit(){
    PositionInfo * ans=(PositionInfo *)malloc(sizeof(PositionInfo));
    //initMeans(ans->means,1000);
    specialInitMeans(ans->means,1000);
    rotationDecoderReset(&ans->rotDec);
    ans->positionCertainty.x=0;
    ans->positionCertainty.y=0;
    ans->decoded=0;
    return ans;
}
unsigned char imageMean(unsigned char *image,int rows, int cols){
    if(!image || rows<1 || cols<1)
        return 0;
    int i;
    long buffer=0;
    long numpixel=rows*cols;
    for(i=0;i<numpixel;++i){
        buffer+=image[i];
    }
    return buffer/numpixel;

}

PositionInfo * localize(PositionInfo * previousInfo,unsigned char * image, int rows, int cols){
    if(!image || rows<1 || cols<1)
        return 0;
    if(!previousInfo)
        previousInfo=positionInfoInit();
    previousInfo->decoded=0;
    int i, subdivision=128;
    int thresholdValue=imageMean(image,rows,cols)-20;
    imgSegList segList=segmentImage(image,cols,rows,thresholdValue);
    if(segList.numElements<6)
        return previousInfo;
    IntPoint * pointArray= sortPointArray(segList, subdivision);
    Neighbours * neigh=establishNeighbourhood(pointArray,segList.numElements);
    int numEdges;
    Edge * edges=extractGoodEdges(neigh,pointArray,segList.numElements, &numEdges);

    linkedKMeans(edges,previousInfo->means,numEdges);
    IntPoint cross=pointArray[segList.numElements/2];
    findCross(neigh,pointArray,segList.numElements,&cross);

    if(correctMeanLength(pointArray,previousInfo->means,10,segList.numElements,&cross)&&segList.numElements>64){
        //copy pointArray to gridArray...
        IntPoint * gridArray = (IntPoint *)malloc(segList.numElements*sizeof(IntPoint));
        for(i=0;i<segList.numElements;i++){
            gridArray[i]=pointArray[i];
        }
        //print initial middle circle
        DotInformation dotInfo=dotInfoInit(gridArray,previousInfo->means,10,segList.numElements,cross);


        ProbabilityGrids probGrids=makeProbabilityGrids(dotInfo);

        if(probGrids.maxProb.numCols>=8&&probGrids.maxProb.numRows>=8){


            IntPoint pictureCenter={(rows*subdivision/2),(cols*subdivision/2)};
            IntPoint decodedPos=probGridsDecode(&probGrids,pictureCenter,&previousInfo->rotDec, previousInfo->means,&dotInfo,100);
            //cout<<"x:"<<decodedPos.x<<" y:"<< decodedPos.y<<endl;
            if(decodedPos.x>=0)
                previousInfo->position.x=decodedPos.x;//100;
            if(decodedPos.y>=0)
                previousInfo->position.y=decodedPos.y;//100;
            previousInfo->decoded=1;


        }
        dotInfoFree(&dotInfo);


        probabilityGridsFree(&probGrids);
        free(gridArray);
    }
    else{
        rotationDecoderDiminuish(&previousInfo->rotDec);
    }


    free(pointArray);
    free(neigh);
    free(edges);
    imgSegListFree(&segList);

    return previousInfo;
}
void release(PositionInfo **info){
    if(info && *info){
        free(*info);
        *info=0;
    }
}

float getX(PositionInfo *info){
    if(info)
        return info->position.x;
    return 0;
}
float getY(PositionInfo *info){
    if(info)
        return info->position.y;
    return 0;
}
float getAngle(PositionInfo *info){
    if(info)
        return atan2(info->means[0].y,info->means[0].x);
    return 0;
}


float getCertaintyX(PositionInfo *info){
    if(info)
        return info->positionCertainty.x;
    return 0;
}
float getCertaintyY(PositionInfo *info){
    if(info)
        return info->positionCertainty.y;
    return 0;
}
float getCertaintyAngle(PositionInfo *info){
    //to be changed;
    if(info)
        return info->rotDec.x+info->rotDec.y;
    return 0;
}

int isDecoded(PositionInfo *info)
{
    if(info)
        return info->decoded;
    return 0;
}



