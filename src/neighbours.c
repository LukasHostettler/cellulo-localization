#include "neighbours.h"
#include <stdlib.h>
#include <limits.h>

#define NO_DIRECTION (-1)

void intPointAdd(IntPoint *p,int x,int y){
    p->x+=x;
    p->y+=y;
}
void intPointMul(IntPoint * toMultiply,int factor){
    toMultiply->x*=factor;
    toMultiply->y*=factor;
}

IntPoint * sortPointArray(imgSegList list, int multiplicator){
    int i,j;
    int num=list.numElements;
    IntPoint * pointArray=(IntPoint *)malloc(num*sizeof(IntPoint));
    //sort to list using insertion sort (almost order(n) )
    for(i=0;i<num;++i){
        j=i;
        IntPoint p;
        p.x=(list.list[i].x*multiplicator)/list.list[i].n;
        p.y=(list.list[j].y*multiplicator)/list.list[i].n;
        pointArray[i]= p;
        while(j>0 && pointArray[j].y < pointArray[j-1].y){
            IntPoint temp=pointArray[j];
            pointArray[j]=pointArray[j-1];
            pointArray[j-1]=temp;
            --j;
        }
    }
    return pointArray;
}

Neighbours * establishNeighbourhood(IntPoint * pointArray,int num){
    int i,j,k,maxIndex=0,forward,backward;
    int dx,dy;
    long squareDists[4],maxSquareDist;
    Neighbours *neigb= NULL;


    //Find Neighbours in sorted list..
    neigb=(Neighbours *)malloc(num*sizeof(Neighbours));

    for(i=0;i<num;++i){
        //init distances to intmax;
        for(j=0;j<4;++j){
            squareDists[j]=UINT_MAX;
        }
        maxSquareDist=UINT_MAX;

        forward=i+1;
        backward=i-1;

        while(forward<num || backward>=0){
            if(forward<num){
                dx=pointArray[i].x-pointArray[forward].x;
                dy=pointArray[i].y-pointArray[forward].y;
                long sqdist=dx*dx+dy*dy;
                if(sqdist < maxSquareDist){
                    squareDists[maxIndex]=sqdist;
                    neigb[i].neigbourIdx[maxIndex]=forward;
                    //update placement for next
                    maxIndex=0;
                    maxSquareDist=squareDists[0];
                    for(k=1;k<4;++k){
                        if(maxSquareDist<squareDists[k]){
                            maxIndex=k;
                            maxSquareDist=squareDists[k];
                        }
                    }
                }
                ++forward;
                if(dy*dy>=maxSquareDist) //stop forward search;
                    forward=num;
            }
            if(backward>=0){
                dx=pointArray[i].x-pointArray[backward].x;
                dy=pointArray[i].y-pointArray[backward].y;
                long sqdist=dx*dx+dy*dy;
                if(sqdist < maxSquareDist){
                    squareDists[maxIndex]=sqdist;
                    neigb[i].neigbourIdx[maxIndex]=backward;
                    //update placement for next
                    maxSquareDist=squareDists[0];
                    maxIndex=0;
                    for(k=1;k<4;++k){
                        if(maxSquareDist<squareDists[k]){
                            maxIndex=k;
                            maxSquareDist=squareDists[k];
                        }
                    }
                }
                --backward;
                if(dy*dy>=maxSquareDist) //stop backward search;
                    backward=-1;
            }
        }
    }
    return neigb;

}
Edge * extractGoodEdges(Neighbours * neigh, IntPoint *coords,int num, int *numEdges){
    int i,j,k,idx;
    int numEdge=0;
    Neighbours tmp;
    Edge *edges=malloc(sizeof(Edge)*num*2);
    for(i=0;i<num;++i) //for each point
    {
        for(j=0;j<4;++j)//for each neighbour of point
        {
            idx=neigh[i].neigbourIdx[j];
            if(idx>i){
                tmp=neigh[idx];
                for(k=0;k<4;++k) //for all neighbours of neighbour
                {
                    if(tmp.neigbourIdx[k]==i){

                        edges[numEdge].direction=NO_DIRECTION;
                        edges[numEdge].dx=coords[idx].x-coords[i].x;
                        edges[numEdge].dy=coords[idx].y-coords[i].y;
                        edges[numEdge].start=i;
                        edges[numEdge++].end=idx;

                        break;
                    }
                }
            }
        }
    }
    edges=realloc(edges,sizeof(Edge)*numEdge);
    *numEdges=numEdge;
    return edges;
}

