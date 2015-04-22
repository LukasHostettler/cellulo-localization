#ifndef CAMERA_H
#define CAMERA_H
#include <opencv2/opencv.hpp>


#include <cellulo_localization.h>

#include "../src/list.h"
#include "../src/imgsegment.h"
#include "../src/imgproc.h"
#include "../src/neighbours.h"
#include "../src/grid.h"
#include "../src/decoder.h"



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
    IntPoint actualRobotPosition;
    IntPoint actualRobotOrientation;
    cv::VideoCapture cap;
    IntPoint *means;
    RotationDecoder rotDec;
};

#endif // CAMERA_H
