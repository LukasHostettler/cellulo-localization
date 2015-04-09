#ifndef CAMERA_H
#define CAMERA_H
#include <opencv2/opencv.hpp>

#include "list.h"
#include "imgsegment.h"
#include "imgproc.h"
#include "neighbours.h"
#include "grid.h"
#include "decoder.h"



using namespace cv;

class Camera
{
public:
    Camera(int camNo=0);
    ~Camera();

    bool getRawFrame(Mat &frame);
    bool getRawFrame(Mat &frame,Rect roi);

    bool getChannel(Mat &channel,int channelNo=2);
    bool getThresholded(Mat &frame, double thresholdValue=128);
    //bool segment2(Mat &I, double thresholdValue);
    bool segment(Mat &I, double thresholdValue=-1);

    //bool segment(Mat &frame,ImgSegment *& segments,double thresholdValue=128);
    //bool segment(ImgSegment *& segments,double thresholdValue);

private:
    cv::VideoCapture cap;
    IntPoint *means;
};

#endif // CAMERA_H