int findCrossInnerLoop(int initialIdx,Neighbours *neigh){
    int i,j,secondGenCandidate,k=-1;
    int * firstGen=neigh[initialIdx].neigbourIdx;
    int * secondGenPointer;
    int secondGen[13]; //has an additional space in order to avoid segmentation faults
    for(i=0;i<4;++i){
        int foundFirst=0;
        secondGenPointer=neigh[firstGen[i]].neigbourIdx;
        for(j=0;j<4;++j){
            secondGenCandidate=secondGenPointer[j];
            if(secondGenCandidate==initialIdx)
                foundFirst=1;
            else
                secondGen[++k]=secondGenCandidate;
        }
        if(!foundFirst) //invalid as there is no returning to initial point
            return 0;
    }
    int doubleCounter=0;
    for(i=0;i<12;++i){
        for(j=i+1;j<12;++j)
            doubleCounter+= secondGen[i]==secondGen[j];
    }
    return doubleCounter==4;

}

int findCross(Neighbours * neigh, IntPoint *coords,int numNeigh,IntPoint * crossCoordinates){
    crossCoordinates->x=coords[numNeigh/2].x;
    crossCoordinates->y=coords[numNeigh/2].y;
    //return 1;
    int j,i=numNeigh/2,zigZag=1;
    while(i<numNeigh&&i>=0){
        if(findCrossInnerLoop(i,neigh)){
            crossCoordinates->x=4*coords[i].x;
            crossCoordinates->y=4*coords[i].y;

            for(j=0;j<4;j++){
                intPointAdd(crossCoordinates,coords[neigh[i].neigbourIdx[j]].x,coords[neigh[i].neigbourIdx[j]].y);
            }
            crossCoordinates->x/=8;
            crossCoordinates->y/=8;

            return 1;
        }
        i+= (i&1)?-zigZag:+zigZag;
        ++zigZag;
    }
    return 0;


}

void initMeans(IntPoint *means, int r){
    int i;
    means[0].x=r;
    means[0].y=0;
    means[1].x=0;
    means[1].y=r;
    for(i=2;i<4;++i){
        means[i].x=-means[i-2].x;
        means[i].y=-means[i-2].y;
    }
}


int quickPartition(int * list, int left,int right,int pivotIndex){
    int storeIndex,tmp,i;
    int pivotValue=list[pivotIndex];
    list[pivotIndex]=list[right];
    list[right]=pivotValue;
    storeIndex=left;
    for(i=left;i<right;++i){
        if(list[i]<pivotValue){
            tmp=list[storeIndex];
            list[storeIndex]=list[i];
            list[i]=tmp;
            ++storeIndex;
        }
    }
    tmp=list[right];
    list[right]=list[storeIndex];
    list[storeIndex]=tmp;
    return storeIndex;
}
//Attention! This function changes the order of the content of list
int quickSelect(int * list, int left,int right,int n){
    if (left==right)
        return list[left];

        int pivotIndex=(left+right)/2;
        pivotIndex=quickPartition(list,left,right,pivotIndex);
        if(n==pivotIndex)
            return list[n];
        else if (n < pivotIndex)
            return quickSelect(list, left, pivotIndex - 1, n);
        else
            return quickSelect(list, pivotIndex + 1, right, n);
}


