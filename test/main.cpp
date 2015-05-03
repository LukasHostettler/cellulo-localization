#include "camera.h"

int main(int argc, char *argv[])
{
    Camera cam=Camera(1);
    Mat frame;

    while(true){
//        if(cam.getRawFrame(frame)){

//            imshow("Color",frame);

//        }
        if(cam.segment(frame)){

            imshow("Segment",frame);
            frame.release();
        }
        if(waitKey(10)!=-1)
            break;

    }

    return 0;
}
