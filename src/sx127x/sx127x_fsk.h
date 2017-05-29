#pragma once

#define REG_RXCONFIG             0x0D
#define REG_RSSIVALUE            0x11
// fsk - 0x12 -x13
#define REG_RXBW                 0x12
#define REG_AFCBW                0x13
#define REG_AFCMSB               0x1B
#define REG_AFCLSB               0x1C
#define REG_FEIMSB               0x1D
#define REG_FEILSB 0x1E
// Packet handler settings
#define REG_PREAMBLEMSB          0x25
#define REG_PREAMBLELSB          0x26
#define REG_FSK_PAYLOADLENGTH    0x32
#define REG_PACKETCONFIG1        0x30
#define REG_PACKETCONFIG2        0x31
#define REG_PREAMBLE_DETECT      0x1F
#define REG_OSC                  0x24
#define REG_FDEVMSB              0x04
#define REG_FDEVLSB              0x05
#define REG_FIFOTHRESH           0x35
#define REG_SYNCCONFIG           0x27
#define REG_PAYLOADLENGTH        0x32

// Status
#define REG_IRQFLAGS1                               0x3E
#define REG_IRQFLAGS2                               0x3F

/*!
 * RegIrqFlags1
 */
#define RF_IRQFLAGS1_MODEREADY                      0x80
#define RF_IRQFLAGS1_RXREADY                        0x40
#define RF_IRQFLAGS1_TXREADY                        0x20
#define RF_IRQFLAGS1_PLLLOCK                        0x10
#define RF_IRQFLAGS1_RSSI                           0x08
#define RF_IRQFLAGS1_TIMEOUT                        0x04
#define RF_IRQFLAGS1_PREAMBLEDETECT                 0x02
#define RF_IRQFLAGS1_SYNCADDRESSMATCH               0x01

/*!
 * RegIrqFlags2
 */
#define RF_IRQFLAGS2_FIFOFULL                       0x80
#define RF_IRQFLAGS2_FIFOEMPTY                      0x40
#define RF_IRQFLAGS2_FIFOLEVEL                      0x20
#define RF_IRQFLAGS2_FIFOOVERRUN                    0x10
#define RF_IRQFLAGS2_PACKETSENT                     0x08
#define RF_IRQFLAGS2_PAYLOADREADY                   0x04
#define RF_IRQFLAGS2_CRCOK                          0x02
#define RF_IRQFLAGS2_LOWBAT                         0x01


#define RF_RXCONFIG_AFCAUTO_MASK                    0xEF
#define RF_RXCONFIG_AFCAUTO_ON                      0x10
#define RF_RXCONFIG_AFCAUTO_OFF                     0x00  // Default

#define RF_RXCONFIG_AGCAUTO_MASK                    0xF7
#define RF_RXCONFIG_AGCAUTO_ON                      0x08 // Default
#define RF_RXCONFIG_AGCAUTO_OFF                     0x00

#define RF_RXCONFIG_RXTRIGER_MASK                   0xF8
#define RF_RXCONFIG_RXTRIGER_OFF                    0x00
#define RF_RXCONFIG_RXTRIGER_RSSI                   0x01
#define RF_RXCONFIG_RXTRIGER_PREAMBLEDETECT         0x06 // Default
#define RF_RXCONFIG_RXTRIGER_RSSI_PREAMBLEDETECT    0x07

#define RF_SYNCCONFIG_SYNCSIZE_MASK                 0xF8
#define RF_SYNCCONFIG_SYNCSIZE_1                    0x00
#define RF_SYNCCONFIG_SYNCSIZE_2                    0x01
#define RF_SYNCCONFIG_SYNCSIZE_3                    0x02
#define RF_SYNCCONFIG_SYNCSIZE_4                    0x03  // Default
#define RF_SYNCCONFIG_SYNCSIZE_5                    0x04
#define RF_SYNCCONFIG_SYNCSIZE_6                    0x05
#define RF_SYNCCONFIG_SYNCSIZE_7                    0x06
#define RF_SYNCCONFIG_SYNCSIZE_8                    0x07

/*!
 * RegPacketConfig1
 */
#define RF_PACKETCONFIG1_PACKETFORMAT_MASK          0x7F
#define RF_PACKETCONFIG1_PACKETFORMAT_FIXED         0x00
#define RF_PACKETCONFIG1_PACKETFORMAT_VARIABLE      0x80  // Default

#define RF_PACKETCONFIG1_DCFREE_MASK                0x9F
#define RF_PACKETCONFIG1_DCFREE_OFF                 0x00  // Default
#define RF_PACKETCONFIG1_DCFREE_MANCHESTER          0x20
#define RF_PACKETCONFIG1_DCFREE_WHITENING           0x40

#define RF_PACKETCONFIG1_CRC_MASK                   0xEF
#define RF_PACKETCONFIG1_CRC_ON                     0x10  // Default
#define RF_PACKETCONFIG1_CRC_OFF                    0x00

