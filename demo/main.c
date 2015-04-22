#include <stdio.h>
#include <cellulo_localization.h>

#define STB_IMAGE_IMPLEMENTATION
#define STB_NO_HDR
#define STB_NO_LINEAR
#include "stb_image.h"

int main(void)
{
    printf("Hello World!\n");
    int rows;
    int cols;
    int bitDepth=1;
    char fileName[]="alongname";
    stbi_uc * image=stbi_load(fileName,&rows,&cols,&bitDepth,bitDepth);
    PositionInfo * loc= localize(0,image,rows,cols);
    printf("%f %f",getX(loc),getY(loc));

    return 0;
}

