#include <stdio.h>
#include <cellulo_localization.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main(void)
{
    int i;
    printf("Localization Demo!\n");
    int rows;
    int cols;
    int bitDepth=1;
    char fileName[]="a.bmp";
    stbi_uc * image=stbi_load(fileName,&rows,&cols,&bitDepth,bitDepth);
    PositionInfo * loc= localize(0,image,rows,cols);
    for(i=0;i<10;i++){
        loc=localize(loc,image,rows,cols);
        if(isDecoded(loc))
            printf("%f %f\n",getX(loc),getY(loc));
    }


    return 0;
}

