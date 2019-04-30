/*
 * Copyright (c) 2019 Idein Inc. (info@idein.jp)
 * All rights reserved.
 * Author: Sugizaki Yukimasa (ysugi@idein.jp)
 *
 * This software is licensed under a Modified (3-Clause) BSD License.
 * You should have received a copy of this license along with this
 * software. If not, contact the copyright holder above.
 */


#ifndef RPICOPY_H_
#define RPICOPY_H_

#include <stddef.h>
#include <stdint.h>

    extern int rpicopy_init(void);
    extern int rpicopy_finalize(void);

    extern uint32_t memcpy_dma_config(const uint32_t dst_addr,
            const uint32_t src_addr, const size_t size, const int n,
            const int burst);
    extern uint32_t memset_dma_config(const uint32_t dst_addr, const int c,
            const size_t size, const int n, const int burst);
    extern uint32_t memcpy_dma(const uint32_t dst_addr, const uint32_t src_addr,
            const size_t size);
    extern uint32_t memset_dma(const uint32_t dst_addr, const int c,
            const size_t size);

#endif /* RPICOPY_H_ */
