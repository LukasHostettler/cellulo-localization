#ifdef __cplusplus
extern "C"
{
#endif


#include <assert.h>


#include <stdlib.h>
#include <limits.h>
#include "intgrid.h"



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

#ifdef __cplusplus
}
#endif
