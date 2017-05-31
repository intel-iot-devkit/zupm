/*
 * Author: Abhishek Malik <abhishek.malik@intel.com>
 *
 * This code is heavily based on the semtech lora library at:
 * https://github.com/Lora-net/LoRaMac-node
 * which is licensed under the BSD license at:
 * upm/src/sx127x/LICENSE.txt
 */


#pragma once

#include "upm.h"
#include "mraa.h"
#include "mraa/gpio.h"
#include "mraa/spi.h"
#include <math.h>
#include <string.h>

#define LORA_DEBUG 0

#define LORA_DEFAULT_SS_PIN    10
#define LORA_DEFAULT_RESET_PIN 9
#define LORA_DEFAULT_DIO0_PIN  2

#define PA_OUTPUT_RFO_PIN      0
#define PA_OUTPUT_PA_BOOST_PIN 1

// registers - mostly lora specific
#define REG_FIFO                 0x00
#define REG_OP_MODE              0x01
#define REG_BITRATE_MSB          0x02
#define REG_BITRATE_LSB          0x03
#define REG_FRF_MSB              0x06
#define REG_FRF_MID              0x07
#define REG_FRF_LSB              0x08
#define REG_PA_CONFIG            0x09
#define REG_LNA                  0x0c
#define REG_FIFO_ADDR_PTR        0x0d
#define REG_FIFO_TX_BASE_ADDR    0x0e
#define REG_FIFO_RX_BASE_ADDR    0x0f
#define REG_FIFO_RX_CURRENT_ADDR 0x10
#define REG_LR_IRQ_FLAGS_MASK    0x11
#define REG_IRQ_FLAGS            0x12
#define REG_RX_NB_BYTES          0x13
#define REG_PKT_RSSI_VALUE       0x1a
#define REG_PKT_SNR_VALUE        0x1b
#define REG_MODEM_CONFIG_1       0x1d
#define REG_MODEM_CONFIG_2       0x1e
#define REG_LR_SYMBTIMEOUT_LSB   0x1F
#define REG_LR_PKTSNRVALUE       0x19
#define REG_PREAMBLE_MSB         0x20
#define REG_PREAMBLE_LSB         0x21
#define REG_PAYLOAD_LENGTH       0x22
#define REG_LR_PAYLOADMAXLENGTH  0x23
#define REG_LR_HOPPERIOD         0x24
#define REG_MODEM_CONFIG_3       0x26
#define REG_SYNC_CONFIG          0x27
#define REG_SYNCVALUE1           0x28
#define REG_SYNCVALUE2           0x29
#define REG_SYNCVALUE3           0x2A
#define REG_RSSI_WIDEBAND        0x2c
#define REG_LR_TEST2F            0x2F
#define REG_PACKET_CONFIG1       0x30
#define REG_DETECTION_OPTIMIZE   0x31
#define REG_FIFO_THRESH          0x35
#define REG_DETECTION_THRESHOLD  0x37
#define REG_SYNC_WORD            0x39
#define REG_DIO_MAPPING_1        0x40
#define REG_DIO_MAPPING_2        0x41
#define REG_VERSION              0x42
#define REG_LR_TEST30            0x30
#define REG_LR_DETECTOPTIMIZE    0x31
#define REG_LR_INVERTIQ          0x33
#define REG_LR_TEST36            0x36
#define REG_LR_DETECTIONTHRESHOLD                   0x37
#define REG_LR_SYNCWORD          0x39
#define REG_LR_TEST3A            0x3A
#define REG_LR_INVERTIQ2         0x3B
#define REG_IMAGE_CAL            0x3B
#define REG_LR_PLLHOP            0x44

#define RX_BUFFER_SIZE           256

/*!
 * Radio wakeup time from SLEEP mode
 */
#define RADIO_OSC_STARTUP                           1 // [ms]

/*!
 * Radio PLL lock and Mode Ready delay which can vary with the temperature
 */
#define RADIO_SLEEP_TO_RX                           2 // [ms]

/*!
 * Radio complete Wake-up Time with margin for temperature compensation
 */
#define RADIO_WAKEUP_TIME ( RADIO_OSC_STARTUP + RADIO_SLEEP_TO_RX )

/*!
 * Sync word for Private LoRa networks
 */
