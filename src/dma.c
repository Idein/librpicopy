/*
 * Copyright (c) 2019 Idein Inc. (info@idein.jp)
 * All rights reserved.
 * Author: Sugizaki Yukimasa (ysugi@idein.jp)
 *
 * This software is licensed under a Modified (3-Clause) BSD License.
 * You should have received a copy of this license along with this
 * software. If not, contact the copyright holder above.
 */


#include "local.h"
#include "rpicopy.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <vc4regmap.h>


/*
 * See "BCM2835 ARM Peripherals" for a usage of DMA:
 * https://www.raspberrypi.org/documentation/hardware/raspberrypi/bcm2835/BCM2835-ARM-Peripherals.pdf
 */

struct dma_conblk {
    uint32_t ti;
    uint32_t source_ad;
    uint32_t dest_ad;
    uint32_t txfr_len;
    uint32_t stride;
    uint32_t nextconbk;
    uint32_t dummy[2];
};

static inline
uint32_t dma_to_cs(const int n)
{
    const uint32_t dma_cs = ((const uint32_t[15]) {
            DMA0_CS,
            DMA1_CS,
            DMA2_CS,
            DMA3_CS,
            DMA4_CS,
            DMA5_CS,
            DMA6_CS,
            DMA7_CS,
            DMA8_CS,
            DMA9_CS,
            DMA10_CS,
            DMA11_CS,
            DMA12_CS,
            DMA13_CS,
            DMA14_CS,
    })[n];
    return dma_cs;
}

static inline
uint32_t dma_to_conblk_ad(const int n)
{
    const uint32_t dma_conblk_ad = ((const uint32_t[15]) {
            DMA0_CONBLK_AD,
            DMA1_CONBLK_AD,
            DMA2_CONBLK_AD,
            DMA3_CONBLK_AD,
            DMA4_CONBLK_AD,
            DMA5_CONBLK_AD,
            DMA6_CONBLK_AD,
            DMA7_CONBLK_AD,
            DMA8_CONBLK_AD,
            DMA9_CONBLK_AD,
            DMA10_CONBLK_AD,
            DMA11_CONBLK_AD,
            DMA12_CONBLK_AD,
            DMA13_CONBLK_AD,
            DMA14_CONBLK_AD,
    })[n];
    return dma_conblk_ad;
}

static inline
void setup_dma_conblk(struct dma_conblk *cb, const uint32_t src_addr,
        const uint32_t dst_addr, const size_t size, const int burst,
        const int src_inc)
{
    memset(cb, 0, sizeof(*cb));
    cb->ti = FIELD_PREP(DMA0_TI_BURST_LENGTH, burst)
        | FIELD_PREP(DMA0_TI_SRC_WIDTH, 1)
        | FIELD_PREP(DMA0_TI_SRC_INC, src_inc)
        | FIELD_PREP(DMA0_TI_DEST_WIDTH, 1)
        | FIELD_PREP(DMA0_TI_DEST_INC, 1);
    cb->source_ad = FIELD_PREP(DMA0_SOURCE_AD_S_ADDR, src_addr);
    cb->dest_ad = FIELD_PREP(DMA0_DEST_AD_D_ADDR, dst_addr);
    /* XXX: In the future we may support 2D transfer. */
    cb->txfr_len = size;
}

static inline
void do_dma_copy(const int dma, const uint32_t cb_addr)
{
    const uint32_t dma_cs = dma_to_cs(dma);
    const uint32_t dma_conblk_ad = dma_to_conblk_ad(dma);

    vc4regmap_write(dma_cs, DMA0_CS_RESET_SET);
    vc4regmap_write(dma_conblk_ad,
            FIELD_PREP(DMA0_CONBLK_AD_SCB_ADDR, cb_addr >> 5));
    vc4regmap_write(dma_cs,
            FIELD_PREP(DMA0_CS_DISDEBUG, 1)
            | FIELD_PREP(DMA0_CS_PRIORITY, 15)
            | FIELD_PREP(DMA0_CS_ACTIVE, 1));
}

static inline
void wait_dma(const int dma)
{
    const uint32_t dma_cs = dma_to_cs(dma);
    while (vc4regmap_read(dma_cs) & DMA0_CS_ACTIVE_SET)
        ;
}

/*
 * There are 16 (15?) DMA engines on BCM283x.
 * DMA 2 and 3 are used by i2s and DMA 13 is used by mmc.
 * DMA 7-14 have lite engines that:
 * - have a half bandwidth compared with others.
 * - limit DMA length register (TXFR_LEN) to 16 bits.
 * - do not support SRC_IGNORE and DEST_IGNORE.
 * I do not know how to enable DMA 15.
 * DMA 0 is (misteriously) a little slower than DMA 1, 4, 5 and 6.
 */

#define MAX_DMA_NUM 5

static const int dma_list[MAX_DMA_NUM] = {1, 4, 5, 6, 0};
static struct dma_conblk *cbs[MAX_DMA_NUM];
static uint32_t cb_addrs[MAX_DMA_NUM];
static void *src_burst = NULL;
static uint32_t src_burst_addr = 0;

