#ifndef YUV_H
#define YUV_H

#include "stdio.h"

#define WIDTH 1920
#define HEIGHT 1080
#define SIZE 2073600
#define SIZEDIV4 518400

typedef __uint8_t uint8;

class YUV
{
public:
    YUV();
    void Free();
    int Load(char* filename);
    int Store(char* filename);

    uint8** Y;
    uint8** U;
    uint8** V;
};

class RGB
{
public:
    RGB();
    void Free();

    uint8** R;
    uint8** G;
    uint8** B;
};

#endif