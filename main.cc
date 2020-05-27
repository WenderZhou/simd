#include <stdio.h>
#include "string.h"
#include "pic.h"
#include "solution.h"
#include "time.h"

using namespace std;

int main()
{
    YUV YUV_src;
    YUV YUV_dst;

    Solution* basic = new Basic();
    Solution* mmx = new MMX();

    YUV_src.Load("dem1.yuv");

    clock_t start1, end1;
    clock_t start2, end2;

    start1 = clock();
    basic->Transform(YUV_src, YUV_dst, 128);
    end1 = clock();
    printf("Basic ISA: %f\n", (double)(end1 - start1) / CLOCKS_PER_SEC);

    start2 = clock();
    mmx->Transform(YUV_src, YUV_dst, 128);
    end2 = clock();
    printf("MMX: %f\n", (double)(end2 - start2) / CLOCKS_PER_SEC);

    YUV_dst.Store("result.yuv");

    return 0;
}