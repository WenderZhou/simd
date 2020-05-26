#include "solution.h"

void Basic::AlphaBlend(const RGB& src, YUV& dst, int alpha)
{
    RGB rgb;
    for(int i = 0; i < HEIGHT; i++)
        for(int j = 0; j < WIDTH; j++)
        {
            rgb.R[i][j] = (alpha * src.R[i][j]) >> 8;
            rgb.G[i][j] = (alpha * src.G[i][j]) >> 8;
            rgb.B[i][j] = (alpha * src.B[i][j]) >> 8;
        }

    rgb.RGB2YUV(dst);
}