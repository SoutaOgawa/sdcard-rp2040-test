#include <stdio.h>
#include "pico/stdlib.h"
#include "hw_config.h"
#include "f_util.h"
#include "sdlog.h"

static FRESULT open_new_log(FIL *fil, char *name, size_t len)
{
    for (int i = 1; i <= 9999; i++)
    {
        snprintf(name, len, "LOG%04d.csv", i);
        FRESULT fr = f_open(fil, name, FA_WRITE | FA_CREATE_NEW);
        if (fr != FR_EXIST)
            return fr; // FR_OK, or a real error
    }
    return FR_EXIST; // all 9999 names used
}

void sdlog_init(sdlog_t *log)
{
    FRESULT fr = f_mount(&log->fs, "", 1);
    if (FR_OK != fr)
    {
        panic("f_mount error: %s (%d)\n", FRESULT_str(fr), fr);
    }

    fr = open_new_log(&log->fil, log->filename, sizeof log->filename);
    if (FR_OK != fr)
    {
        panic("create error: %s (%d)\n", FRESULT_str(fr), fr);
    }

    f_printf(&log->fil, "time_us,rpm,temp\n"); // header row
}

void sdlog_row(sdlog_t *log, uint32_t time_us, uint32_t rpm, uint16_t temp, uint16_t tps)
{
    if (f_printf(&log->fil, "%lu,%lu,%lu,%lu\n", (unsigned long)time_us, (unsigned long)rpm, (unsigned long)temp, (unsigned long)tps) < 0)
    {
        printf("f_printf failed\n");
    }
    else
    {
        f_sync(&log->fil);
    }
}

void sdlog_close(sdlog_t *log)
{
    FRESULT fr = f_close(&log->fil);
    if (FR_OK != fr)
    {
        printf("f_close error: %s (%d)\n", FRESULT_str(fr), fr);
    }

    fr = f_unmount("");
    if (FR_OK != fr)
    {
        printf("f_unmount error: %s (%d)\n", FRESULT_str(fr), fr);
    }
}