#define LORA_MAC_PRIVATE_SYNCWORD                   0x12

/*!
 * Sync word for Public LoRa networks
 */
#define LORA_MAC_PUBLIC_SYNCWORD                    0x34

/*!
 * RegDetectionThreshold
 */
#define RFLR_DETECTIONTHRESH_SF7_TO_SF12            0x0A // Default
#define RFLR_DETECTIONTHRESH_SF6                    0x0C

/*!
 * RegDetectOptimize
 */
#define RFLR_DETECTIONOPTIMIZE_MASK                 0xF8
#define RFLR_DETECTIONOPTIMIZE_SF7_TO_SF12          0x03 // Default
#define RFLR_DETECTIONOPTIMIZE_SF6                  0x05

/*!
 * RegPllHop - lora
 */
#define RFLR_PLLHOP_FASTHOP_MASK                    0x7F
#define RFLR_PLLHOP_FASTHOP_ON                      0x80
#define RFLR_PLLHOP_FASTHOP_OFF                     0x00 // Default

/*!
 * RegModemConfig1 - lora
 */
#define RFLR_MODEMCONFIG1_BW_MASK                   0x0F
#define RFLR_MODEMCONFIG1_BW_7_81_KHZ               0x00
#define RFLR_MODEMCONFIG1_BW_10_41_KHZ              0x10
#define RFLR_MODEMCONFIG1_BW_15_62_KHZ              0x20
#define RFLR_MODEMCONFIG1_BW_20_83_KHZ              0x30
#define RFLR_MODEMCONFIG1_BW_31_25_KHZ              0x40
#define RFLR_MODEMCONFIG1_BW_41_66_KHZ              0x50
#define RFLR_MODEMCONFIG1_BW_62_50_KHZ              0x60
#define RFLR_MODEMCONFIG1_BW_125_KHZ                0x70 // Default
#define RFLR_MODEMCONFIG1_BW_250_KHZ                0x80
#define RFLR_MODEMCONFIG1_BW_500_KHZ                0x90

#define RFLR_MODEMCONFIG1_CODINGRATE_MASK           0xF1
#define RFLR_MODEMCONFIG1_CODINGRATE_4_5            0x02
#define RFLR_MODEMCONFIG1_CODINGRATE_4_6            0x04 // Default
#define RFLR_MODEMCONFIG1_CODINGRATE_4_7            0x06
#define RFLR_MODEMCONFIG1_CODINGRATE_4_8            0x08

#define RFLR_MODEMCONFIG1_IMPLICITHEADER_MASK       0xFE
#define RFLR_MODEMCONFIG1_IMPLICITHEADER_ON         0x01
#define RFLR_MODEMCONFIG1_IMPLICITHEADER_OFF        0x00 // Default

/*!
 * RegModemConfig2 - lora
 */
#define RFLR_MODEMCONFIG2_SF_MASK                   0x0F
#define RFLR_MODEMCONFIG2_SF_6                      0x60
#define RFLR_MODEMCONFIG2_SF_7                      0x70 // Default
#define RFLR_MODEMCONFIG2_SF_8                      0x80
#define RFLR_MODEMCONFIG2_SF_9                      0x90
#define RFLR_MODEMCONFIG2_SF_10                     0xA0
#define RFLR_MODEMCONFIG2_SF_11                     0xB0
#define RFLR_MODEMCONFIG2_SF_12                     0xC0

#define RFLR_MODEMCONFIG2_TXCONTINUOUSMODE_MASK     0xF7
#define RFLR_MODEMCONFIG2_TXCONTINUOUSMODE_ON       0x08
#define RFLR_MODEMCONFIG2_TXCONTINUOUSMODE_OFF      0x00

#define RFLR_MODEMCONFIG2_RXPAYLOADCRC_MASK         0xFB
#define RFLR_MODEMCONFIG2_RXPAYLOADCRC_ON           0x04
#define RFLR_MODEMCONFIG2_RXPAYLOADCRC_OFF          0x00 // Default

#define RFLR_MODEMCONFIG2_SYMBTIMEOUTMSB_MASK       0xFC
#define RFLR_MODEMCONFIG2_SYMBTIMEOUTMSB            0x00 // Default

/*!
 * RegModemConfig3 - lora
 */
