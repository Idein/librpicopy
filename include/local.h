/*
 * Copyright (c) 2019 Idein Inc. (info@idein.jp)
 * All rights reserved.
 * Author: Sugizaki Yukimasa (ysugi@idein.jp)
 *
 * This software is licensed under a Modified (3-Clause) BSD License.
 * You should have received a copy of this license along with this
 * software. If not, contact the copyright holder above.
 */


#ifndef RPICOPY_LOCAL_H_
#define RPICOPY_LOCAL_H_

#include <stdio.h>
#include <rpimemmgr.h>

    extern int rpicopy_dma_init(void);
    extern int rpicopy_dma_finalize(void);

    extern struct rpimemmgr rpicopy_rpimemmgr;

#define print_error(fmt, ...) \
    do { \
        fprintf(stderr, "%s:%u (%s): ", __FILE__, __LINE__, __func__); \
        fprintf(stderr, fmt, ##__VA_ARGS__); \
    } while (0)

#define BIT(n) (1U << (n))

#define _PASTE(a, b) a ## b
#define PASTE(a, b) _PASTE(a, b)

#define FIELD_GET(name, v) (((v) & PASTE(name, _SET)) >> PASTE(name, _LSB))

#define FIELD_PREP(name, v) (((v) << PASTE(name, _LSB)) & PASTE(name, _SET))

#endif /* RPICOPY_LOCAL_H_ */
