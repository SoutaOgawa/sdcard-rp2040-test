#include <stdio.h>

#include "pico/stdlib.h"
#include "pico/rand.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

#include "sdlog.h"
//#include "radio.h"

typedef struct {
    uint32_t time_us;
    uint32_t rpm;
    uint16_t temp;
    uint16_t tps;
} packet_t;

void setup_adc()
{
    adc_init();
    adc_gpio_init(26); // ADC0 on GPIO26
    adc_gpio_init(27); // ADC1 on GPIO27
}

int main()
{
    stdio_init_all();
    setup_adc();
    sleep_ms(2000); // Wait for USB serial to be ready

    puts("Hello, world!");

    //if (!radio_init())
    //{
    //    printf("radio init failed\n");
    //}

    sdlog_t log;
    sdlog_init(&log);
    printf("Logging to %s\n", log.filename);

    while (true)
    {
        packet_t pkt = {time_us_32(), (uint32_t)0, (uint16_t)0, (uint16_t)0};

        adc_select_input(0);
        pkt.temp = adc_read();

        adc_select_input(1);
        pkt.tps = adc_read();

        sdlog_row(&log, pkt.time_us, pkt.rpm, pkt.temp, pkt.tps);
        //radio_send(pkt);
        printf("%lu %lu %lu\n", (unsigned long)pkt.time_us, (unsigned long)pkt.temp, (unsigned long)pkt.tps);
        sleep_ms(100);
    }
}