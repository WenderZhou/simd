#include "pic.h"
#include "stdio.h"

YUV::YUV()
{
    Y = new uint8*[HEIGHT];
    U = new uint8*[HEIGHT];
    V = new uint8*[HEIGHT];

    for(int i = 0; i < HEIGHT; i++)
    {
        Y[i] = new uint8[WIDTH];
        U[i] = new uint8[WIDTH];
        V[i] = new uint8[WIDTH];
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

RGB::RGB()
{
    R = new uint8*[HEIGHT];
    G = new uint8*[HEIGHT];
    B = new uint8*[HEIGHT];

    for(int i = 0; i < HEIGHT; i++)
    {
        R[i] = new uint8[WIDTH];
        G[i] = new uint8[WIDTH];
        B[i] = new uint8[WIDTH];
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