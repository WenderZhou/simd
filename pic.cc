#include "pic.h"
#include "stdio.h"

YUV::YUV()
{
    Y = new int*[HEIGHT];
    U = new int*[HEIGHT];
    V = new int*[HEIGHT];

    for(int i = 0; i < HEIGHT; i++)
    {
        Y[i] = new int[WIDTH];
        U[i] = new int[WIDTH];
        V[i] = new int[WIDTH];
    }
}

void YUV::Free()
{
    for(int i = 0; i < HEIGHT; i++)
    {
        delete []Y[i];
        delete []U[i];
        delete []V[i];
    }

    delete []Y;
    delete []U;
    delete []V;
}

int YUV::Load(char* filename)
{
    FILE* file = fopen(filename, "rb");
    if(file == NULL)
    {
        printf("Load picture failed: can not open file!\n");
        return -1;
    }

    for(int i = 0; i < HEIGHT; i++)
        for(int j = 0; j < WIDTH; j++)
            fread(&Y[i][j], sizeof(char), 1, file);

    int temp;

    for(int i = 0; i < HEIGHT; i += 2)
        for(int j = 0; j < WIDTH; j += 2)
        {
            fread(&temp, sizeof(char), 1, file);
            U[i][j] = temp;
            U[i + 1][j] = temp;
            U[i][j + 1] = temp;
            U[i + 1][j + 1] = temp;
        }

    for(int i = 0; i < HEIGHT; i += 2)
        for(int j = 0; j < WIDTH; j += 2)
        {
            fread(&temp, sizeof(char), 1, file);
            V[i][j] = temp;
            V[i + 1][j] = temp;
            V[i][j + 1] = temp;
            V[i + 1][j + 1] = temp;
        }

    fclose(file);
    return 0;
}

int YUV::Store(char* filename)
{
    FILE* file = fopen(filename, "wb+");
    if(file == NULL)
    {
        printf("Store picture failed: can not open file!\n");
        return -1;
    }

    for(int i = 0; i < HEIGHT; i++)
        for(int j = 0; j < WIDTH; j++)
            fwrite(&Y[i][j],sizeof(char),1,file);
    for(int i = 0; i < HEIGHT; i += 2)
        for(int j = 0; j < WIDTH; j += 2)
            fwrite(&U[i][j],sizeof(char),1,file);
    for(int i = 0; i < HEIGHT; i += 2)
        for(int j = 0; j < WIDTH; j += 2)
            fwrite(&V[i][j],sizeof(char),1,file);

    fclose(file);
    return 0;
}

static int clamp(int v)
{
    if(v > 255)
        return 255;
    if(v < 0)
        return 0;
    return v;
}

RGB::RGB()
{
    R = new int*[HEIGHT];
    G = new int*[HEIGHT];
    B = new int*[HEIGHT];

    for(int i = 0; i < HEIGHT; i++)
    {
        R[i] = new int[WIDTH];
        G[i] = new int[WIDTH];
        B[i] = new int[WIDTH];
    }
}

void RGB::Free()
{
    for(int i = 0; i < HEIGHT; i++)
    {
        delete []R[i];
        delete []G[i];
        delete []B[i];
    }

    delete []R;
    delete []G;
    delete []B;
}

void RGB::YUV2RGB(const YUV& yuv)
{
    for(int i = 0; i < HEIGHT; i++)
        for(int j = 0; j < WIDTH; j++)
        {
            int y = yuv.Y[i][j] - 16;
            int u = yuv.U[i][j] - 128;
            int v = yuv.V[i][j] - 128;

            R[i][j] = clamp((298 * y + 409 * v) >> 8);
            G[i][j] = clamp((298 * y - 100 * u - 208 * v) >> 8);
            B[i][j] = clamp((298 * y + 516 * u) >> 8); 
        }
}

void RGB::RGB2YUV(YUV& yuv)
{
    for(int i = 0; i < HEIGHT; i++)
        for(int j = 0; j < WIDTH; j++)
        {
            int r = R[i][j];
            int g = G[i][j];
            int b = B[i][j];

            yuv.Y[i][j] = clamp(((66 * r + 129 * g + 25 * b) >> 8) + 16);
            yuv.U[i][j] = clamp(((-38 * r - 74 * g + 112 * b) >> 8) + 128);
            yuv.V[i][j] = clamp(((112 * r - 94 * g - 18 * b) >> 8) + 128);
        }
}