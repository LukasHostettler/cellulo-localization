#include <iostream>
#include "camera.h"


#define NOSEGMENT 0xFFF
using namespace std;


Camera::Camera(int camNo)
{
    cap=VideoCapture(camNo);
    if (!cap.isOpened())  // if not success, exit program
    {
        cout << "Cannot open the video cam" << endl;
        return;
    }
    //cap.set(CV_CAP_PROP_FPS,60.0); does not work for current cam
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,240.0);
    cap.set(CV_CAP_PROP_FRAME_WIDTH,320.0);
    //cap.set(CV_CAP_PROP_FORMAT,CV_8UC1);
    //cap.set(CV_CAP_PROP_CONTRAST,100);
    double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
    double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video
    cout << "Frame size : " << dWidth << " x " << dHeight << endl;
    means=new IntPoint[4];
    initMeans(means,100);
}

Camera::~Camera()
{
    cap.release();
    delete means;
}
bool Camera::getRawFrame(Mat &frame){
    bool ans=false;
    if(cap.isOpened()){
        ans=cap.read(frame);
    }
    return ans;
}

bool Camera::getRawFrame(Mat &frame,Rect roi){
    bool ans=false;
    Mat tmp;
    if(cap.isOpened()){
        ans= cap.read(tmp);
        frame = tmp(roi).clone();
    }
    return ans;
}

bool Camera::getChannel(Mat &frame,int channelNo){
    Mat rgb;
    vector<Mat> channels;
    bool ans=this->getRawFrame(rgb);
    if(ans){
        split(rgb,channels);
        frame=channels[channelNo];
    }
    return ans;
}

bool Camera::getThresholded(Mat &frame,double thresholdValue){
    Mat green;
    bool ans=this->getChannel(green,1);
    if(ans)
        threshold(green,frame,thresholdValue,256,THRESH_BINARY_INV);
    return ans;
}

void drawPoints(Mat &I,imgSegList segs,Scalar color=Scalar(0,0,0)){
    for(ImgSegment * i= segs.list;i<segs.list+segs.numElements;i++){
        circle(I,Point(i->x/i->n,i->y/i->n),5,color);
    }
}

void drawLines(Mat &I,IntPoint * pointArray,Neighbours * neigh, int n,int mul,Scalar color=Scalar(0,0,0)){
    for(int i=0;i<n;i++){
        Point p1(pointArray[i].x/mul,pointArray[i].y/mul);
        Neighbours nei=neigh[i];
        for(int j=0;j<4;j++){
            Point p2(pointArray[nei.neigbourIdx[j]].x/mul,pointArray[nei.neigbourIdx[j]].y/mul);
            line(I,p1,p2,color);
        }
    }
}
void drawLines(Mat &I, IntPoint *pointArray, Edge *edges, int n, int mul,Scalar color=Scalar(0,0,0)){
    for(Edge *e=edges;e<edges+n;e++ ){
        Point p1(pointArray[e->start].x/mul,pointArray[e->start].y/mul);
        Point p2(pointArray[e->end].x/mul,pointArray[e->end].y/mul);
        line(I,p1,p2,color);

    }
}
void drawLines(Mat &I, IntPoint *pointArray, Grid *grid, int mul, Scalar color=Scalar(0,0,0)){
    int i,j,idx;

    for(i=0;i<grid->numRows;++i){
        Point first,last;
        bool foundFirst=false,foundLast=false;
        for(j=0;j<grid->numCols;++j){
            idx=grid->pointIndex[i+j*grid->numRows];
            if(!foundFirst){
                if(idx>=0){
                    foundFirst=true;
                    first= Point(pointArray[idx].x/mul,pointArray[idx].y/mul);
                }
            }
            else{
                if(idx>=0){
                    foundLast=true;
                    last=Point(pointArray[idx].x/mul,pointArray[idx].y/mul);
                }
            }
        }
        if(foundFirst&&foundLast)
            line(I,first,last,color);
    }

    for(j=0;j<grid->numCols;++j){
        Point first,last;
        bool foundFirst=false,foundLast=false;
        for(i=0;i<grid->numRows;++i){

            idx=grid->pointIndex[i+j*grid->numRows];
            if(!foundFirst){
                if(idx>=0){
                    foundFirst=true;
                    first= Point(pointArray[idx].x/mul,pointArray[idx].y/mul);
                }
            }
            else{
                if(idx>=0){
                    foundLast=true;
                    last=Point(pointArray[idx].x/mul,pointArray[idx].y/mul);
                }
            }
        }
        if(foundFirst&&foundLast)
            line(I,first,last,color);
    }
}

