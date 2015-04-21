
#include "localization.h"
#include "stdlib.h"



PositionInfo * positionInfoInit(){
    PositionInfo * ans=(PositionInfo *)malloc(sizeof(PositionInfo));
    initMeans(ans->means,1000);
    rotationDecoderReset(&ans->rotDec);
    ans->positionCertainty.x=0;
    ans->positionCertainty.y=0;
    ans->decoded=0;
    return ans;
}

PositionInfo * localize(PositionInfo * previousInfo,unsigned char * image, int rows, int cols){
    if(!previousInfo)
        previousInfo=positionInfoInit();
    int i, subdivision=128;
    int thresholdValue=100;//NEEDS CHANGE...
    imgSegList segList=segmentImage(image,rows,cols,thresholdValue);
    if(segList.numElements<6)
        return previousInfo;
    IntPoint * pointArray= sortPointArray(segList, subdivision);
    Neighbours * neigh=establishNeighbourhood(pointArray,segList.numElements);
    int numEdges;
    Edge * edges=extractGoodEdges(neigh,pointArray,segList.numElements, &numEdges);

    linkedKMeans(edges,previousInfo->means,numEdges);
    IntPoint cross=pointArray[segList.numElements/2];
    int findCrossSuccess=findCross(neigh,pointArray,segList.numElements,&cross);

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
            int nCol=0,nRow=0;
            intGridFindBestNxN(probGrids.maxProb,&nCol,&nRow,8);
            //find orientation:
            int a=forwardProbability(probGrids.prob1,nRow,nCol);
            int b=downwardProbability(probGrids.prob2,nRow,nCol);
            //cout<<"Results: a>0:"<< int(a>0) <<" b>0: "<<int(b>0)<<" a: "<<setw( 6 )<<a<<" b: "<<setw(6 )<<b<<endl;
            rotationDecoderUpdate(&previousInfo->rotDec,b,a);
            if(!rotationDecoderUpdateMeans(&previousInfo->rotDec,previousInfo->means)){
                IntPoint pos=decodePos(probGrids,nRow,nCol);
                previousInfo->position.x=pos.x-nRow;
                previousInfo->position.y=pos.y-nCol;
            }
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




