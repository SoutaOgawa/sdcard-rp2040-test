#ifndef RADIO_H
#define RADIO_H

#include <stdint.h>

typedef struct {
    uint32_t time_us;
    uint32_t rpm;
    uint16_t temp;
} packet_t;

bool radio_init();
bool radio_send(const packet_t &pkt);

#endif