#define RFLR_MODEMCONFIG3_LOWDATARATEOPTIMIZE_MASK  0xF7
#define RFLR_MODEMCONFIG3_LOWDATARATEOPTIMIZE_ON    0x08
#define RFLR_MODEMCONFIG3_LOWDATARATEOPTIMIZE_OFF   0x00 // Default

#define RFLR_MODEMCONFIG3_AGCAUTO_MASK              0xFB
#define RFLR_MODEMCONFIG3_AGCAUTO_ON                0x04 // Default
#define RFLR_MODEMCONFIG3_AGCAUTO_OFF 0x00

// reg image cal related WOW!!
/*!
 * RegImageCal
 */
#define RF_IMAGECAL_AUTOIMAGECAL_MASK               0x7F
#define RF_IMAGECAL_AUTOIMAGECAL_ON                 0x80
#define RF_IMAGECAL_AUTOIMAGECAL_OFF                0x00  // Default

#define RF_IMAGECAL_IMAGECAL_MASK                   0xBF
#define RF_IMAGECAL_IMAGECAL_START                  0x40

#define RF_IMAGECAL_IMAGECAL_RUNNING                0x20
#define RF_IMAGECAL_IMAGECAL_DONE                   0x00  // Default

#define RF_IMAGECAL_TEMPCHANGE_HIGHER               0x08
#define RF_IMAGECAL_TEMPCHANGE_LOWER                0x00

#define RF_IMAGECAL_TEMPTHRESHOLD_MASK              0xF9
#define RF_IMAGECAL_TEMPTHRESHOLD_05                0x00
#define RF_IMAGECAL_TEMPTHRESHOLD_10                0x02  // Default
#define RF_IMAGECAL_TEMPTHRESHOLD_15                0x04
#define RF_IMAGECAL_TEMPTHRESHOLD_20                0x06

#define RF_IMAGECAL_TEMPMONITOR_MASK                0xFE
#define RF_IMAGECAL_TEMPMONITOR_ON                  0x00 // Default
#define RF_IMAGECAL_TEMPMONITOR_OFF                 0x01

/*!
 * RegIrqFlags
 */
#define RFLR_IRQFLAGS_RXTIMEOUT                     0x80
#define RFLR_IRQFLAGS_RXDONE                        0x40
#define RFLR_IRQFLAGS_PAYLOADCRCERROR               0x20
#define RFLR_IRQFLAGS_VALIDHEADER                   0x10
#define RFLR_IRQFLAGS_TXDONE                        0x08
#define RFLR_IRQFLAGS_CADDONE                       0x04
#define RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL            0x02
#define RFLR_IRQFLAGS_CADDETECTED                   0x01

/*!
 * RegInvertIQ
 */
#define RFLR_INVERTIQ_RX_MASK                       0xBF
#define RFLR_INVERTIQ_RX_OFF                        0x00
#define RFLR_INVERTIQ_RX_ON                         0x40
#define RFLR_INVERTIQ_TX_MASK                       0xFE
#define RFLR_INVERTIQ_TX_OFF                        0x01
#define RFLR_INVERTIQ_TX_ON                         0x00

/*!
 * RegInvertIQ2
 */
#define RFLR_INVERTIQ2_ON                           0x19
#define RFLR_INVERTIQ2_OFF                          0x1D

/*!
 * RegDioMapping1
 */
#define RFLR_DIOMAPPING1_DIO0_MASK                  0x3F
#define RFLR_DIOMAPPING1_DIO0_00                    0x00  // Default
#define RFLR_DIOMAPPING1_DIO0_01                    0x40
#define RFLR_DIOMAPPING1_DIO0_10                    0x80
#define RFLR_DIOMAPPING1_DIO0_11                    0xC0

#define RFLR_DIOMAPPING1_DIO1_MASK                  0xCF
#define RFLR_DIOMAPPING1_DIO1_00                    0x00  // Default
#define RFLR_DIOMAPPING1_DIO1_01                    0x10
#define RFLR_DIOMAPPING1_DIO1_10                    0x20
#define RFLR_DIOMAPPING1_DIO1_11                    0x30

