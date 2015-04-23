#ifdef __cplusplus
extern "C"
{
#endif
#include <stdlib.h>
#include <limits.h>
//needs replacing sometime...
#include <math.h>
#include <assert.h>
#include "grid.h"
#include "decoder.h"


typedef struct{
    int numCols;
    int numRows;
    int * indices;
}HelperGrid;



IntGrid intGridCreateEmpty(){
    IntGrid ans={0,0,0,0};
    return ans;
}
IntGrid intGridCreate(int numCol, int numRow){
    int i;
    IntGrid ans=intGridCreateEmpty();
    if(numCol>0 && numRow >0){
        ans.data=malloc(sizeof *ans.data *numCol*numRow);
        ans.array=malloc(sizeof *ans.array *numRow);
        for(i=0;i<numRow;i++){
            ans.array[i]=ans.data+i*numCol;
        }
        if(ans.data && ans.array){
            ans.numCols=numCol;
            ans.numRows=numRow;
        }
    }
    return ans;
}



IntGrid intGridCopy(IntGrid other){
    int i;
    IntGrid ans=intGridCreate(other.numCols,other.numRows);
    for(i=0;i<other.numCols*other.numRows;++i){ //could use memcopy...
        ans.data[i]=other.data[i];
    }
    return ans;
}
int intGridAddTo(IntGrid a,IntGrid b){
    int i;
    //check same dimensions
    int condition=a.numCols==b.numCols && a.numRows==b.numRows &&a.numCols>0 &&a.numRows>0;
    if(condition){
        for(i=0;i<a.numCols*a.numRows;++i){
            *(a.data+i)+=b.data[i];
        }
        return 1;
    }
    return 0;
}

IntGrid intGridAdd(IntGrid a,IntGrid b){
    int i;
    //check same dimensions
    int condition=a.numCols==b.numCols && a.numRows==b.numRows &&a.numCols>0 &&a.numRows>0;
    if(condition){
        IntGrid ans=intGridCreate(a.numCols,a.numRows);
        for(i=0;i<ans.numCols*ans.numRows;++i){
            ans.data[i]=a.data[i]+b.data[i];
        }
        return ans;
    }
    else{
        return intGridCreateEmpty();
    }
}
IntGrid intGridMax(IntGrid a,IntGrid b){
    int i,condition=a.numCols==b.numCols && a.numRows==b.numRows &&a.numCols>0 &&a.numRows>0;
    if(condition){
        IntGrid ans=intGridCreate(a.numCols,a.numRows);
        for(i=0;i<ans.numCols*ans.numRows;++i){
            ans.data[i]=a.data[i]>b.data[i]?a.data[i]:b.data[i];
        }
        return ans;
    }
    return intGridCreateEmpty();
}
void intGridApplyToEveryElement(IntGrid g,int(*f)(int)){
    int i;
    for(i=0;i<g.numCols*g.numRows;++i){
        g.data[i]=(*f)(g.data[i]);
    }
}

IntGrid intGridCopySub(IntGrid other,int topRow, int leftCol,int numRows, int numCols){
    int j;
    int *dataPtrIn,*dataPtrOut;
    assert(numCols>0&&numCols>0);
    assert(other.numCols>=(leftCol+numCols) &&other.numRows>=(topRow+numRows));
    IntGrid ans=intGridCreate(numCols,numRows);
    dataPtrOut=ans.data;
    for(j=topRow;j<topRow+numRows;++j){
        for(dataPtrIn=other.array[j]+leftCol;dataPtrIn<other.array[j]+leftCol+numCols;++dataPtrIn){
            *(dataPtrOut++)=*dataPtrIn;
        }
    }
    return ans;
}

void intGridFill(IntGrid toFill,int value){
    int i;
    for(i=0;i<toFill.numCols*toFill.numRows;++i){
        toFill.data[i]=value;
    }
}

//void divide(int *a,int b){
//    *a=b?*a/b:*a;
//}

void intGridOperation(IntGrid toModify,IntGrid modifier,void (*func)(int *,int)){
    int i;
    for(i=0;i<toModify.numCols*toModify.numRows;++i)
        (*func)(toModify.data+i,modifier.data[i]);
}
IntGrid intGridLocalSum(IntGrid g,int windowSize){
    int i,j,accumulator;
    int * dataPtrIn, *dataPtrOut;
    IntGrid tmp,ans;
    tmp=intGridCreate(g.numCols-windowSize+1,g.numRows);
    for(i=0;i<g.numRows;++i){
        dataPtrIn=g.array[i];
        dataPtrOut=tmp.array[i];
        accumulator=0;
        for(j=0;j<windowSize-1;++j){
            accumulator+=dataPtrIn[j];
        }
        for(j=0;j<tmp.numCols;j++){
            accumulator+=dataPtrIn[j+windowSize-1];
            *(dataPtrOut++)=accumulator;
            accumulator-=dataPtrIn[j];
        }
    }
    ans=intGridCreate(tmp.numCols,tmp.numRows-windowSize+1);
    for(i=0;i<tmp.numCols;i++){
        accumulator=0;
        for(j=0;j<windowSize-1;j++){
            accumulator+=tmp.array[j][i];
        }
        for(j=0;j<ans.numRows;j++){
            accumulator+=tmp.array[j+windowSize-1][i];
            ans.array[j][i]=accumulator;
            accumulator-=tmp.array[j][i];
        }
    }
    intGridFree(&tmp);
    return ans;

}
//returns the index of maximum.
int intGridMaximum(IntGrid g,int *idx){
    int max=INT_MIN;
    int maxIdx=0,i;
    for(i=0;i<g.numCols*g.numRows;i++){
        if(g.data[i]>max){
            max=g.data[i];
            maxIdx=i;
        }
    }
    if(idx)
        *idx=maxIdx;
    return max;
}
void intGridIndToSub(int ind, int * numCol,int * numRow){
    //idx=numCols*row+col;
    //int col=ind %(*numCol);
    //int row=(ind-col)/(*numCol);
    if(numCol && numRow){
        *numRow=*numRow;
        *numRow=ind/(*numCol);
        *numCol=ind-(*numRow)* (*numCol);

    }
}
void intGridFindBestNxN(IntGrid grid,int *nCol,int *nRow,int n){
    int maxIdx;
    //handle too small base grids
    assert(nCol&&nRow&& grid.numCols>=n && grid.numRows>=n);
    IntGrid tmp=intGridLocalSum(grid,n);
    (*nCol)=tmp.numCols;
    (*nRow)=tmp.numRows;
    intGridMaximum(tmp,&maxIdx);
    intGridIndToSub(maxIdx,nCol,nRow);
    //(*nCol)--;//+=n-1;
    //(*nRow)--;//+=n-1;
    intGridFree(&tmp);
}

void intGridFree(IntGrid * g){
    free(g->array);
    g->array=0;
    free(g->data);
    g->data=0;
    g->numCols=0;
    g->numRows=0;
}



int mulby2(int val){
    return val<<1;
}
void intGridTest(){
    int i;
    IntGrid a=intGridCreate(3,4);
    for(i=0;i<12;i++){
        a.data[i]=i;
    }
    IntGrid b=intGridCopy(a);
    b.array[1][0]=15;
    IntGrid c=intGridAdd(a,b);
    IntGrid e=intGridCopySub(a,0,1,3,3);
    intGridApplyToEveryElement(c,&mulby2);
    IntGrid d=intGridLocalSum(c,2);
    int max, maxIdx,rowN=d.numRows,colN=d.numCols;
    max=intGridMaximum(d,&maxIdx);
    intGridIndToSub(maxIdx,&colN,&rowN);
    intGridFree(&a);
    intGridFree(&b);
    intGridFree(&c);
    intGridFree(&d);
    intGridFree(&e);
}


int itterateSpiral(IntGrid g, int * x, int * y){
    int xdir=0;
    int ydir=1;
    int tmp;
    int counter=0;
    int nextCorner=0;
    int lengthChanger=1;
    int length=0;
    while(g.array[*x][*y]<0){

        if(counter==nextCorner){
            tmp=ydir;
            ydir=xdir;
            xdir=-tmp;
            if(lengthChanger){
                length+=1;
                lengthChanger=0;
            }
            else
                lengthChanger=1;
            nextCorner+=length;


        }
        counter++;
        *x+=xdir;
        *y+=ydir;
        if(*x>=g.numCols || *x<0 || *y<0 || *y>=g.numRows)
            return 0;
    }
    return 1;
}

void testSpiral(){
    IntGrid g=intGridCreate(5,5);
    int i;
    for(i=0;i<25;++i){
        g.data[i]=i-25;
    }
    int a=2;
    int b=2;
    itterateSpiral(g,&a,&b);
}

//! Given estimated means this function corrects their length
//! it also provides the point of origin close to the center of points and alligned with the grid
//! PointArray is the underlying PointCoordinates, n is the number of points
int correctMeanLength(IntPoint * pointArray,IntPoint *means,int power,int n,IntPoint * origin){
    //swap vectors if order is bad
    if(means[0].x*means[1].y-means[0].y*means[1].x<0){
        IntPoint tmp=means[1];
        means[1]=means[0];
        means[0]=tmp;
    }
    int i,j,xoffset,yoffset;
    int minGridX=INT_MAX,maxGridX=INT_MIN,minGridY=INT_MAX,maxGridY=INT_MIN;
    int a=means[0].x;// Matrix elements of matrix
    int b=means[1].x;// a b
    int c=means[0].y;// c d
    int d=means[1].y;//
    int determinant=a*d-c*b;
    if(!determinant) //skip if determinant==0;
        return 0;
    IntPoint * gridCoordinate=(IntPoint *)malloc(sizeof(IntPoint)*n);
    IntPoint * grdPointer=gridCoordinate+n;
    IntPoint * projectedArray=(IntPoint *)malloc(sizeof(IntPoint)*n)+n;
    IntPoint * pointPointer=pointArray+n;


    while(pointPointer!=pointArray){
        --pointPointer;
        --projectedArray;
        --grdPointer;
        //project
        xoffset=pointPointer->x-origin->x;
        yoffset=pointPointer->y-origin->y;
        projectedArray->x= (((long)d*xoffset-b*yoffset)<<power)/determinant;
        projectedArray->y= (((long)-c*xoffset+a*yoffset)<<power)/determinant;


        //assign coordinate
        grdPointer->x=((projectedArray->x)>>power)+(((projectedArray->x)>>(power-1))&1);
        if(grdPointer->x<minGridX)
            minGridX=grdPointer->x;
        if(grdPointer->x>maxGridX)
            maxGridX=grdPointer->x;
        grdPointer->y=(projectedArray->y>>power)+((projectedArray->y>>(power-1))&1);
        if(grdPointer->y<minGridY)
            minGridY=grdPointer->y;
        if(grdPointer->y>maxGridY)
            maxGridY=grdPointer->y;

    }
    if(maxGridX-minGridX>1000 || maxGridY-minGridY>1000)
        return 0;
    IntGrid indiceGrid=intGridCreate(maxGridX-minGridX+1,maxGridY-minGridY+1);
    intGridFill(indiceGrid,NOPOINT);
    for(i=0;i<n;++i){
        indiceGrid.array[gridCoordinate[i].y-minGridY][gridCoordinate[i].x-minGridX]=i;
    }
    int * accumulator=(int *) malloc(n*sizeof(int));
    int counter=0;
    for(i=0;i<indiceGrid.numRows;++i){
        for(j=0;j<indiceGrid.numCols-1;++j){
            int idx1=indiceGrid.array[i][j];
            int idx2=indiceGrid.array[i][j+1];
            if(idx1>=0 && idx2>=0)
                accumulator[counter++]=projectedArray[idx2].x-projectedArray[idx1].x;
        }
    }
    if(counter){
        intList tmp;
        tmp.list=accumulator;
        tmp.numElements=counter;
        int corr=intListMedian(tmp);
        means[0].x=(a*corr)>>power;
        means[0].y=(c*corr)>>power;
    }

    counter=0;
    for(i=0;i<indiceGrid.numRows-1;++i){
        for(j=0;j<indiceGrid.numCols;++j){
            int idx1=indiceGrid.array[i][j];
            int idx2=indiceGrid.array[i+1][j];
            if(idx1>=0 && idx2>=0)
                accumulator[counter++]=projectedArray[idx2].y-projectedArray[idx1].y;
        }
    }
    if(counter){
        intList tmp;
        tmp.list=accumulator;
        tmp.numElements=counter;
        int corr=intListMedian(tmp);
        means[1].x=(b*corr)>>power;
        means[1].y=(d*corr)>>power;
    }
    free(accumulator);

    int rowEst=indiceGrid.numRows/2;
    int colEst=indiceGrid.numCols/2;

    if(itterateSpiral(indiceGrid,&rowEst,&colEst)){
        *origin=pointArray[indiceGrid.array[rowEst][colEst]];
    }


    intGridFree(&indiceGrid);
    free(projectedArray);
    free(gridCoordinate);
    return 1;
}
IntPoint medianCorrectOffsets(DotInformation d){
    IntPoint median;
    intList tmp,cpy;
    int i;

    tmp.numAlloc=d.numElements;
    tmp.numElements=d.numElements;

    tmp.list=d.xoffsets;
    cpy=intListCopy(&tmp); //used in order to not destroy the order of the list
    median.x=intListMedian(cpy);
    intListFree(&cpy);

    tmp.list=d.yoffsets;
    cpy=intListCopy(&tmp);
    median.y=intListMedian(cpy);
    intListFree(&cpy);
    for(i=0;i<d.numElements;++i){
        d.xoffsets[i]-=median.x;
        d.yoffsets[i]-=median.y;
    }
    return median;
}


DotInformation dotInfoCreateEmpty(){
    DotInformation ans;
    ans.numElements=0;
    ans.gridMaxCols=0;
    ans.gridMaxRows=0;
    ans.xoffsets=0;
    ans.yoffsets=0;
    ans.gridCoordinate=0;
    return ans;
}
void dotInfoFree(DotInformation *d){
    if(d){
        d->numElements=0;
        d->gridMaxCols=0;
        d->gridMaxRows=0;
        if(d->xoffsets)
            free(d->xoffsets);
        if(d->yoffsets)
            free(d->yoffsets);
        if(d->gridCoordinate)
            free(d->gridCoordinate);
        d->xoffsets=0;
        d->yoffsets=0;
        d->gridCoordinate=0;
    }

}

DotInformation dotInfoInit(IntPoint * pointArray,IntPoint * means,int power,int numPoints,IntPoint origin){
    DotInformation ans=dotInfoCreateEmpty();
    int xoffset,yoffset,xproj,yproj,minGridX=INT_MAX,maxGridX=INT_MIN,minGridY=INT_MAX,maxGridY=INT_MIN;
    IntPoint medianOffset;
    //matrix [a b ;c d];
    int a=means[0].x;
    int b=means[1].x;
    int c=means[0].y;
    int d=means[1].y;
    int determinant=a*d-c*b;
    if(!determinant ||numPoints<1)
        return ans;


    ans.numElements=numPoints;
    ans.xoffsets=(int *)malloc(sizeof(int)*numPoints);
    ans.yoffsets=(int *)malloc(sizeof(int)*numPoints);
    ans.gridCoordinate=(IntPoint *)malloc(sizeof(IntPoint)*numPoints);

    IntPoint * pointOrig=pointArray; //pointer trough pointArray;
    int * xoffsets=ans.xoffsets; // pointer through offsetArray
    int * yoffsets=ans.yoffsets; // pointer through offsetArray
    IntPoint * gridCoordinate=ans.gridCoordinate; //pointer trough gridCoordiates
    while(pointOrig<pointArray+numPoints){
        //project
        xoffset=pointOrig->x-origin.x;
        yoffset=pointOrig->y-origin.y;
        xproj= ((long)(d*xoffset-b*yoffset)<<power)/determinant;
        yproj= ((long)(-c*xoffset+a*yoffset)<<power)/determinant;

        //assign coordinate
        gridCoordinate->x=(xproj>>power)+((xproj>>(power-1))&1);
        gridCoordinate->y=(yproj>>power)+((yproj>>(power-1))&1);

        //update minMax
        if(gridCoordinate->x<minGridX)
            minGridX=gridCoordinate->x;
        if(gridCoordinate->x>maxGridX)
            maxGridX=gridCoordinate->x;
        if(gridCoordinate->y<minGridY)
            minGridY=gridCoordinate->y;
        if(gridCoordinate->y>maxGridY)
            maxGridY=gridCoordinate->y;

        //Debug set point to grid coordinate.
        pointOrig->x=a*gridCoordinate->x+b*gridCoordinate->y+origin.x;
        pointOrig->y=c*gridCoordinate->x+d*gridCoordinate->y+origin.y;


        //find offsets:
        *xoffsets=xproj-(gridCoordinate->x<<power);
        *yoffsets=yproj-(gridCoordinate->y<<power);

        //Increment pointers;
        ++pointOrig;
        ++xoffsets;
        ++yoffsets;
        ++gridCoordinate;
    }
    ans.gridMaxRows=maxGridX-minGridX+1;
    ans.gridMaxCols=maxGridY-minGridY+1;

    //Shift coordinates to be positive.
    for(gridCoordinate=ans.gridCoordinate;gridCoordinate<ans.gridCoordinate+ans.numElements;++gridCoordinate){
        intPointAdd(gridCoordinate,-minGridX,-minGridY);
    }

    medianOffset =medianCorrectOffsets(ans);

    while(pointOrig!=pointArray){
        pointOrig--;
        pointOrig->x+=(a*medianOffset.x+b*medianOffset.y)>>power;
        pointOrig->y+=(c*medianOffset.x+d*medianOffset.y)>>power;
    }
    ans.gridOrigin=origin;
    intPointAdd(&(ans.gridOrigin),a*minGridX +b* minGridY,c*minGridX+d*minGridY);
    intPointAdd(&(ans.gridOrigin),(a*medianOffset.x+b*medianOffset.y)>>power,(c*medianOffset.x+d*medianOffset.y)>>power);
    return ans;
}

ProbabilityGrids makeProbabilityGrids(DotInformation dotInfo){
    ProbabilityGrids ans;
    IntPoint *probs= probabilities(dotInfo.xoffsets, dotInfo.yoffsets,dotInfo.numElements);
    ans.prob1=intGridCreate(dotInfo.gridMaxCols,dotInfo.gridMaxRows);
    intGridFill(ans.prob1,0);
    ans.prob2=intGridCreate(dotInfo.gridMaxCols,dotInfo.gridMaxRows);
    intGridFill(ans.prob2,0);
    ans.maxProb=intGridCreate(dotInfo.gridMaxCols,dotInfo.gridMaxRows);
    intGridFill(ans.maxProb,0);

    int probProduct, i;
    for(i=0;i<dotInfo.numElements;++i){
        probProduct =abs(probs[i].x*probs[i].y);
        IntPoint coord=dotInfo.gridCoordinate[i];
        if(probProduct>=ans.maxProb.array[coord.x][coord.y]){
            ans.maxProb.array[coord.x][coord.y]=probProduct;
            ans.prob1.array[coord.x][coord.y]=probs[i].x;
            ans.prob2.array[coord.x][coord.y]=probs[i].y;
        }

    }
    free(probs);
    //temporary

    return ans;

}

void probabilityGridsFree(ProbabilityGrids * grid){
    intGridFree(&(grid->maxProb));
    intGridFree(&grid->prob1);
    intGridFree(&grid->prob2);
}


#ifdef __cplusplus
}
#endif