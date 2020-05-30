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

    Basic* basic = new Basic();
    MMX* mmx = new MMX();
    SSE2* sse2 = new SSE2();
    AVX* avx = new AVX();

    char srcPath[20] = "dem1.yuv";
    char dstPath[20] = "result.yuv";

    YUV_src.Load(srcPath);

    clock_t start1, end1;
    clock_t start2, end2;
    clock_t start3, end3;
    clock_t start4, end4;

    start1 = clock();
    basic->Transform(YUV_src, YUV_dst, 128);
    end1 = clock();
    printf("Basic ISA: %f\n", (double)(end1 - start1) / CLOCKS_PER_SEC);

    // start2 = clock();
    // mmx->Transform(YUV_src, YUV_dst, 128);
    // end2 = clock();
    // printf("MMX: %f\n", (double)(end2 - start2) / CLOCKS_PER_SEC);

    start3 = clock();
    sse2->Transform(YUV_src, YUV_dst, 128);
    end3 = clock();
    printf("SSE2: %f\n", (double)(end3 - start3) / CLOCKS_PER_SEC);

    start4 = clock();
    avx->Transform(YUV_src, YUV_dst, 128);
    end4 = clock();
    printf("AVX: %f\n", (double)(end4 - start4) / CLOCKS_PER_SEC);

    YUV_dst.Store(dstPath);

    return 0;
}