#define RFLR_DIOMAPPING1_DIO2_MASK                  0xF3
#define RFLR_DIOMAPPING1_DIO2_00                    0x00  // Default
#define RFLR_DIOMAPPING1_DIO2_01                    0x04
#define RFLR_DIOMAPPING1_DIO2_10                    0x08
#define RFLR_DIOMAPPING1_DIO2_11                    0x0C

#define RFLR_DIOMAPPING1_DIO3_MASK                  0xFC
#define RFLR_DIOMAPPING1_DIO3_00                    0x00  // Default
#define RFLR_DIOMAPPING1_DIO3_01                    0x01
#define RFLR_DIOMAPPING1_DIO3_10                    0x02
#define RFLR_DIOMAPPING1_DIO3_11                    0x03

/*!
 * RegDioMapping2
 */
#define RFLR_DIOMAPPING2_DIO4_MASK                  0x3F
#define RFLR_DIOMAPPING2_DIO4_00                    0x00  // Default
#define RFLR_DIOMAPPING2_DIO4_01                    0x40
#define RFLR_DIOMAPPING2_DIO4_10                    0x80
#define RFLR_DIOMAPPING2_DIO4_11                    0xC0

#define RFLR_DIOMAPPING2_DIO5_MASK                  0xCF
#define RFLR_DIOMAPPING2_DIO5_00                    0x00  // Default
#define RFLR_DIOMAPPING2_DIO5_01                    0x10
#define RFLR_DIOMAPPING2_DIO5_10                    0x20
#define RFLR_DIOMAPPING2_DIO5_11                    0x30

#define RFLR_DIOMAPPING2_MAP_MASK                   0xFE
#define RFLR_DIOMAPPING2_MAP_PREAMBLEDETECT         0x01
#define RFLR_DIOMAPPING2_MAP_RSSI                   0x00  // Default

// register masks
/*!
 * RegIrqFlagsMask
 */
#define RFLR_IRQFLAGS_RXTIMEOUT_MASK                0x80
#define RFLR_IRQFLAGS_RXDONE_MASK                   0x40
#define RFLR_IRQFLAGS_PAYLOADCRCERROR_MASK          0x20
#define RFLR_IRQFLAGS_VALIDHEADER_MASK              0x10
#define RFLR_IRQFLAGS_TXDONE_MASK                   0x08
#define RFLR_IRQFLAGS_CADDONE_MASK                  0x04
#define RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL_MASK       0x02
#define RFLR_IRQFLAGS_CADDETECTED_MASK              0x01


// modes
#define MODE_LONG_RANGE_MODE     0x80
#define MODE_LONG_RANGE_MODE_OFF 0x7F
#define MODE_SLEEP               0x00
#define MODE_STDBY               0x01
#define MODE_TX                  0x03
#define MODE_RX_CONTINUOUS       0x05
#define MODE_RX_SINGLE           0x06
#define MODE_CAD                 0x07
#define OP_MODE_MASK             0x80

// PA config
#define PA_BOOST                 0x80

#define MAX_PKT_LENGTH             255

#define FREQ_STEP              61.03515625
#define XTAL_FREQ              32000000
#define RF_MID_BAND_THRESH     525000000

// init values, use, no idea
#define RADIO_INIT_REGISTERS_VALUE                \
		{                                                 \
	{ MODEM_FSK , REG_LNA                , 0x23 },\
	{ MODEM_FSK , REG_FIFO_ADDR_PTR      , 0x1E },\
	{ MODEM_FSK , REG_FIFO_TX_BASE_ADDR  , 0xD2 },\
	{ MODEM_FSK , REG_PKT_RSSI_VALUE     , 0x01 },\
	{ MODEM_FSK , REG_PREAMBLE_DETECT    , 0xAA },\
	{ MODEM_FSK , REG_OSC                , 0x07 },\
	{ MODEM_FSK , REG_SYNC_CONFIG        , 0x12 },\
	{ MODEM_FSK , REG_SYNCVALUE1         , 0xC1 },\
	{ MODEM_FSK , REG_SYNCVALUE2         , 0x94 },\
	{ MODEM_FSK , REG_SYNCVALUE3         , 0xC1 },\
	{ MODEM_FSK , REG_PACKET_CONFIG1     , 0xD8 },\
	{ MODEM_FSK , REG_FIFO_THRESH        , 0x8F },\
	{ MODEM_FSK , REG_IMAGE_CAL          , 0x02 },\
	{ MODEM_FSK , REG_DIO_MAPPING_1      , 0x00 },\
	{ MODEM_FSK , REG_DIO_MAPPING_2      , 0x30 },\
	{ MODEM_LORA, REG_LR_PAYLOADMAXLENGTH, 0x40 },\
		}

