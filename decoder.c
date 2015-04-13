#include <stdlib.h>
#include "decoder.h"

static char mainNumberSequence[]={0,0,0,0,0,0,1,0,0,1,1,1,1,1,0,1,0,0,1,0,0,0,0,1,1,1,0,1,1,1,0,0,1,0,1,0,1,0,0,0,1,0,1,1,0,1,1,0,0,1,1,0,1,0,1,1,1,1,0,0,0,1,1};
static char mainNumberSequenceList[]={6,5,13,23,40,80,160,63,126,254,245,237,214,165,76,148,37,71,136,15,30,63,120,239,221,189,118,230,206,150,46,85,172,85,166,70,143,23,46,95,183,109,221,183,103,206,158,54,111,216,176,95,189,124,245,231,199,141,28,52,100,196,133};
static int diffNumberSequence1[]={};
static int diffNumberSequence2[]={};
static int diffNumberSequence3[]={};
static int diffNumberSequence4[]={};
static int mainLookUp[]={-1,0,-1,1,-1,-1,-1,-1,2,-1,-1,-1,-1,19,-1,-1,-1,-1,3,-1,-1,37,-1,58,-1,-1,-1,-1,20,-1,-1,-1,16,-1,-1,-1,-1,-1,4,-1,-1,30,-1,-1,38,-1,-1,59,-1,-1,-1,-1,47,-1,-1,-1,-1,-1,21,-1,-1,7,-1,-1,-1,-1,17,-1,35,-1,-1,14,-1,-1,-1,-1,-1,-1,5,-1,33,-1,-1,31,-1,-1,-1,-1,-1,-1,39,-1,-1,51,-1,60,-1,-1,-1,-1,-1,44,-1,-1,-1,-1,48,41,-1,-1,-1,-1,-1,26,-1,-1,-1,-1,22,53,-1,-1,-1,-1,8,-1,-1,-1,62,-1,-1,-1,-1,-1,18,-1,-1,-1,36,57,-1,-1,-1,15,-1,-1,-1,-1,29,-1,-1,-1,-1,46,-1,-1,-1,-1,6,-1,-1,34,-1,13,-1,-1,-1,32,-1,-1,-1,-1,-1,-1,50,-1,-1,-1,43,-1,-1,40,-1,-1,25,-1,-1,52,-1,-1,-1,61,-1,-1,-1,-1,-1,56,-1,-1,-1,28,-1,-1,45,-1,-1,-1,-1,12,-1,-1,-1,-1,49,-1,42,-1,-1,24,-1,-1,-1,-1,-1,-1,55,-1,27,-1,-1,-1,11,-1,-1,-1,-1,23,-1,-1,54,-1,-1,10,-1,-1,-1,-1,-1,9,-1,-1,-1,-1,-1,-1};//256 long 2^8
//static inline int abs(int x)
//{
//    return (x + (x >> 31)) ^ (x >> 31);
//}

#define MEAN_OFFSET 180 /* USUAL OFFSET*/
#define PROB_MAX 1024L  /*Defines the Vertical Height at the peak  */
#define POWERSHIFT 21 /* SHIFT USED TO AVOID FLOATS */
#define FACTOR_B 3L*MEAN_OFFSET*MEAN_OFFSET
#define FACTOR_A (-(3*PROB_MAX<<POWERSHIFT)/(2*MEAN_OFFSET*FACTOR_B))
#define HORIZONTAL_MAX_SQUARED FACTOR_B
int prob1(int diagonalOffset){
    int ans = 0;
    if(diagonalOffset*diagonalOffset<HORIZONTAL_MAX_SQUARED){
        ans=(FACTOR_A*(diagonalOffset*diagonalOffset*diagonalOffset -FACTOR_B*diagonalOffset))>>POWERSHIFT;
    }
    return ans;
}


IntPoint * probabilities(int * offsetsx,int *offsetsy,int n){
    int i;
    int dx=0,dy=0;//debuggers
    IntPoint * ans=(IntPoint *)malloc(sizeof(IntPoint)*n);
    for(i=0;i<n;++i){
        dx+=abs(*offsetsx+ *offsetsy);
        dy+=abs(*offsetsx- *offsetsy);
        ans[i].x=prob1(*offsetsx+ *offsetsy);
        ans[i].y=prob1(*offsetsx- *offsetsy);
        ++offsetsx;
        ++offsetsy;
    }
    dx/=n;
    dy/=n;
    return ans;
}

int forwardProbability(IntGrid g){
    int i,j,lookup,votes=0;
    for(i=0;i<g.numCols;++i){
        lookup=0;
        for(j=0;j<g.numRows;++j){
            lookup|= (g.array[j][i]<512)<<j;
        }
        if(mainLookUp[lookup]>=0)
            ++votes;
        //reverse 8-bit lookup
        lookup = ((lookup * 0x0802LU & 0x22110LU) | (lookup * 0x8020LU & 0x88440LU)) * 0x10101LU >> 16;
        if(mainLookUp[(~lookup)&0xFF]>=0)
            --votes;
    }
    return (votes);
}



