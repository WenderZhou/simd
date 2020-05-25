#include "pic.h"
#include "stdio.h"

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
}

RGB::RGB(const YUV& yuv)
{

}