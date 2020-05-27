#include "solution.h"

void Solution::Transform(const YUV& src, YUV& dst, int alpha)
{
    RGB oldRGB, newRGB;
    YUV2RGB(src, oldRGB);
    AlphaBlend(oldRGB, newRGB, alpha);
    RGB2YUV(newRGB, dst);
    oldRGB.Free();
    newRGB.Free();
}

static uint8 clamp(int v)
{
    if(v > 255)
        return 255;
    if(v < 0)
        return 0;
    return (uint8)v;
}

void Basic::YUV2RGB(const YUV& yuv, RGB& rgb)
{
    for(int i = 0; i < SIZE; i++)
    {
        int y = yuv.Y[i] - 16;
        int u = yuv.U[i] - 128;
        int v = yuv.V[i] - 128;

        rgb.R[i] = clamp((298 * y + 409 * v) >> 8);
        rgb.G[i] = clamp((298 * y - 100 * u - 208 * v) >> 8);
        rgb.B[i] = clamp((298 * y + 516 * u) >> 8); 
    }
}

void Basic::AlphaBlend(const RGB& src, RGB& dst, int alpha)
{
    for(int i = 0; i < SIZE; i++)
    {
        dst.R[i] = (alpha * src.R[i]) >> 8;
        dst.G[i] = (alpha * src.G[i]) >> 8;
        dst.B[i] = (alpha * src.B[i]) >> 8;
    }
}

void Basic::RGB2YUV(const RGB& rgb, YUV& yuv)
{
    for(int i = 0; i < SIZE; i++)
    {
        int r = rgb.R[i];
        int g = rgb.G[i];
        int b = rgb.B[i];

        yuv.Y[i] = clamp(((66 * r + 129 * g + 25 * b) >> 8) + 16);
        yuv.U[i] = clamp(((-38 * r - 74 * g + 112 * b) >> 8) + 128);
        yuv.V[i] = clamp(((112 * r - 94 * g - 18 * b) >> 8) + 128);
    }
}

void MMX::YUV2RGB(const YUV& yuv, RGB& rgb)
{
    __m64 c1 = c(16);
    __m64 c2 = c(128);
    for(int i = 0; i < SIZE; i += 4)
    {
        __m64 y = sub(pack(yuv.Y + i), c1);
        __m64 u = sub(pack(yuv.U + i), c2);
        __m64 v = sub(pack(yuv.V + i), c2);

        unpack(srai(add(mul(y,c(298)),mul(v,c(409))),8),rgb.R + i);
        unpack(srai(sub(sub(mul(y,c(298)),mul(u,c(100))),mul(v,c(208))),8),rgb.G + i);
        unpack(srai(add(mul(y,c(298)),mul(u,c(516))),8),rgb.B + i);
    }
    _m_empty();
}

void MMX::AlphaBlend(const RGB& src, RGB& dst, int alpha)
{
    __m64 A = c((short)alpha);
    for(int i = 0; i < SIZE; i += 4)
    {
        __m64 r = pack(src.R + i);
        __m64 g = pack(src.G + i);
        __m64 b = pack(src.B + i);

        unpack(srai(mul(r,A),8),dst.R + i);
        unpack(srai(mul(g,A),8),dst.G + i);
        unpack(srai(mul(b,A),8),dst.B + i);
    }
    _m_empty();
}

void MMX::RGB2YUV(const RGB& rgb, YUV& yuv)
{
    for(int i = 0; i < SIZE; i += 4)
    {
        __m64 r = pack(rgb.R + i);
        __m64 g = pack(rgb.G + i);
        __m64 b = pack(rgb.B + i);

        unpack(add(srai(add(add(mul(r,c(66)),mul(g,c(129))),mul(b,c(25))),8),c(16)),yuv.Y + i);
        unpack(add(srai(sub(sub(mul(b,c(112)),mul(r,c(38))),mul(g,c(74))),8),c(128)),yuv.U + i);
        unpack(add(srai(sub(sub(mul(r,c(112)),mul(g,c(94))),mul(b,c(18))),8),c(128)),yuv.V + i);
    }
    _m_empty();
}