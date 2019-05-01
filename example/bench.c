/*
 * Copyright (c) 2019 Idein Inc. (info@idein.jp)
 * All rights reserved.
 * Author: Sugizaki Yukimasa (ysugi@idein.jp)
 *
 * This software is licensed under a Modified (3-Clause) BSD License.
 * You should have received a copy of this license along with this
 * software. If not, contact the copyright holder above.
 */


#include <rpicopy.h>
#include <rpimemmgr.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define barrier() __asm__ volatile ("" : : : "memory")
#define barrier_data(ptr) __asm__ volatile ("" : : "r" (ptr) : "memory")

#define printf_abort(str, ...) \
    do { \
        fprintf(stderr, "%s:%u (%s): ", __FILE__, __LINE__, __func__); \
        fprintf(stderr, str, ##__VA_ARGS__); \
        exit(EXIT_FAILURE); \
    } while (0)


static inline
double getsec(void)
{
    struct timespec t;
    (void) clock_gettime(CLOCK_MONOTONIC_RAW, &t);
    return (double) t.tv_sec + t.tv_nsec * 1e-9;
}

static
_Bool check_if_all(uint8_t *p, size_t size, const uint8_t c)
{
    for (size_t i = 0; i < size; i ++)
        if (p[i] != c)
            return 0;
    return !0;
}

static uint8_t *src, *dst;
static uint32_t src_addr, dst_addr;
static const size_t size = 4096 * 4096;

static
void test_memcpy(const int num_dma, const int burst)
{
    memcpy_dma_config(dst_addr, src_addr, size, num_dma, burst);
}

static
void test_memset(const int num_dma, const int burst)
{
    memset_dma_config(dst_addr, src[0], size, num_dma, burst);
}

static
void do_bench(const int num_dma, const int burst, void (*func)(int, int))
{
    printf("num_dma=%d burst=%d: ", num_dma, burst);
    memset(src, 0x55, size);
    memset(dst, 0xaa, size);

    if (!check_if_all(src, size, 0x55))
        printf_abort("Input src is not all 0x55\n");
    if (!check_if_all(dst, size, 0xaa))
        printf_abort("Input dst is not all 0xaa\n");

    const int rep = 32;

    barrier();
    const double start = getsec();
    barrier();
    for (int i = 0; i < rep; i ++) {
        barrier_data(src);
        barrier_data(dst);
        func(num_dma, burst);
    }
    barrier();
    const double end = getsec();
    barrier();
    const double t = end - start;
    printf("%e [s], %e [s], %e [B/s]\n", t, t / rep, size / (t / rep));

    if (!check_if_all(src, size, 0x55))
        printf_abort("Output src is not all 0x55\n");
    if (!check_if_all(dst, size, 0x55))
        printf_abort("Output dst is not all 0x55\n");
}

int main(void)
{
    struct rpimemmgr st;

    if (rpicopy_init())
        printf_abort("rpicopy_init\n");
    if (rpimemmgr_init(&st))
        printf_abort("rpimemmgr_init\n");
    if (rpimemmgr_alloc_vcsm(size, 1, VCSM_CACHE_TYPE_NONE, (void**) &src,
                &src_addr, &st))
        printf_abort("rpimemmgr_alloc_vcsm\n");
    if (rpimemmgr_alloc_vcsm(size, 1, VCSM_CACHE_TYPE_NONE, (void**) &dst,
                &dst_addr, &st))
        printf_abort("rpimemmgr_alloc_vcsm\n");

    printf("# DMA memcpy\n");
    do_bench(1, 1, test_memcpy);
    do_bench(1, 2, test_memcpy);
    do_bench(1, 3, test_memcpy);
    do_bench(2, 1, test_memcpy);
    do_bench(3, 1, test_memcpy);
    do_bench(4, 1, test_memcpy);
    do_bench(5, 1, test_memcpy);

    printf("# DMA memset\n");
    do_bench(1, 1, test_memset);
    do_bench(1, 2, test_memset);
    do_bench(1, 3, test_memset);
    do_bench(2, 1, test_memset);
    do_bench(3, 1, test_memset);
    do_bench(4, 1, test_memset);
    do_bench(5, 1, test_memset);

    if (rpimemmgr_free_by_usraddr(dst, &st))
        printf_abort("rpimemmgr_free_by_usraddr\n");
    if (rpimemmgr_free_by_usraddr(src, &st))
        printf_abort("rpimemmgr_free_by_usraddr\n");
    if (rpimemmgr_finalize(&st))
        printf_abort("rpimemmgr_finalize\n");
    if (rpicopy_finalize())
        printf_abort("rpicopy_finalize\n");
    return 0;
}