typedef enum
{
	MODEM_FSK = 0,
	MODEM_LORA
} RadioModems_t;

/*!
 * FSK bandwidth definition
 */
typedef struct
{
	uint32_t bandwidth;
	uint8_t  RegValue;
}FskBandwidth_t;

/*!
 * Radio driver internal state machine states definition
 */
typedef enum
{
	RF_IDLE = 0,
	RF_RX_RUNNING,
	RF_TX_RUNNING,
	RF_CAD,
}RadioState_t;

/**
 * state for external world
 */
typedef enum
{
	RADIO_IDLE = 0,
	RX_TIMEOUT,
	TX_TIMEOUT,
	RX_DONE,
	TX_DONE,
	RX_ERROR,
	FHSS_CHANGE_CHANNEL,
	CAD_DONE
}RadioStateExt_t;

/*!
 * Radio FSK modem parameters
 */
typedef struct RadioFskSettings_t
{
	int8_t   Power;
	uint32_t Fdev;
	uint32_t Bandwidth;
	uint32_t BandwidthAfc;
	uint32_t Datarate;
	uint16_t PreambleLen;
	bool     FixLen;
	uint8_t  PayloadLen;
	bool     CrcOn;
	bool     IqInverted;
	bool     RxContinuous;
	uint32_t TxTimeout;
} RadioFskSettings_t;

/*!
 * Radio FSK packet handler state
 */
typedef struct RadioFskPacketHandler_t
{
	uint8_t  PreambleDetected;
	uint8_t  SyncWordDetected;
	int8_t   RssiValue;
	int32_t  AfcValue;
	uint8_t  RxGain;
	uint16_t Size;
	uint16_t NbBytes;
	uint8_t  FifoThresh;
	uint8_t  ChunkSize;
} RadioFskPacketHandler_t;

/*!
 * Radio LoRa modem parameters
 */
typedef struct RadioLoRaSettings_t
{
	int8_t   Power;
	uint32_t Bandwidth;
	uint32_t Datarate;
	bool     LowDatarateOptimize;
	uint8_t  Coderate;
	uint16_t PreambleLen;
	bool     FixLen;
	uint8_t  PayloadLen;
	bool     CrcOn;
	bool     FreqHopOn;
	uint8_t  HopPeriod;
	bool     IqInverted;
	bool     RxContinuous;
	uint32_t TxTimeout;
} RadioLoRaSettings_t;

/*!
 * Radio LoRa packet handler state
 */
typedef struct RadioLoRaPacketHandler_t
{
	int8_t SnrValue;
	int16_t RssiValue;
	uint8_t Size;
} RadioLoRaPacketHandler_t;

/*!
 * Radio Settings
 */
typedef struct RadioSettings_t
{
	RadioState_t              State;
	RadioModems_t             Modem;
	uint32_t                  Channel;
	struct RadioFskSettings_t*       Fsk;
	struct RadioFskPacketHandler_t*  FskPacketHandler;
	struct RadioLoRaSettings_t*      LoRa;
	struct RadioLoRaPacketHandler_t* LoRaPacketHandler;
} RadioSettings_t;

typedef struct _sx1276_context{
	mraa_gpio_context        gpio_ss;
	mraa_gpio_context        gpio_reset;
	mraa_gpio_context        gpio_dio0;
	uint8_t                  _ss;
	uint8_t                  _reset;
	uint8_t                  _dio0;
	mraa_gpio_context        DIO1;
	mraa_spi_context         spi;
	int                      _frequency;
	int                      _packetIndex;
	int                      _implicitHeaderMode;
	uint8_t                  RxTxBuffer[RX_BUFFER_SIZE];
	struct RadioSettings_t*  Settings;
} *sx1276_context;

/**
 * Function to initialize the sx1276 driver and return the driver
 * context.
 *
 * @param ss chip select pin
 * @param reset reset pin
 * @param dio0 dio0 pin
 * @param frequency initial frequency to be set
 *
 * @return sx1276_context
 */
