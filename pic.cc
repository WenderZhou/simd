#include "pic.h"
#include "stdio.h"

YUV::YUV()
{
    Y = new uint8[SIZE];
    U = new uint8[SIZE];
    V = new uint8[SIZE];
}

void YUV::Free()
{
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

    for(int i = 0; i < SIZE; i++)
        fread(Y + i, sizeof(char), 1, file);

    int temp;

    for(int i = 0; i < HEIGHT; i += 2)
        for(int j = 0; j < WIDTH; j += 2)
        {
            fread(&temp, sizeof(char), 1, file);
            U[i * WIDTH + j] = temp;
            U[(i + 1) * WIDTH + j] = temp;
            U[i * WIDTH + j + 1] = temp;
            U[(i + 1) * WIDTH + j + 1] = temp;
        }

    for(int i = 0; i < HEIGHT; i += 2)
        for(int j = 0; j < WIDTH; j += 2)
        {
            fread(&temp, sizeof(char), 1, file);
            V[i * WIDTH + j] = temp;
            V[(i + 1) * WIDTH + j] = temp;
            V[i * WIDTH + j + 1] = temp;
            V[(i + 1) * WIDTH + j + 1] = temp;
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

    for(int i = 0; i < SIZE; i++)
        fwrite(Y + i,sizeof(char),1,file);
    for(int i = 0; i < HEIGHT; i += 2)
        for(int j = 0; j < WIDTH; j += 2)
            fwrite(U + i * WIDTH + j,sizeof(char),1,file);
    for(int i = 0; i < HEIGHT; i += 2)
        for(int j = 0; j < WIDTH; j += 2)
            fwrite(V + i * WIDTH + j,sizeof(char),1,file);

    fclose(file);
    return 0;
}

RGB::RGB()
{
    R = new uint8[SIZE];
    G = new uint8[SIZE];
    B = new uint8[SIZE];
}

void RGB::Free()
{
    delete []R;
    delete []G;
    delete []B;
}