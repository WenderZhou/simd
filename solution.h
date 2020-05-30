#ifndef SOLUTION_H
#define SOLUTION_H

#include "pic.h"
#include "mmintrin.h"
#include "emmintrin.h"
#include "immintrin.h"

class Basic
{
public:
    void YUV2RGB(const YUV& yuv, RGB& rgb);
    void AlphaBlend(const RGB& src, RGB& dst, int alpha);
    void RGB2YUV(const RGB& rgb, YUV& yuv);

    void Transform(const YUV& src, YUV& dst, int alpha);
};


class MMX
{
public:
    void YUV2RGB(const YUV& yuv, RGB& rgb);
    void AlphaBlend(const RGB& src, RGB& dst, int alpha);
    void RGB2YUV(const RGB& rgb, YUV& yuv);

    void Transform(const YUV& src, YUV& dst, int alpha);

    typedef __m64 m64;
    
    inline m64 c(short c)
    {
        return _mm_set1_pi16(c);
    }

    inline m64 l(uint8* src)
    {
        return _mm_unpacklo_pi8(_m_from_int(*(int*)src), (m64)0LL);
    }

    inline void s(m64 x, uint8* dst)
    {
        *(int*)dst = _m_to_int(_mm_packs_pu16(x, (m64)0LL));
    }

    inline m64 add(m64 a, m64 b)
    {
        return _m_paddw(a, b);
    }

    inline m64 sub(m64 a, m64 b)
    {
        return _m_psubw(a, b);
    }

    inline m64 mul(m64 a, m64 b)
    {
        return _m_pmullw(a, b);
    }

    inline m64 srai(m64 x, int shift)
    {
        return _m_psrawi(x, shift);
    }

};

class SSE2
{
public:
    void YUV2RGB(const YUV& yuv, RGB& rgb);
    void AlphaBlend(const RGB& src, RGB& dst, int alpha);
    void RGB2YUV(const RGB& rgb, YUV& yuv);

    void Transform(const YUV& src, YUV& dst, int alpha);

    typedef __m128i m128;
    
    inline m128 c(short c)
    {
        return _mm_set1_epi16(c);
    }

    inline m128 l(uint8* src)
    {
        return _mm_unpacklo_epi8(_mm_cvtsi64x_si128(*(long long*)src), c(0));
    }

    inline void s(m128 x, uint8* dst)
    {
        *(long long*)dst = _mm_cvtsi128_si64(_mm_packus_epi16(x, c(0)));
    }

    inline m128 add(m128 a, m128 b)
    {
        return _mm_add_epi16(a, b);
    }

    inline m128 sub(m128 a, m128 b)
    {
        return _mm_sub_epi16(a, b);
    }

    inline m128 mul(m128 a, m128 b)
    {
        return _mm_mullo_epi16(a, b);
    }

    inline m128 srai(m128 x, int shift)
    {
        return _mm_srai_epi16(x, shift);
    }

};

class AVX
{
public:
    void YUV2RGB(const YUV& yuv, RGB& rgb);
    void AlphaBlend(const RGB& src, RGB& dst, int alpha);
    void RGB2YUV(const RGB& rgb, YUV& yuv);

    void Transform(const YUV& src, YUV& dst, int alpha);

    typedef __m256i m256;
    
    inline m256 c(short c)
    {
        return _mm256_set1_epi16(c);
    }

    inline m256 l(uint8* src)
    {
        __m128i temp = _mm_set_epi64x(*((long long*)(src) + 1),*(long long*)src);
        return _mm256_cvtepu8_epi16(temp);
    }

    inline void s(m256 x, uint8* dst)
    {
        m256 temp = _mm256_packus_epi16(x, c(0));
        __m128i p0 = _mm256_extractf128_si256(temp,0);
        __m128i p1 = _mm256_extractf128_si256(temp,1);
        _mm_storel_epi64((__m128i*)dst,p0);
        _mm_storel_epi64((__m128i*)(dst + 8),p1);
    }

    inline m256 add(m256 a, m256 b)
    {
        return _mm256_add_epi16(a, b);
    }

    inline m256 sub(m256 a, m256 b)
    {
        return _mm256_sub_epi16(a, b);
    }

    inline m256 mul(m256 a, m256 b)
    {
        return _mm256_mullo_epi16(a, b);
    }

    inline m256 srai(m256 x, int shift)
    {
        return _mm256_srai_epi16(x, shift);
    }

};

#endif