sx1276_context init(int ss, int reset, int dio0, long frequency);

/**
 * Function that is puts the device to sleep
 * to be called at the end of a transmission or when the functionality
 * provided by the device is no longer required
 */
void end(sx1276_context SX1276);

/**
 * Function that signals the beginning of a particular transmission
 *
 * @param sx1276_context device context
 */
int beginPacket(sx1276_context SX1276, int implicitHeader);

/**
 * Function that signifies the end of a packet being transmitted
 *
 * @param sx1276_context device context
 */
int endPacket(sx1276_context SX1276);

/**
 * Function to parse the received packet
 *
 * @param sx1276_context device context
 * @param size size of the packet received
 */
int parsePacket(sx1276_context SX1276, int size);

/**
 * Function to calculate the RSSI of the received packet
 */
int packetRssi(sx1276_context SX1276, RadioModems_t modem);

/**
 * Function to calculate the SNR of a particular received packet
 */
float packetSnr(sx1276_context SX1276);

/**
 * A writeFIFO implementation for the driver
 *
 * @param sx1276_context device context
 * @param buffer buffer to be written into the FIFO
 * @param size Size of the buffer to be written
 */
int write_buf(sx1276_context SX1276, const uint8_t *buffer, size_t size);

/**
 * A function for internal driver use
 */
int available(sx1276_context SX1276);

/**
 * A readFIFO implementation for the driver
 */
int SX1276read(sx1276_context SX1276);

/**
 * Legacy function. To be removed in a future implementation
 */
int peek(sx1276_context SX1276);

/**
 * Legacy function. To be removed in a future implementation
 */
void flush(sx1276_context SX1276);

/**
 * Legacy function. To be removed in a future implementation
 */
void onReceive(void(*callback)(int));

/**
 * Legacy function. To be removed in a future implementation
 */
void receive(sx1276_context SX1276, int size);

/**
 * Legacy function. To be removed in a future implementation
 */
void idle(sx1276_context SX1276);

/**
 * Function to put the SX1276 to sleep
 */
void SX1276sleep(sx1276_context SX1276);

/**
 * Function to set the transmitting power for the device
 *
 * @param sx1276_context device context
 * @param level power level to be set
 */
void setTxPower(sx1276_context SX1276, int level, int outputPin);

/**
 * Function to set the frequency of the device
 */
void setFrequency(sx1276_context SX1276, long frequency);

/**
 * Legacy function. To be removed in a future implementation
 */
void setSpreadingFactor(sx1276_context SX1276, int sf);

/**
 * Legacy function. To be removed in a future implementation
 */
void setSignalBandwidth(sx1276_context SX1276, long sbw);

/**
 * Legacy function. To be removed in a future implementation
 */
void setCodingRate4(sx1276_context SX1276, int denominator);

/**
 * Legacy function. To be removed in a future implementation
 */
void setPreambleLength(sx1276_context SX1276, long length);

/**
 * Legacy function. To be removed in a future implementation
 */
void setSyncWord(sx1276_context SX1276, int sw);

/**
 * Legacy function. To be removed in a future implementation
 */
void crc(sx1276_context SX1276);

/**
 * Legacy function. To be removed in a future implementation
 */
void noCrc(sx1276_context SX1276);

/**
 * Function to generate a random uint8_t value based on the
 * retrieved RSSI
 */
int8_t random(sx1276_context SX1276);

/**
 * Legacy function. To be removed in a future implementation
 */
void setSPIFrequency(sx1276_context SX1276, uint32_t frequency);

/**
 * Dump values of all the register for sx1276
 */
void dumpRegisters(sx1276_context SX1276);

/**
 * Legacy function. To be removed in a future implementation
 */
void explicitHeaderMode(sx1276_context SX1276);

/**
 * Legacy function. To be removed in a future implementation
 */
void implicitHeaderMode(sx1276_context SX1276);

/**
 * Legacy function. To be removed in a future implementation
 */
void handleDio0Rise(sx1276_context SX1276);

/**
 * Function to read the value of a particular sx1276 register
 *
 * @param sx1276_context device context
 * @param address Address of the register to be read
 *
 * @return Value read from the register
 */
uint8_t readRegister(sx1276_context SX1276, uint8_t address);