void drawDirections(Mat &I,IntPoint *pointArray, int n,int mul,IntPoint direction,Scalar color=Scalar(0,0,0)){
    Point dir(direction.x/mul,direction.y/mul);
    for(IntPoint * point=pointArray;point<pointArray+n;point++ ){
        Point p1(point->x/mul,point->y/mul);
        Point p2=p1+dir;
        line(I,p1,p2,color);

    }
}
void drawPoints(Mat &I,IntPoint *pointArray,Grid *g,IntGrid maxProb, int mult){
    int i,j,idx;

    for(i=0;i<g->numCols;++i){
        for(j=0;j<g->numRows;++j){
            idx=g->pointIndex[j*g->numCols+i];
            if(idx>=0){
                float p=255*maxProb.data[i+j*g->numCols]/1024.0;
                Scalar color(0,p,255-p);
                circle(I,Point(pointArray[idx].x/mult,pointArray[idx].y/mult),3,color,-1);
            }
        }
    }

}




bool Camera::segment(Mat &I, double thresholdValue){

    //intGridTest();
    int i,subdivision=128;
    int chan=1;
    bool ans=getRawFrame(I);
    vector<Mat> channels;
    split(I,channels);

    if(thresholdValue==-1){
        Scalar imgMean=mean(channels[chan]);
        thresholdValue=imgMean(0)-8;
    }

    imgSegList segList=segmentImage(channels[chan].ptr(),channels[chan].rows,channels[chan].cols,thresholdValue);
    drawPoints(I,segList);
    if(segList.numElements<6)
        return false;
    IntPoint * pointArray= sortPointArray(segList, subdivision);
    Neighbours * neigh=establishNeighbourhood(pointArray,segList.numElements);
    int numEdges;
    Edge * edges=extractGoodEdges(neigh,pointArray,segList.numElements,&numEdges);

    //drawLines(I,pointArray,neigh,segList.numElements,subdivision);
    //drawLines(I,pointArray,edges,numEdges,subdivision);

    //!setup next threshold

    linkedKMeans(edges,means,numEdges);

    i=0;
    IntPoint cross=pointArray[segList.numElements/2];
    int findCrossSuccess=(findCross(neigh,pointArray,segList.numElements,&cross));
    if(findCrossSuccess){
        circle(I,Point(cross.x/subdivision,cross.y/subdivision),7,Scalar(0,0,0),3);
    }

    //IntPoint tmp_mean={means[0].x*10,means[0].y*10};
    //drawDirections(I,pointArray,segList.numElements,subdivision,tmp_mean);

    //drawDirections(I,pointArray,segList.numElements,subdivision,means[0]);
    //drawDirections(I,pointArray,segList.numElements,subdivision,means[1],255.0);

    waitKey(10);
    if(correctMeanLength(pointArray,means,10,segList.numElements,&cross)){
        //copy pointArray to gridArray...
        IntPoint * gridArray = (IntPoint *)malloc(segList.numElements*sizeof(IntPoint));
        for(i=0;i<segList.numElements;i++){
            gridArray[i]=pointArray[i];
        }
        circle(I,Point(cross.x/subdivision,cross.y/subdivision),7,Scalar(255,255,255),3);
        Grid g=makeGrid(gridArray,means,10,segList.numElements,cross);
        drawLines(I,pointArray,edges,numEdges,subdivision,Scalar(0,255,255));
        drawLines(I, gridArray, &g,subdivision);



        IntGrid maxProb;
        ProbabilityGrid p=calculateProbabilities(g,&maxProb,0.14,10);
        drawPoints(I,gridArray,&g,maxProb,subdivision);


        if(g.numCols>8 &&g.numRows>8){
            ProbabilityGrid eightByEight=probabilityGridConstrainToBest(p,maxProb,8);
            IntGrid xMatrix=intGridMax(eightByEight.prob[0],eightByEight.prob[1]);
            IntGrid yMatrix=intGridMax(eightByEight.prob[1],eightByEight.prob[2]);
            int a=forwardProbability(xMatrix);
            int b=forwardProbability(yMatrix);
            intGridFree(&xMatrix);
            intGridFree(&yMatrix);
            probabilityGridFree(eightByEight);
            intGridFree(&maxProb);
        }
        else
            cout<<"not good condition";
        probabilityGridFree(p);


        free(gridArray);
    }
    else{
            cout<<"above ground\n";
    }


    free(pointArray);
    free(neigh);
    free(edges);
    imgSegListFree(&segList);
    return ans;
}

