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


#define HOR_MAX 44000
#define VER_MAX 31000
#define HOR_MIN 0
#define VER_MIN 0
#define HOR_BORDER 12000
#define VER_BORDER 12000

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

    if(ioctl(fd, UI_SET_EVBIT, EV_REL) < 0)
        die("error: ioctl");
    //    if(ioctl(fd, UI_SET_RELBIT, REL_X) < 0)
    //        die("error: ioctl");
    //    if(ioctl(fd, UI_SET_RELBIT, REL_Y) < 0)
    //        die("error: ioctl");
    if(ioctl(fd,UI_SET_RELBIT,REL_WHEEL)<0)
        die("error: ioctl");
    if(ioctl(fd,UI_SET_EVBIT, EV_ABS)<0)
        die("error: ioctl");
    if(ioctl(fd, UI_SET_ABSBIT,ABS_X)<0)
        die("error: ioctl");
    if(ioctl(fd,UI_SET_ABSBIT,ABS_Y)<0)
        die("error:ioctl");

    memset(&uidev, 0, sizeof(uidev));
    snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "cellulo_mouse");
    uidev.id.bustype = BUS_VIRTUAL;//BUS_USB;
    uidev.id.vendor  = 0x1;
    uidev.id.product = 0x1;
    uidev.id.version = 1;
    uidev.absmax[ABS_X]= HOR_MAX-HOR_BORDER;
    uidev.absmax[ABS_Y]= VER_MAX-VER_BORDER;
    uidev.absmin[ABS_X]= HOR_MIN+HOR_BORDER;
    uidev.absmin[ABS_Y]= VER_MIN+VER_BORDER;

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
bool setMouseAngleRel(int fd, float angle){
    struct input_event     ev;
    while(angle< -M_PI)
        angle+=M_PI;
    while(angle>M_PI)
        angle-=M_PI;
    if(angle*angle<(M_PI*0.0001*M_PI ))
        return false;
    int intAngle=(angle*100.0)+0.5;

    memset(&ev, 0, sizeof(struct input_event));
    ev.type = EV_REL;
    ev.code = REL_WHEEL;
    ev.value = intAngle;
    if(write(fd, &ev, sizeof(struct input_event)) < 0)
        die("error: write");
    memset(&ev, 0, sizeof(struct input_event));
    ev.type = EV_SYN;
    ev.code = 0;
    ev.value = 0;
    if(write(fd, &ev, sizeof(struct input_event)) < 0)
        die("error: write");
    return true;
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

    //usleep(1500);
    return;
}

int main(void){
    static float oldAngle;
    int i;
    int dx,dy;
    Mat frame;
    VideoCapture cap=initCam(1);
    getChannel(cap,frame,1);
    PositionInfo *posInfo=localize(NULL,frame.ptr(),frame.rows,frame.cols);
    int                    fd;
    fd=createMouse();
    sleep(1);
    for(i=1;i<4000;i++){
        getChannel(cap,frame,1);
        posInfo=localize(posInfo,frame.ptr(),frame.cols,frame.rows);
        if(getX(posInfo)>0 &&getY(posInfo)>0){
            dx=getX(posInfo);
            dy=getY(posInfo);
        }
        else {
           oldAngle=0;
           continue;
        }
        float newAngle=getAngle(posInfo);
        float diff=newAngle-oldAngle;
        if(oldAngle!=0){
            if(setMouseAngleRel(fd,diff))
                oldAngle =newAngle;
        }
        else
        {
            oldAngle=newAngle;
        }


        for(int j=0;j<1;j++)
            setMouseTo(fd,HOR_MAX-dy,VER_MIN+dx);
    }
    destroyMouse(fd);
    return 0;

}