/**
 * Function to write an 8 bit value to a register
 *
 * @param sx1276_context device context
 * @param address Address of register to be read
 * @param value Value to be written to register
 */
void writeRegister(sx1276_context SX1276, uint8_t address, uint8_t value);

/**
 * Legacy function. To be removed in a future implementation
 */
uint8_t singleTransfer(sx1276_context SX1276, uint8_t address, uint8_t value);

/**
 * Function to set modem for the device: FSK or LoRa
 *
 * @param sx1276_context device context
 * @param modem FSK/LoRa
 */
void SX1276SetModem(sx1276_context SX1276, RadioModems_t modem);

/**
 * Sets operation mode of the device
 *
 * @param sx1276_context device context
 * @param mode operation mode for the device
 */
void SX1276SetOpMode(sx1276_context SX1276, uint8_t mode);

/**
 * Function to handle RX Chain Calibration
 */
void SX1276RxChainCalibration(sx1276_context SX1276);

/**
 * Sets the channels configuration and tells if channel is free
 */
bool SX1276IsChannelFree(sx1276_context SX1276, RadioModems_t modem, uint32_t freq, int16_t rssiThresh);

/**
 * Function to generate a random 32 bit value based on RSSI
 */
uint32_t SX1276Random(sx1276_context SX1276);

/**
 * Sets the reception parameters
 */
void SX1276SetRxConfig(sx1276_context SX1276, RadioModems_t modem, uint32_t bandwidth,
		uint32_t datarate, uint8_t coderate,
		uint32_t bandwidthAfc, uint16_t preambleLen,
		uint16_t symbTimeout, bool fixLen,
		uint8_t payloadLen,
		bool crcOn, bool freqHopOn, uint8_t hopPeriod,
		bool iqInverted, bool rxContinuous);

/**
 * Sets the transmission parameters
 */
void SX1276SetTxConfig(sx1276_context SX1276, RadioModems_t modem, int8_t power, uint32_t fdev,
		uint32_t bandwidth, uint32_t datarate,
		uint8_t coderate, uint16_t preambleLen,
		bool fixLen, bool crcOn, bool freqHopOn,
		uint8_t hopPeriod, bool iqInverted, uint32_t timeout);

/**
 * Computes the packet time on air in ms for the given payload
 */
uint32_t SX1276GetTimeOnAir(sx1276_context SX1276, RadioModems_t modem, uint8_t pktLen);

/**
 * Sends the buffer of size. Prepares the packet to be sent and sets
 * the radio in transmission
 */
void SX1276Send(sx1276_context SX1276, uint8_t *buffer, uint8_t size);

/**
 * Sets the radio in continuous wave transmission mode
 */
void SX1276SetTx(sx1276_context SX1276, uint32_t timeout);

/**
 * Sets the radio in reception mode for the given time
 */
RadioStateExt_t SX1276SetRx(sx1276_context SX1276, uint32_t timeout);

/**
 * Start a Channel Activity Detection
 */
void SX1276StartCad(sx1276_context SX1276);

/**
 * Sets the radio in continuous wave transmission mode
 */
void SX1276SetTxContinuousWave(sx1276_context SX1276, uint32_t freq, int8_t power, uint16_t time);

/**
 * Sets the maximum payload length.
 */
void SX1276SetMaxPayloadLength(sx1276_context SX1276, RadioModems_t modem, uint8_t max);

/**
 * Sets the network to public or private. Updates the sync byte.
 */
void SX1276SetPublicNetwork(sx1276_context SX1276, bool enable);

/**
 * Function to be called on the occurance of a timeout
 */
void SX1276OnTimeoutIrq(sx1276_context SX1276);

/**
 * Function to be called if DIO0 occurs
 */
void SX1276OnDio0Irq(sx1276_context SX1276);

/**
 * Function to be called if DIO1 occurs
 */
void SX1276OnDio1Irq(sx1276_context SX1276);

/**
 * Function to be called if DIO2 occurs
 */
void SX1276OnDio2Irq(sx1276_context SX1276);

/**
 * Returns length of the received packet
 */
uint8_t SX1276GetRxBufferlen(sx1276_context SX1276);

/**
 * Returns the received buffer
 */
void SX1276GetRxBuffer(sx1276_context SX1276, uint8_t* buf);
