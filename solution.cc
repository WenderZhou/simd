#include "solution.h"

static uint8 clamp(int v)
{
    if(v > 255)
        return 255;
    if(v < 0)
        return 0;
    return (uint8)v;
}

// basic x86 ISA
void Basic::YUV2RGB(const YUV& yuv, RGB& rgb)
{
    for(int i = 0; i < HEIGHT; i++)
        for(int j = 0; j < WIDTH; j++)
        {
            int y = yuv.Y[i][j] - 16;
            int u = yuv.U[i][j] - 128;
            int v = yuv.V[i][j] - 128;

            rgb.R[i][j] = clamp((298 * y + 409 * v) >> 8);
            rgb.G[i][j] = clamp((298 * y - 100 * u - 208 * v) >> 8);
            rgb.B[i][j] = clamp((298 * y + 516 * u) >> 8); 
        }
}

void Basic::AlphaBlend(const RGB& src, RGB& dst, int alpha)
{
    for(int i = 0; i < HEIGHT; i++)
        for(int j = 0; j < WIDTH; j++)
        {
            dst.R[i][j] = (alpha * src.R[i][j]) >> 8;
            dst.G[i][j] = (alpha * src.G[i][j]) >> 8;
            dst.B[i][j] = (alpha * src.B[i][j]) >> 8;
        }
}

void Basic::RGB2YUV(const RGB& rgb, YUV& yuv)
{
    for(int i = 0; i < HEIGHT; i++)
        for(int j = 0; j < WIDTH; j++)
        {
            int r = rgb.R[i][j];
            int g = rgb.G[i][j];
            int b = rgb.B[i][j];

            yuv.Y[i][j] = clamp(((66 * r + 129 * g + 25 * b) >> 8) + 16);
            yuv.U[i][j] = clamp(((-38 * r - 74 * g + 112 * b) >> 8) + 128);
            yuv.V[i][j] = clamp(((112 * r - 94 * g - 18 * b) >> 8) + 128);
        }
}

void Basic::Transform(const YUV& src, YUV& dst, int alpha)
{
    RGB oldRGB, newRGB;
    YUV2RGB(src, oldRGB);
    AlphaBlend(oldRGB, newRGB, alpha);
    RGB2YUV(newRGB, dst);
    oldRGB.Free();
    newRGB.Free();
}

// MMX version
void MMX::YUV2RGB(const YUV& yuv, RGB& rgb)
{
    _mm_empty();
    for(int i = 0; i < HEIGHT; i++)
        for(int j = 0; j < WIDTH; j += 4)
        {
            m64 y = sub(l(&yuv.Y[i][j]), c(16));
            m64 u = sub(l(&yuv.U[i][j]), c(128));
            m64 v = sub(l(&yuv.V[i][j]), c(128));

            s(srai(add(mul(y,c(74)),mul(v,c(102))),6),&rgb.R[i][j]);
            s(srai(sub(sub(mul(y,c(74)),mul(u,c(25))),mul(v,c(52))),6),&rgb.G[i][j]);
            s(srai(add(mul(y,c(74)),mul(u,c(128))),6),&rgb.B[i][j]);
        }
    _mm_empty();
}

void MMX::AlphaBlend(const RGB& src, RGB& dst, int alpha)
{
    _mm_empty();
    m64 A = srai(c((short)alpha),1);
    for(int i = 0; i < HEIGHT; i++)
        for(int j = 0; j < WIDTH; j += 4)
        {
            m64 r = l(&src.R[i][j]);
            m64 g = l(&src.G[i][j]);
            m64 b = l(&src.B[i][j]);

            s(srai(mul(r,A),7),&dst.R[i][j]);
            s(srai(mul(g,A),7),&dst.G[i][j]);
            s(srai(mul(b,A),7),&dst.B[i][j]);
        }
    _mm_empty();
}

void MMX::RGB2YUV(const RGB& rgb, YUV& yuv)
{
    _mm_empty();
    for(int i = 0; i < HEIGHT; i++)
        for(int j = 0; j < WIDTH; j += 4)
        {
            m64 r = l(&rgb.R[i][j]);
            m64 g = l(&rgb.G[i][j]);
            m64 b = l(&rgb.B[i][j]);

            s(add(srai(add(add(mul(r,c(16)),mul(g,c(32))),mul(b,c(6))),6),c(16)),&yuv.Y[i][j]);
            s(add(srai(sub(sub(mul(b,c(28)),mul(r,c(9))),mul(g,c(19))),6),c(128)),&yuv.U[i][j]);
            s(add(srai(sub(sub(mul(r,c(28)),mul(g,c(23))),mul(b,c(4))),6),c(128)),&yuv.V[i][j]);
        }
    _mm_empty();
}

void MMX::Transform(const YUV& src, YUV& dst, int alpha)
{
    RGB oldRGB, newRGB;
    YUV2RGB(src, oldRGB);
    AlphaBlend(oldRGB, newRGB, alpha);
    RGB2YUV(newRGB, dst);
    oldRGB.Free();
    newRGB.Free();
}