int rpicopy_dma_init(void)
{
    for (int i = 0; i < MAX_DMA_NUM; i ++)
        cbs[i] = NULL;
    for (int i = 0; i < MAX_DMA_NUM; i ++) {
        if (rpimemmgr_alloc_vcsm(sizeof(struct dma_conblk), 32,
                    VCSM_CACHE_TYPE_NONE, (void**) &cbs[i], &cb_addrs[i],
                    &rpicopy_rpimemmgr)) {
            print_error("Failed to allocate dma_conblk[%d]\n", i);
            goto failed_alloc_dma;
        }
    }

    if (rpimemmgr_alloc_vcsm(32 * 15, 32, VCSM_CACHE_TYPE_NONE,
                (void**) &src_burst, &src_burst_addr, &rpicopy_rpimemmgr)) {
        print_error("Failed to allocate burst buffer\n");
        goto failed_alloc_burst;
    }

    uint32_t v = vc4regmap_read(DMA_ENABLE);
    for (int i = 0; i < MAX_DMA_NUM; i ++)
        v |= BIT(dma_list[i]);
    vc4regmap_write(DMA_ENABLE, v);

    return 0;

failed_alloc_burst:
    if (rpimemmgr_free_by_usraddr(src_burst, &rpicopy_rpimemmgr)) {
        print_error("Failed to free burst buffer\n");
        /* Continue finalization. */
    }
    src_burst = NULL;
    src_burst_addr = 0;

failed_alloc_dma:
    for (int i = 0; i < MAX_DMA_NUM; i ++) {
        if (cbs[i] != NULL) {
            if (rpimemmgr_free_by_usraddr(cbs[i], &rpicopy_rpimemmgr)) {
                print_error("Failed to free dma_conblk[%d]\n", i);
                /* Continue finalization. */
            }
            cbs[i] = NULL;
            cb_addrs[i] = 0;
        }
    }

    return 1;
}

int rpicopy_dma_finalize(void)
{
    int err = 0;

    if (src_burst != NULL) {
        if (rpimemmgr_free_by_usraddr(src_burst, &rpicopy_rpimemmgr)) {
            print_error("Failed to free burst buffer\n");
            err = 1;
            /* Continue finalization. */
        }
        src_burst = NULL;
        src_burst_addr = 0;
    }

    for (int i = 0; i < MAX_DMA_NUM; i ++) {
        if (cbs[i] != NULL) {
            if (rpimemmgr_free_by_usraddr(cbs[i], &rpicopy_rpimemmgr)) {
                print_error("Failed to free dma_conblk[%d]\n", i);
                err = 1;
                /* Continue finalization. */
            }
            cbs[i] = NULL;
            cb_addrs[i] = 0;
        }
    }

    return err;
}

static inline
void mem_dma_core(const uint32_t dst_addr, const uint32_t src_addr,
        const size_t size, const int num_dma, const int burst,
        const int src_inc)
{
    const size_t size_per_dma = size / num_dma;

    uint32_t offset = 0;
    for (int i = 0; i < num_dma; i ++, offset += size_per_dma)
        setup_dma_conblk(cbs[i], src_inc ? src_addr + offset : src_addr,
                dst_addr + offset,
                (i == num_dma - 1) ? size - offset : size_per_dma, burst,
                src_inc);

    for (int i = 0; i < num_dma; i ++)
        do_dma_copy(dma_list[i], cb_addrs[i]);

    for (int i = 0; i < num_dma; i ++)
        wait_dma(dma_list[i]);
}

uint32_t memcpy_dma_config(const uint32_t dst_addr, const uint32_t src_addr,
        const size_t size, const int num_dma, const int burst)
{
    mem_dma_core(dst_addr, src_addr, size, num_dma, burst, 1);
    return dst_addr;
}

uint32_t memset_dma_config(const uint32_t dst_addr, const int c,
        const size_t size, const int num_dma, const int burst)
{
    /* The size of a burst in non-lite engine is 32 bytes. */
    memset(src_burst, c, 32 * burst);
    mem_dma_core(dst_addr, src_burst_addr, size, num_dma, burst, 0);
    return dst_addr;
}

/*
 * As for bursts, they can be more, but larger bursts brick your Raspberry Pi
 * if its bus is busy (e.g. running omxplayer at the same time).  Run
 * example/bench.c for performance comparison for each parameters.
 */

uint32_t memcpy_dma(const uint32_t dst_addr, const uint32_t src_addr,
        const size_t size)
{
    /*
     * 4,1: pi1
     * 6,1: pi2, pi3, pi3+
     */
    return memcpy_dma_config(dst_addr, src_addr, size, 5, 1);
}

uint32_t memset_dma(const uint32_t dst_addr, const int c, const size_t size)
{
    /*
     * 5,1: pi3, pi3+
     * 6,1: pi1, pi2
     */
    return memset_dma_config(dst_addr, c, size, 5, 1);
}
