#include <stdio.h>
#include "string.h"
#include "pic.h"
#include "solution.h"

// #include "mmintrin.h"

using namespace std;

int main()
{
    YUV YUV_src;
    YUV YUV_dst;

    Solution* basic = new Basic();

    YUV_src.Load("dem1.yuv");

    RGB rgb;
    rgb.YUV2RGB(YUV_src);

    basic->AlphaBlend(rgb,YUV_dst,128);

    YUV_dst.Store("result.yuv");

    return 0;
}