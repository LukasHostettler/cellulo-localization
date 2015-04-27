#include "camera.h"
#include <iostream>
#include <vector>

int main(int argc, char *argv[])
{
    //    QApplication a(argc, argv);
    //    MainWindow w;
    //    w.show();

    //    return a.exec();

    Camera cam=Camera(0);
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