// SSE2 version
void SSE2::YUV2RGB(const YUV& yuv, RGB& rgb)
{
    for(int i = 0; i < HEIGHT; i++)
        for(int j = 0; j < WIDTH; j += 8)
        {
            m128 y = sub(l(&yuv.Y[i][j]), c(16));
            m128 u = sub(l(&yuv.U[i][j]), c(128));
            m128 v = sub(l(&yuv.V[i][j]), c(128));

            s(srai(add(mul(y,c(74)),mul(v,c(102))),6),&rgb.R[i][j]);
            s(srai(sub(sub(mul(y,c(74)),mul(u,c(25))),mul(v,c(52))),6),&rgb.G[i][j]);
            s(srai(add(mul(y,c(74)),mul(u,c(128))),6),&rgb.B[i][j]);
        }
}

void SSE2::AlphaBlend(const RGB& src, RGB& dst, int alpha)
{
    m128 A = srai(c((short)alpha),1);
    for(int i = 0; i < HEIGHT; i++)
        for(int j = 0; j < WIDTH; j += 8)
        {
            m128 r = l(&src.R[i][j]);
            m128 g = l(&src.G[i][j]);
            m128 b = l(&src.B[i][j]);

            s(srai(mul(r,A),7),&dst.R[i][j]);
            s(srai(mul(g,A),7),&dst.G[i][j]);
            s(srai(mul(b,A),7),&dst.B[i][j]);
        }
}

void SSE2::RGB2YUV(const RGB& rgb, YUV& yuv)
{
    for(int i = 0; i < HEIGHT; i++)
        for(int j = 0; j < WIDTH; j += 8)
        {
            m128 r = l(&rgb.R[i][j]);
            m128 g = l(&rgb.G[i][j]);
            m128 b = l(&rgb.B[i][j]);

            s(add(srai(add(add(mul(r,c(16)),mul(g,c(32))),mul(b,c(6))),6),c(16)),&yuv.Y[i][j]);
            s(add(srai(sub(sub(mul(b,c(28)),mul(r,c(9))),mul(g,c(19))),6),c(128)),&yuv.U[i][j]);
            s(add(srai(sub(sub(mul(r,c(28)),mul(g,c(23))),mul(b,c(4))),6),c(128)),&yuv.V[i][j]);
        }
}

void SSE2::Transform(const YUV& src, YUV& dst, int alpha)
{
    RGB oldRGB, newRGB;
    YUV2RGB(src, oldRGB);
    AlphaBlend(oldRGB, newRGB, alpha);
    RGB2YUV(newRGB, dst);
    oldRGB.Free();
    newRGB.Free();
}

// AVX version
void AVX::YUV2RGB(const YUV& yuv, RGB& rgb)
{
    for(int i = 0; i < HEIGHT; i++)
        for(int j = 0; j < WIDTH; j += 16)
        {
            m256 y = sub(l(&yuv.Y[i][j]), c(16));
            m256 u = sub(l(&yuv.U[i][j]), c(128));
            m256 v = sub(l(&yuv.V[i][j]), c(128));

            s(srai(add(mul(y,c(74)),mul(v,c(102))),6),&rgb.R[i][j]);
            s(srai(sub(sub(mul(y,c(74)),mul(u,c(25))),mul(v,c(52))),6),&rgb.G[i][j]);
            s(srai(add(mul(y,c(74)),mul(u,c(128))),6),&rgb.B[i][j]);
        }
}

void AVX::AlphaBlend(const RGB& src, RGB& dst, int alpha)
{
    m256 A = srai(c((short)alpha),1);
    for(int i = 0; i < HEIGHT; i++)
        for(int j = 0; j < WIDTH; j += 16)
        {
            m256 r = l(&src.R[i][j]);
            m256 g = l(&src.G[i][j]);
            m256 b = l(&src.B[i][j]);

            s(srai(mul(r,A),7),&dst.R[i][j]);
            s(srai(mul(g,A),7),&dst.G[i][j]);
            s(srai(mul(b,A),7),&dst.B[i][j]);
        }
}

void AVX::RGB2YUV(const RGB& rgb, YUV& yuv)
{
    for(int i = 0; i < HEIGHT; i++)
        for(int j = 0; j < WIDTH; j += 16)
        {
            m256 r = l(&rgb.R[i][j]);
            m256 g = l(&rgb.G[i][j]);
            m256 b = l(&rgb.B[i][j]);

            s(add(srai(add(add(mul(r,c(16)),mul(g,c(32))),mul(b,c(6))),6),c(16)),&yuv.Y[i][j]);
            s(add(srai(sub(sub(mul(b,c(28)),mul(r,c(9))),mul(g,c(19))),6),c(128)),&yuv.U[i][j]);
            s(add(srai(sub(sub(mul(r,c(28)),mul(g,c(23))),mul(b,c(4))),6),c(128)),&yuv.V[i][j]);
        }
}

void AVX::Transform(const YUV& src, YUV& dst, int alpha)
{
    RGB oldRGB, newRGB;
    YUV2RGB(src, oldRGB);
    AlphaBlend(oldRGB, newRGB, alpha);
    RGB2YUV(newRGB, dst);
    oldRGB.Free();
    newRGB.Free();
}