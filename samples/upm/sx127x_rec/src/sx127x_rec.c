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

uint8_t buffer[10] = {0};

void main() {
    printf("LoRa Receiver\n");

    // start initialization
    // ss, reset, dio0
    // 101
    //setPins(10, 3, 2);
    // c1000
    //setPins(60, 82, 72);

    // complete initialization by finishing pin setup and
    // setting frequency
    sx1276_context dev = init(60, 82, 72, 915e6);
    SX1276SetRxConfig(dev, MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                      LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                      LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                      0, true, 0, 0, LORA_IQ_INVERSION_ON, true);

    SX1276SetPublicNetwork(dev, true);

    //dumpRegisters();

    int packet_size = 0;
    uint8_t ret_buf[256] = {0};

#if 1
    printf("Receiver\n");
    while(1) {
        if(SX1276SetRx(dev, 3000) == RX_DONE) {
        	SX1276GetRxBuffer(dev, ret_buf);
        	for(uint i =0; i < SX1276GetRxBufferlen(dev); i++) {
        		printf("%c", (char)ret_buf[i]);
        	}
        	printf("\n");
        }
// ** WORKING CODE **
/*
        // receive packet actually
        packet_size = parsePacket(0);
        if(packet_size) {
            printf("Received a packet!! Packet Size: %d\n", packet_size);
            printf("Value: ");
            while(available()) {
                printf("%c", (char)SX1276read());
            }
            printf("\n");
            printf("With RSSI: %d\n", packetRssi(MODEM_LORA));
        }
*/

        //upm_delay_us(10000000);
    }
#endif
}
