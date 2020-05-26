#ifndef YUV_H
#define YUV_H

#include "stdio.h"

#define WIDTH 1920
#define HEIGHT 1080

class YUV
{
public:
    YUV();
    void Free();
    int Load(char* filename);
    int Store(char* filename);

    int** Y;
    int** U;
    int** V;
};

class RGB
{
public:
    RGB();
    void Free();
    void YUV2RGB(const YUV& yuv);
    void RGB2YUV(YUV& yuv);

    int** R;
    int** G;
    int** B;
};

#endif