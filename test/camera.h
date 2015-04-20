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

    //!
    bool getRawFrame(Mat &frame);
    bool getRawFrame(Mat &frame,Rect roi);

    bool getChannel(Mat &channel,int channelNo=2);
    bool getThresholded(Mat &frame, double thresholdValue=128);

    bool segment(Mat &I, double thresholdValue=-1);



private:
    cv::VideoCapture cap;
    IntPoint *means;
    RotationDecoder rotDec;
};

#endif // CAMERA_H
