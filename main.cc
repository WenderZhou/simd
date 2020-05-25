#include <stdio.h>
#include "string.h"
#include "pic.h"

#include "mmintrin.h"

using namespace std;

int R[HEIGHT][WIDTH];
int G[HEIGHT][WIDTH];
int B[HEIGHT][WIDTH];

int R1[HEIGHT][WIDTH];
int G1[HEIGHT][WIDTH];
int B1[HEIGHT][WIDTH];

void clamp(int* v)
{
    if(*v < 0)
        *v = 0;
    if(*v > 255)
        *v = 255;
}

int main()
{
    YUV YUV_src;
    YUV YUV_dst;

    YUV_src.Load("dem1.yuv");

    for(int i = 0; i < HEIGHT; i++)
        for(int j = 0; j < WIDTH; j++)
        {
            int idx = ((i / 2) * (WIDTH / 2)) + (j / 2);
            R[i][j] = (int)(1.164383 * (Y[i][j] - 16) + 1.596027 * (V[idx] - 128));
            B[i][j] = (int)(1.164383 * (Y[i][j] - 16) + 2.017232 * (U[idx] - 128));
            G[i][j] = (int)(1.164383 * (Y[i][j] - 16) - 0.391762 * (U[idx] - 128) - 0.812968 * (V[idx] - 128));
            clamp(&R[i][j]);
            clamp(&B[i][j]);
            clamp(&G[i][j]);
        }

    // for(int A = 1; A < 255; A = A + 3)
    // {
    //     printf("A = %d\n", A);

    // for(int i = 0; i < HEIGHT; i++)
    //     for(int j = 0; j < WIDTH; j++)
    //     {
    //         R1[i][j] = R[i][j] * A / 256;
    //         G1[i][j] = G[i][j] * A / 256;
    //         B1[i][j] = B[i][j] * A / 256;
    //     }

    for(int i = 0; i < HEIGHT; i++)
        for(int j = 0; j < WIDTH; j++)
        {
            Y1[i][j] = (int)(0.256788 * R[i][j] + 0.504129 * G[i][j] + 0.097906 * B[i][j] + 16);
            clamp(&Y1[i][j]);
        }
    for(int k = 0; k < HEIGHT * WIDTH / 4; k++)
    {
        int i = k / (WIDTH / 2) * 2;
        int j = k % (WIDTH / 2) * 2;
        U1[k] = (int)(-0.148223 * R[i][j] - 0.290993 * G[i][j] + 0.439216 * B[i][j] + 128);
        V1[k] = (int)(0.439216 * R[i][j] - 0.367788 * G[i][j] - 0.071427 * B[i][j] + 128);
        clamp(&V1[k]);
        clamp(&U1[k]);
    }


}