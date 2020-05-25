#ifndef YUV_H
#define YUV_H

#define WIDTH 1920
#define HEIGHT 1080

class YUV
{
public:
    int Load(char* filename);
    int Store(char* filename);

    int Y[HEIGHT][WIDTH];
    int U[HEIGHT][WIDTH];
    int V[HEIGHT][WIDTH];
};

class RGB
{
public:
    RGB(const YUV& yuv);

    int R[HEIGHT][WIDTH];
    int G[HEIGHT][WIDTH];
    int B[HEIGHT][WIDTH];
};

#endif