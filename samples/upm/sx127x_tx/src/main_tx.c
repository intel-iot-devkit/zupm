#include <zephyr.h>
#include <misc/printk.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdint.h>
#include <math.h>
#include "upm.h"
#include "mraa.h"
#include "sx127x.h"
#include "upm_utilities.h"

#define TX_OUTPUT_POWER                             20        // dBm

#define LORA_BANDWIDTH                              0         // [0: 125 kHz,
                                                              //  1: 250 kHz,
                                                              //  2: 500 kHz,
                                                              //  3: Reserved]
#define LORA_SPREADING_FACTOR                       7         // [SF7..SF12]
#define LORA_CODINGRATE                             1         // [1: 4/5,
                                                              //  2: 4/6,
                                                              //  3: 4/7,
                                                              //  4: 4/8]
#define LORA_PREAMBLE_LENGTH                        8         // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT                         5         // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON                  false
#define LORA_IQ_INVERSION_ON                        false

int counter = 0;

uint8_t buffer[25] = {0};

void main() {
    printf("LoRa Sender\n");

    // start initialization
    // ss, reset, dio0
    // 101
    //setPins(10, 3, 2);
    // c1000
    //setPins(60, 82, 72);

    // complete initialization by finishing pin setup and
    // setting frequency
    sx1276_context dev = init(60, 82, 72, 915e6);
    SX1276SetTxConfig(dev, MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                      LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                      LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                      true, 0, 0, LORA_IQ_INVERSION_ON, 3000);

    SX1276SetPublicNetwork(dev, true);

    //dumpRegisters();

#if 1
    while(1) {
        printf("sending packet: %d\n", counter);
        // adding ping here
        snprintf(buffer, 25, "Ping from Abhishek %d", counter++);
        SX1276Send(dev, buffer, 25);

// ORIGINAL CODE ** WORKS **
#if 0
        // send packet actually
        beginPacket(false);
        //printf("packet sent: ")
        snprintf(buffer, 10, "Ping %d", counter++);
        printf("buffer: %s\n", buffer);
        write_buf(buffer, 10);
        endPacket();
#endif

        counter++;

        upm_delay_us(10000000);
    }
#endif
}
