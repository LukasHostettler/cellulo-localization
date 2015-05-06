//Credit to Gregory Thiemonge uinput sample

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <linux/uinput.h>

#include <opencv2/opencv.hpp>
#include <cellulo_localization.h>
#include "../test/camera.h"

#define HOR_MAX 30000
#define VER_MAX 43500
#define die(str, args...) do { \
    perror(str); \
    exit(EXIT_FAILURE); \
    } while(0)

VideoCapture initCam(int camNo=0){
    VideoCapture cap=VideoCapture(camNo);
    if (!cap.isOpened())  // if not success, exit program
        die("Cannot open the cam");
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,120.0);
    cap.set(CV_CAP_PROP_FRAME_WIDTH,160.0);
    return cap;
}
bool getChannel(VideoCapture &cap,Mat &frame,int channelNo){
    frame.release();
    Mat rgb;
    bool ans=false;
    vector<Mat> channels;

    if(cap.isOpened()){
        ans=cap.read(rgb);
        if(ans){
            split(rgb,channels);
            frame=channels[channelNo];
            return true;
        }
    }


    return false;
}


int createMouse(){
    struct uinput_user_dev uidev;

    int fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if(fd < 0)
        die("error: open");

    if(ioctl(fd, UI_SET_EVBIT, EV_KEY) < 0)
        die("error: ioctl");
    if(ioctl(fd, UI_SET_KEYBIT, BTN_LEFT) < 0)
        die("error: ioctl");

    //    if(ioctl(fd, UI_SET_EVBIT, EV_REL) < 0)
    //        die("error: ioctl");
    //    if(ioctl(fd, UI_SET_RELBIT, REL_X) < 0)
    //        die("error: ioctl");
    //    if(ioctl(fd, UI_SET_RELBIT, REL_Y) < 0)
    //        die("error: ioctl");
    if(ioctl(fd,UI_SET_EVBIT, EV_ABS)<0)
        die("error: ioctl");
    if(ioctl(fd, UI_SET_ABSBIT,ABS_X)<0)
        die("error: ioctl");
    if(ioctl(fd,UI_SET_ABSBIT,ABS_Y)<0)
        die("error:ioctl");

    memset(&uidev, 0, sizeof(uidev));
    snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "uinput-sample");
    uidev.id.bustype = BUS_USB;
    uidev.id.vendor  = 0x1;
    uidev.id.product = 0x1;
    uidev.id.version = 1;
    uidev.absmax[ABS_X]= HOR_MAX;
    uidev.absmax[ABS_Y]= VER_MAX;
    uidev.absmin[ABS_X]=0;
    uidev.absmin[ABS_Y]=0;

    if(write(fd, &uidev, sizeof(uidev)) < 0)
        die("error: write");

    if(ioctl(fd, UI_DEV_CREATE) < 0)
        die("error: ioctl");

    sleep(2);
    return fd;
}
void destroyMouse(int fd){
    sleep(2);

    if(ioctl(fd, UI_DEV_DESTROY) < 0)
        die("error: ioctl");

    close(fd);
}

void setMouseTo(int fd,int x,int y){
    struct input_event     ev;
    memset(&ev, 0, sizeof(struct input_event));
    ev.type = EV_ABS;
    ev.code = ABS_X;
    ev.value = x;
    if(write(fd, &ev, sizeof(struct input_event)) < 0)
        die("error: write");

    memset(&ev, 0, sizeof(struct input_event));
    ev.type = EV_ABS;
    ev.code = ABS_Y;
    ev.value = y;
    if(write(fd, &ev, sizeof(struct input_event)) < 0)
        die("error: write");

    memset(&ev, 0, sizeof(struct input_event));
    ev.type = EV_SYN;
    ev.code = 0;
    ev.value = 0;
    if(write(fd, &ev, sizeof(struct input_event)) < 0)
        die("error: write");

    usleep(15000);
    return;
}

int main(void){
    int i;
    int dx,dy;
    Mat frame;
    VideoCapture cap=initCam(0);
    getChannel(cap,frame,1);
    PositionInfo *posInfo=localize(NULL,frame.ptr(),frame.rows,frame.cols);
    int                    fd;
    fd=createMouse();
    sleep(1);
    for(i=1;i<1000;i++){
        frame.release();
        getChannel(cap,frame,1);
        posInfo=localize(posInfo,frame.ptr(),frame.cols,frame.rows);
        if(getX(posInfo)>0)
            dx=getX(posInfo);
        if(getY(posInfo)>0)
            dy=getY(posInfo);
        for(int j=0;j<1;j++)
            setMouseTo(fd,HOR_MAX-dy,dx);
    }
    destroyMouse(fd);
    return 0;

}
