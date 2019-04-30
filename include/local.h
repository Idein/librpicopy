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