#define RF_PACKETCONFIG1_CRCAUTOCLEAR_MASK          0xF7
#define RF_PACKETCONFIG1_CRCAUTOCLEAR_ON            0x00  // Default
#define RF_PACKETCONFIG1_CRCAUTOCLEAR_OFF           0x08

#define RF_PACKETCONFIG1_ADDRSFILTERING_MASK        0xF9
#define RF_PACKETCONFIG1_ADDRSFILTERING_OFF         0x00  // Default
#define RF_PACKETCONFIG1_ADDRSFILTERING_NODE        0x02
#define RF_PACKETCONFIG1_ADDRSFILTERING_NODEBROADCAST 0x04

#define RF_PACKETCONFIG1_CRCWHITENINGTYPE_MASK      0xFE
#define RF_PACKETCONFIG1_CRCWHITENINGTYPE_CCITT     0x00  // Default
#define RF_PACKETCONFIG1_CRCWHITENINGTYPE_IBM       0x01

/*!
 * RegPacketConfig2
 */

#define RF_PACKETCONFIG2_WMBUS_CRC_ENABLE_MASK      0x7F
#define RF_PACKETCONFIG2_WMBUS_CRC_ENABLE           0x80
#define RF_PACKETCONFIG2_WMBUS_CRC_DISABLE          0x00  // Default

#define RF_PACKETCONFIG2_DATAMODE_MASK              0xBF
#define RF_PACKETCONFIG2_DATAMODE_CONTINUOUS        0x00
#define RF_PACKETCONFIG2_DATAMODE_PACKET            0x40  // Default

#define RF_PACKETCONFIG2_IOHOME_MASK                0xDF
#define RF_PACKETCONFIG2_IOHOME_ON                  0x20
#define RF_PACKETCONFIG2_IOHOME_OFF                 0x00  // Default

#define RF_PACKETCONFIG2_BEACON_MASK                0xF7
#define RF_PACKETCONFIG2_BEACON_ON                  0x08
#define RF_PACKETCONFIG2_BEACON_OFF                 0x00  // Default

#define RF_PACKETCONFIG2_PAYLOADLENGTH_MSB_MASK     0xF8

/*!
 * RegDioMapping1
 */
#define RF_DIOMAPPING1_DIO0_MASK                    0x3F
#define RF_DIOMAPPING1_DIO0_00                      0x00  // Default
#define RF_DIOMAPPING1_DIO0_01                      0x40
#define RF_DIOMAPPING1_DIO0_10                      0x80
#define RF_DIOMAPPING1_DIO0_11                      0xC0

#define RF_DIOMAPPING1_DIO1_MASK                    0xCF
#define RF_DIOMAPPING1_DIO1_00                      0x00  // Default
#define RF_DIOMAPPING1_DIO1_01                      0x10
#define RF_DIOMAPPING1_DIO1_10                      0x20
#define RF_DIOMAPPING1_DIO1_11                      0x30

#define RF_DIOMAPPING1_DIO2_MASK                    0xF3
#define RF_DIOMAPPING1_DIO2_00                      0x00  // Default
#define RF_DIOMAPPING1_DIO2_01                      0x04
#define RF_DIOMAPPING1_DIO2_10                      0x08
#define RF_DIOMAPPING1_DIO2_11                      0x0C

#define RF_DIOMAPPING1_DIO3_MASK                    0xFC
#define RF_DIOMAPPING1_DIO3_00                      0x00  // Default
#define RF_DIOMAPPING1_DIO3_01                      0x01
#define RF_DIOMAPPING1_DIO3_10                      0x02
#define RF_DIOMAPPING1_DIO3_11                      0x03

/*!
 * RegDioMapping2
 */
#define RF_DIOMAPPING2_DIO4_MASK                    0x3F
#define RF_DIOMAPPING2_DIO4_00                      0x00  // Default
#define RF_DIOMAPPING2_DIO4_01                      0x40
#define RF_DIOMAPPING2_DIO4_10                      0x80
#define RF_DIOMAPPING2_DIO4_11                      0xC0

#define RF_DIOMAPPING2_DIO5_MASK                    0xCF
#define RF_DIOMAPPING2_DIO5_00                      0x00  // Default
#define RF_DIOMAPPING2_DIO5_01                      0x10
#define RF_DIOMAPPING2_DIO5_10                      0x20
#define RF_DIOMAPPING2_DIO5_11                      0x30

#define RF_DIOMAPPING2_MAP_MASK                     0xFE
#define RF_DIOMAPPING2_MAP_PREAMBLEDETECT           0x01
#define RF_DIOMAPPING2_MAP_RSSI                     0x00  // Default

/*!
 * RegRxConfig
 */
#define RF_RXCONFIG_RESTARTRXONCOLLISION_MASK       0x7F
#define RF_RXCONFIG_RESTARTRXONCOLLISION_ON         0x80
#define RF_RXCONFIG_RESTARTRXONCOLLISION_OFF        0x00 // Default

#define RF_RXCONFIG_RESTARTRXWITHOUTPLLLOCK         0x40 // Write only

#define RF_RXCONFIG_RESTARTRXWITHPLLLOCK            0x20 // Write only
