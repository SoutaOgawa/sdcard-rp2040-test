#ifndef SDLOG_H
#define SDLOG_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "ff.h"

    typedef struct
    {
        FATFS fs;
        FIL fil;
        char filename[16];
    } sdlog_t;

    void sdlog_init(sdlog_t *log);
    void sdlog_row(sdlog_t *log, uint32_t time_us, uint32_t rpm, uint16_t temp, uint16_t tps);
    void sdlog_close(sdlog_t *log);

#ifdef __cplusplus
}
#endif

#endif