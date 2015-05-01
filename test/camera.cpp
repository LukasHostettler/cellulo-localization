#include <iostream>
#include <iomanip>
#include <math.h>
#include "camera.h"

//temporary include to visualize data.

#define STB_IMAGE_IMPLEMENTATION
#include "../demo/stb_image.h"


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
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,120.0);
    cap.set(CV_CAP_PROP_FRAME_WIDTH,160.0);
    //cap.set(CV_CAP_PROP_FORMAT,CV_8UC1);
    //cap.set(CV_CAP_PROP_CONTRAST,100);
    double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
    double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video
    cout << "Frame size : " << dWidth << " x " << dHeight << endl;
    means=new IntPoint[2]; //TO BE CHANGED
    initMeans(means,100);
    rotationDecoderReset(&rotDec);
}

Camera::~Camera()
{
    cap.release();
    delete means;
}

void Camera::drawRobot(int width, int height,int radius){
    static Scalar backGround(255,255,255);
    bool showNumber=false;
    int number;
    IntPoint knobPos[3]={{58,6400},{151,6400},{233,6400}};
    IntPoint numberPad[]={{148,7391},{34,7137},{132,7130},{233,7131},{34,7209},{132,7209},{233,7209},{38,7298},{126,7295},{218,7289}};

    switch(actualRobotPosition.y/1000){
    case 1:
        backGround=Scalar(255,255,255);//w
        break;
    case 2:
        backGround=Scalar(140,140,255);//r
        break;
    case 3:
        backGround=Scalar(140,255,255);//y

        break;
    case 4:
        backGround=Scalar(140,255,140);//g
        break;
    case 5:
        backGround=Scalar(255,140,140);//b
        break;
    case 6:

        for(int i=0;i<3;i++)
        if(intPointSquareDist(knobPos[i],actualRobotPosition)<30*30){
            backGround.val[i]=255*(atan2(means->x,means->y)+M_PI)/(2*M_PI);
            backGround.val[(i+1)%3]=backGround.val[i]/2;
            backGround.val[(i+2)%3]=backGround.val[i]/2;

        }

        break;
    case 7:
        for(int i=0;i<10;i++){
            if(intPointSquareDist(numberPad[i],actualRobotPosition)<30*30){
                showNumber=true;
                number=i;
            }

        }
        break;
        //default:
    }

    Mat I(height,width, CV_8UC3,backGround);
    Scalar black(0,0,0);

    if(actualRobotPosition.x>=0 &&actualRobotPosition.y>=0){
        Point pos(actualRobotPosition.x%1000,actualRobotPosition.y%1000);
        Point dir(means->y/80,means->x/80);
        circle(I,pos,radius,black);
        line(I,pos,pos+dir,black);
        if(showNumber){
            string text[]={"0","1","2","3","4","5","6","7","8","9"};
            int fontFace = CV_FONT_HERSHEY_PLAIN;
            double fontScale = 3;
            putText(I,text[number],Point(100,100),fontFace,fontScale,Scalar(0,0,0));
        }

    }
    imshow("Robot on sheet",I);



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

void printClassification(Mat &I,ProbabilityGrids grids,IntPoint *means,IntPoint origin,int subdivision){
    int row, col;
    string text;
    int fontFace = CV_FONT_HERSHEY_PLAIN;
    double fontScale = 0.5;
    origin.x-=300;
    origin.y+=300;
    //int colorVal=(255*g.array[row][col])/ (max-min);
    for(row=0;row<grids.maxProb.numRows;++row){
        for(col=0;col<grids.maxProb.numCols;++col){
            Point center=transformFromIdx(col,row,means,origin,subdivision);
            if(grids.prob1.array[row][col]<0 &&grids.prob2.array[row][col]<0)
                text="L";
            else if(grids.prob1.array[row][col]>0 &&grids.prob2.array[row][col]<0)
                text="D";
            else if(grids.prob1.array[row][col]<0 &&grids.prob2.array[row][col]>0)
                text="U";
            else if(grids.prob1.array[row][col]>0 &&grids.prob2.array[row][col]>0)
                text="R";
            putText(I,text,center,fontFace,fontScale,Scalar(0,0,0));
            //int fontFace, double fontScale
        }
    }
}

void printSquare(Mat &I,int row, int col, IntPoint * means,IntPoint origin, int subdivision,Scalar color=Scalar(255,0,255)){
    Point pt0=transformFromIdx(col,row,means,origin,subdivision);
    Point pt1=transformFromIdx(col+7,row,means,origin,subdivision);
    Point pt2=transformFromIdx(col+7,row+7,means,origin,subdivision);
    Point pt3=transformFromIdx(col,row+7,means,origin,subdivision);

    circle(I,pt0,10,color);
    line(I,pt0,pt1,color);
    line(I,pt1,pt2,color);
    line(I,pt2,pt3,color);
    line(I,pt3,pt0,color);
}

bool Camera::segment(Mat &I, double thresholdValue){

    drawRobot();
    //intGridTest();
    int i,subdivision=128;
    int chan=1;
    bool ans=getRawFrame(I);
    vector<Mat> channels;
    split(I,channels);

    if(thresholdValue==-1){
        Scalar imgMean=mean(channels[chan]);
        thresholdValue=imgMean(0)-20;
    }


    imgSegList segList=segmentImage(channels[chan].ptr(),channels[chan].rows,channels[chan].cols,thresholdValue);
    drawPoints(I,segList);
    if(segList.numElements<6)
        return false;
    IntPoint * pointArray= sortPointArray(segList, subdivision);
    Neighbours * neigh=establishNeighbourhood(pointArray,segList.numElements);
    int numEdges;
    Edge * edges=extractGoodEdges(neigh,pointArray,segList.numElements,&numEdges);

    drawLines(I,pointArray,neigh,segList.numElements,subdivision);
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

    drawDirections(I,pointArray,segList.numElements,subdivision,means[0]);
    drawDirections(I,pointArray,segList.numElements,subdivision,means[1],255.0);
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
        printClassification(I,probGrids,means,dotInfo.gridOrigin,subdivision);
        if(probGrids.maxProb.numCols>=8&&probGrids.maxProb.numRows>=8){
            int nCol=0,nRow=0;
            intGridFindBestNxN(probGrids.maxProb,&nCol,&nRow,8);
            printSquare(I,nRow,nCol,means,dotInfo.gridOrigin,subdivision);
            //find orientation:
            int a=forwardProbability(probGrids.prob1,nRow,nCol);
            int b=downwardProbability(probGrids.prob2,nRow,nCol);
            //cout<<"Results: a>0:"<< int(a>0) <<" b>0: "<<int(b>0)<<" a: "<<setw( 6 )<<a<<" b: "<<setw(6 )<<b<<endl;
            rotationDecoderUpdate(&rotDec,b,a);
            int rotate=rotationDecoderUpdateMeans(&rotDec,means);
            if(1){
                probabilityGridsTurn(&probGrids,rotate);
                int tmp;
                switch(rotate%4){
                case 3:
                    tmp=nCol;
                    nCol=nRow;
                    nRow=probGrids.prob1.numRows-8-tmp;
                    break;
                case 2:
                    nCol=probGrids.prob1.numCols-nCol-8;
                    nRow=probGrids.prob1.numRows-nRow-8;
                    break;
                case 1:
                    tmp=nCol;
                    nCol=probGrids.prob1.numCols-nRow-8;
                    nRow=tmp;
                    break;
                }

                IntPoint pos=decodePos(probGrids,nRow,nCol);
                cout<<"x: "<<pos.x-nRow<<" y: "<<pos.y-nCol;
                if(pos.x>=0)
                    actualRobotPosition.x=pos.x-nRow;//subdivision;
                if(pos.y>=0)
                    actualRobotPosition.y=pos.y-nCol;//subdivision;
                actualRobotHeadPosition.x=actualRobotPosition.x+means[0].x/subdivision;
                actualRobotHeadPosition.y=actualRobotPosition.y+means[0].y/subdivision;
                long deltaX=(I.rows*subdivision/2)-dotInfo.gridOrigin.x;
                long deltaY=(I.cols*subdivision/2)-dotInfo.gridOrigin.y;
                int a=-means[0].x;// Matrix elements of matrix
                int b=-means[1].x;// a b
                int c=-means[0].y;// c d
                int d=-means[1].y;//
                int determinant=a*d-c*b;
                cout<<" offset "<<100*(d*deltaX-b*deltaY)/determinant;
                cout<<" "<<100*(-c*deltaX+a*deltaY)/determinant<<endl;
                if(pos.x>=0){
                    actualRobotPosition.x*=100;
                    actualRobotPosition.x-=(10*(d*deltaX-b*deltaY))/determinant;
                    //actualRobotPosition.x-=(100*(-c*deltaX+a*deltaY))/determinant;
                    cout<<actualRobotPosition.x<<"   ";
                    actualRobotPosition.x/=100;
                }
                if(pos.y>=0){
                    actualRobotPosition.y*=100;
//                    actualRobotPosition.y-=(100*(d*deltaX-b*deltaY))/determinant;

                    actualRobotPosition.y-=(10*(-c*deltaX+a*deltaY))/determinant;
                    cout<<actualRobotPosition.y<<endl;
                    actualRobotPosition.y/=100;
                }


            }
            else
                cout<<"turned"<<endl;
        }
        dotInfoFree(&dotInfo);
        //drawLines(I,pointArray,edges,numEdges,subdivision,Scalar(0,255,255));


        probabilityGridsFree(&probGrids); 
        free(gridArray);
    }
    else{
            cout<<"above ground"<<endl;
            actualRobotPosition.x=-1000;
            actualRobotPosition.y=-1000;
            rotationDecoderDiminuish(&rotDec);
    }


    free(pointArray);
    free(neigh);
    free(edges);
    imgSegListFree(&segList);
    return ans;
}
