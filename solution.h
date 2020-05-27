#ifndef SOLUTION_H
#define SOLUTION_H

#include "pic.h"
#include "mmintrin.h"

class Solution
{
public:
    virtual void AlphaBlend(const RGB& src, YUV& dst, int alpha) = 0;
};


class Basic : public Solution
{
public:
    void AlphaBlend(const RGB& src, YUV& dst, int alpha) override;
};


class MMX : public Solution
{
public:
    void AlphaBlend(const RGB& src, YUV& dst, int alpha) override;

    inline __m64 constant(short c)
    {
        return _mm_set1_pi16(c);
    }

    inline __m64 pack(uint8* src)
    {
        return _m_punpcklbw(_m_from_int(*(int*)src), (__m64)0LL);
    }

    inline void unpack(__m64 x, uint8* dst)
    {
        *(int*)dst = _m_to_int(_m_packuswb(x, (__m64)0LL));
    }

    inline __m64 add(__m64 a, __m64 b)
    {
        return _m_paddw(a, b);
    }

    inline __m64 sub(__m64 a, __m64 b)
    {
        return _m_psubw(a, b);
    }

    inline __m64 mul(__m64 a, __m64 b)
    {
        return _m_pmullw(a, b);
    }

    inline __m64 srai(__m64 x, int shift)
    {
        return _m_psrawi(x, shift);
    }

};

#endif