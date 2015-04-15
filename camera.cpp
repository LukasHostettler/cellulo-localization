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
    rotationDecoderReset(&rotDec);
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



void drawDirections(Mat &I,IntPoint *pointArray, int n,int mul,IntPoint direction,Scalar color=Scalar(0,0,0)){
    Point dir(direction.x/mul,direction.y/mul);
    for(IntPoint * point=pointArray;point<pointArray+n;point++ ){
        Point p1(point->x/mul,point->y/mul);
        Point p2=p1+dir;
        line(I,p1,p2,color);

    }
}


Point transformFromIdx(int col, int row,IntPoint *means, IntPoint origin,int subdivision){
    int px=(origin.x+row*means[0].x+col*means[1].x )/subdivision;
    int py=(origin.y+row*means[0].y+col*means[1].y )/subdivision;
    return Point(px,py);
}
void printIntGrid(Mat &I,IntGrid g,IntPoint *means,IntPoint origin,int subdivision,int min=0,int max=1024*256){
    int row, col;
    circle(I,Point(origin.x/subdivision,origin.y/subdivision),200,Scalar(0,0,0),3);
    for(row=0;row<g.numRows;++row){
        for(col=0;col<g.numCols;++col){
            Point center=transformFromIdx(col,row,means,origin,subdivision);
            int colorVal=(255*g.array[row][col])/ (max-min);
            circle(I,center,5,Scalar(0,colorVal,255-colorVal),-1);
        }
    }
}
void printSquare(Mat &I,int row, int col, IntPoint * means,IntPoint origin, int subdivision,Scalar color=Scalar(255,0,255)){
    Point pt0=transformFromIdx(col,row,means,origin,subdivision);
    Point pt1=transformFromIdx(col+8,row,means,origin,subdivision);
    Point pt2=transformFromIdx(col+8,row+8,means,origin,subdivision);
    Point pt3=transformFromIdx(col,row+8,means,origin,subdivision);

    circle(I,pt0,10,color);
    line(I,pt0,pt1,color);
    line(I,pt1,pt2,color);
    line(I,pt2,pt3,color);
    line(I,pt3,pt0,color);
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
        thresholdValue=imgMean(0)-10;
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
    if(correctMeanLength(pointArray,means,10,segList.numElements,&cross)&&segList.numElements>64){
        //copy pointArray to gridArray...
        IntPoint * gridArray = (IntPoint *)malloc(segList.numElements*sizeof(IntPoint));
        for(i=0;i<segList.numElements;i++){
            gridArray[i]=pointArray[i];
        }
        //print initial middle circle
        DotInformation dotInfo=dotInfoInit(gridArray,means,10,segList.numElements,cross);
        circle(I,Point(dotInfo.gridOrigin.x/subdivision,dotInfo.gridOrigin.y/subdivision),7,Scalar(255,255,255),3);

        ProbabilityGrids probGrids=makeProbabilityGrids(dotInfo);
        printIntGrid(I,probGrids.maxProb,means,dotInfo.gridOrigin,subdivision);

        if(probGrids.maxProb.numCols>=8&&probGrids.maxProb.numRows>=8){
            int nCol=0,nRow=0;
            intGridFindBestNxN(probGrids.maxProb,&nCol,&nRow,8);
            printSquare(I,nRow,nCol,means,dotInfo.gridOrigin,subdivision);
            //find orientation:
            int a=forwardProbability(probGrids.prob2,nRow,nCol);
            int b=downwardProbability(probGrids.prob1,nRow,nCol);
            cout<<"Results: a>0:"<< int(a>0) <<" b>0: "<<int(b>0)<<" a: "<<a<<" b: "<<b<<endl;
            rotationDecoderUpdate(&rotDec,-b,-a);
            cout<<rotationDecoderUpdateMeans(&rotDec,means)<<endl;
        }
        dotInfoFree(&dotInfo);
        drawLines(I,pointArray,edges,numEdges,subdivision,Scalar(0,255,255));


        probabilityGridsFree(&probGrids);


        free(gridArray);
    }
    else{
            cout<<"above ground"<<endl;
            rotationDecoderDiminuish(&rotDec);
    }


    free(pointArray);
    free(neigh);
    free(edges);
    imgSegListFree(&segList);
    return ans;
}

