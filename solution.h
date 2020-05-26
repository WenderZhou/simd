#ifndef SOLUTION_H
#define SOLUTION_H

#include "pic.h"

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

#endif