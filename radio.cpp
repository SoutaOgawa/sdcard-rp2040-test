#include <stdio.h>
#include "pico/stdlib.h"
#include "RF24.h"
#include "radio.h"

static SPI spi;
static RF24 radio(26, 27);   // CE = GP26, CSN = GP27

static const uint8_t address[][6] = {"1Node", "2Node"};

bool radio_init()
{
    spi.begin(spi1, 14, 15, 8);   // SCK, MOSI, MISO

    if (!radio.begin(&spi)) {
        printf("radio hardware is not responding!!\n");
        return false;
    }

    radio.setPALevel(RF24_PA_LOW);
    radio.setPayloadSize(sizeof(packet_t));
    radio.openWritingPipe(address[0]);
    radio.stopListening();   // TX mode
    return true;
}

bool radio_send(const packet_t &pkt)
{
    return radio.write(&pkt, sizeof(pkt));
}