int intListMedian(intList list){
    if(list.numElements)
        return quickSelect(list.list,0,list.numElements-1,list.numElements/2);
    else
        return 0;
}
void medianMeans(Edge *edges,IntPoint *means,int num){
    int dx,dy,dir,i;
    Edge * e;
    int squareLength[4];
    intList accumulators[4];
    for(i=0;i<4;++i){
        squareLength[i]=means[i].x*means[i].x+means[i].y*means[i].y;
        accumulators[i]=intListCreate();
    }
    //sort edges into acumulators
    for(e=edges;e<edges+num;++e){
        dir=e->direction;
        dx=e->dx-means[dir].x;
        dy=e->dy-means[dir].y;
        if((dx*dx+dy*dy)<0.25*squareLength[dir]){ //TODO: parameter to be determined
            intListPush(accumulators+(dir&1),(1-(dir&2))*e->dx);
            intListPush(accumulators+(dir&1)+2,(1-(dir&2))*e->dy);
        }
    }
    for(i=0;i<2;++i){
        //TODO assure that non-zero...
        means[i].x=intListMedian(accumulators[i]);
        means[i].y=intListMedian(accumulators[i+2]);
        means[i+2].x=-means[i].x;
        means[i+2].y=-means[i].y;
    }


    //free lists
    for(i=0;i<4;++i)
        intListFree(accumulators+i);

}
//works pretty nicely, length of the vectors are slighly offset
void linkedKMeans(Edge * edges,IntPoint *oldMean,int num){
    //Edges have a natural structure to be positive y.
    int i,j,itt,minIdx,maxNumItt=10;
    int minSqdist,tmpSqdist,dx,dy;
    int hasChanged=1; //in order to go through loop.
    IntPoint mean[4],tmpMean;
    int pointCounter[4];


    for(itt=0;itt<maxNumItt&&hasChanged;++itt){//while not stable solution
        for(j=0;j<4;j++){
            pointCounter[j]=0;
            mean[j].x=0;
            mean[j].y=0;
        }
        for(i=0;i<num;++i){//for all vectors
            //classify point
            minIdx=0;
            dx=edges[i].dx-oldMean[0].x;
            dy=edges[i].dy-oldMean[0].y;
            minSqdist=dx*dx+dy*dy;
            for(j=1;j<4;++j)//for all clusters
            {
                dx=edges[i].dx-oldMean[j].x;
                dy=edges[i].dy-oldMean[j].y;
                tmpSqdist=dx*dx+dy*dy;
                if(tmpSqdist<minSqdist){
                    minIdx=j;
                    minSqdist=tmpSqdist;
                }
            }
            //add classified Point to cluster
            mean[minIdx].x+=edges[i].dx;
            mean[minIdx].y+=edges[i].dy;
            ++pointCounter[minIdx];
            // save cluster info in edge
            edges[i].direction=minIdx;
            //could do the changeTest here?
            //+1 ItTeration if not moved,but control over numpoints reclassified
            //possibility to limit number of comp with clusters...
        }
        //move averages
        hasChanged=0;
        for(j=0;j<2;++j){
            int numPoint=pointCounter[j]+pointCounter[j+2];
            if(numPoint)
            {
                tmpMean.x=(mean[j].x-mean[j+2].x)/numPoint;
                tmpMean.y=(mean[j].y-mean[j+2].y)/numPoint;
            }
            else
            {
                //A divide by zero means the points are badly conditioned, or the means are off

                //Therefore we continue the algorithm with a reinit.

                tmpMean.x= (-1+2*j)*oldMean[1-j].y;
                tmpMean.y= (1-2*j)*oldMean[1-j].x;


            }
            if(tmpMean.x!=oldMean[j].x || tmpMean.y!=oldMean[j].y)
                hasChanged=1;
            oldMean[j]=tmpMean;
            oldMean[j+2].x=-tmpMean.x;
            oldMean[j+2].y=-tmpMean.y;
        }

    }
    medianMeans(edges,oldMean,num);
}

