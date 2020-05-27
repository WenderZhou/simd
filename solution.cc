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

void MMX::AlphaBlend(const RGB& src, YUV& dst, int alpha)
{
    RGB rgb;
    __m64 A = constant((short)alpha);
    for(int i = 0; i < HEIGHT; i++)
        for(int j = 0; j < WIDTH; j += 4)
        {
            __m64 r = pack((uint8*)&src.R[i][j]);
            __m64 g = pack((uint8*)&src.G[i][j]);
            __m64 b = pack((uint8*)&src.B[i][j]);

            unpack(srai(mul(r,A),8),(uint8*)&rgb.R[i][j]);
            unpack(srai(mul(g,A),8),(uint8*)&rgb.G[i][j]);
            unpack(srai(mul(b,A),8),(uint8*)&rgb.B[i][j]);
        }
    _m_empty();
    rgb.RGB2YUV(dst);
}