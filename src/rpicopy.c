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
#include <rpimemmgr.h>
#include <vc4regmap.h>

struct rpimemmgr rpicopy_rpimemmgr;

int rpicopy_init(void)
{
    if (vc4regmap_init()) {
        print_error("Failed to initialize vc4regmap\n");
        goto failed_vc4regmap;
    }

    if (rpimemmgr_init(&rpicopy_rpimemmgr)) {
        print_error("Failed to initialize rpimemmgr\n");
        goto failed_rpimemmgr;
    }

    if (rpicopy_dma_init()) {
        print_error("Failed to initialize DMA\n");
        goto failed_dma;
    }

    return 0;

failed_dma:
    if (rpimemmgr_finalize(&rpicopy_rpimemmgr)) {
        print_error("Failed to finalize rpimemmgr\n");
        /* Continue finalization. */
    }

failed_rpimemmgr:
    if (vc4regmap_finalize()) {
        print_error("Failed to finalize vc4regmap\n");
        /* Continue finalization. */
    }

failed_vc4regmap:
    return 1;
}

int rpicopy_finalize(void)
{
    int err = 0;

    if (rpicopy_dma_finalize()) {
        print_error("Failed to finalize DMA\n");
        err = 1;
        /* Continue finalization. */
    }

    if (rpimemmgr_finalize(&rpicopy_rpimemmgr)) {
        print_error("Failed to finalize rpimemmgr\n");
        err = 1;
        /* Continue finalization. */
    }

    return err;
}
