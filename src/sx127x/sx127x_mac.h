#pragma once

#include "upm.h"
#include "mraa.h"
#include "sx127x.h"

#define RECEIVE_DELAY1            1        // s
#define RECEIVE_DELAY2            2        // s (must be RECEIVE_DELAY1 + 1s)
#define JOIN_ACCEPT_DELAY1        5        // s
#define JOIN_ACCEPT_DELAY2        6        // s
#define MAX_FCNT_GAP              16384
#define ADR_ACK_LIMIT             64
#define ADR_ACK_DELAY             32
#define ACK_TIMEOUT               2        // +/-1 s

#define MULTITECH 1
#define MAX_PAYLOAD_SIZE 255

/*!
 * Sets the length of the LoRaMAC footer field.
 * Mainly indicates the MIC field length
 */
#define LORAMAC_MFR_LEN           4

// this is specific to the US 915 standard
#define LORA_MAX_NB_CHANNELS      72
#define LORA_MIN_NB_CHANNELS      0

#define LORAWAN_OTAA_JOIN_PACK_LEN 23

#define LORAMAC_DEBUG 1

// vv imp
/*!
 * LoRaMac datarates definition
 */
#define DR_0                                        0  // SF10 - BW125 |
#define DR_1                                        1  // SF9  - BW125 |
#define DR_2                                        2  // SF8  - BW125 +-> Up link
#define DR_3                                        3  // SF7  - BW125 |
#define DR_4                                        4  // SF8  - BW500 |
#define DR_5                                        5  // RFU
#define DR_6                                        6  // RFU
#define DR_7                                        7  // RFU
#define DR_8                                        8  // SF12 - BW500 |
#define DR_9                                        9  // SF11 - BW500 |
#define DR_10                                       10 // SF10 - BW500 |
#define DR_11                                       11 // SF9  - BW500 |
#define DR_12                                       12 // SF8  - BW500 +-> Down link
#define DR_13                                       13 // SF7  - BW500 |
#define DR_14                                       14 // RFU          |
#define DR_15                                       15 // RFU          |

#define UP_LINK 0
#define DOWN_LINK 1

#define BASE_TX_POWER  30

typedef enum lorawan_msg_type {
	JOIN_TX,
	JOIN_RX,
	TX,
	RX_1,
	RX_2
}LoRaWanMsg_t;

typedef enum lorawan_error_types {
	LORAMAC_JOIN_SUCCESS,
	LORAMAC_JOIN_FAILURE,
	LORAMAC_TRANSMIT_SUCCESS,
	LORAMAC_TRANSMIT_FAILURE,
	LORAMAC_RECEIVE_SUCCESS,
	LORAMAC_RECEIVE_FAILURE,
	LORAMAC_GENERAL_FAILURE,
	LORAMAC_RX_TIMEOUT,
	LORAMAC_TX_TIMEOUT
}LoRaWanResult_t;

/*!
 * LoRaMAC frame types
 *
 * LoRaWAN Specification V1.0.1, chapter 4.2.1, table 1
 */
typedef enum eLoRaMacFrameType
{
    /*!
     * LoRaMAC join request frame
     */
    FRAME_TYPE_JOIN_REQ              = 0x00,
    /*!
     * LoRaMAC join accept frame
     */
    FRAME_TYPE_JOIN_ACCEPT           = 0x01,
    /*!
     * LoRaMAC unconfirmed up-link frame
     */
    FRAME_TYPE_DATA_UNCONFIRMED_UP   = 0x02,
    /*!
     * LoRaMAC unconfirmed down-link frame
     */
    FRAME_TYPE_DATA_UNCONFIRMED_DOWN = 0x03,
    /*!
     * LoRaMAC confirmed up-link frame
     */
    FRAME_TYPE_DATA_CONFIRMED_UP     = 0x04,
    /*!
     * LoRaMAC confirmed down-link frame
     */
    FRAME_TYPE_DATA_CONFIRMED_DOWN   = 0x05,
    /*!
     * LoRaMAC RFU frame
     */
    FRAME_TYPE_RFU                   = 0x06,
    /*!
     * LoRaMAC proprietary frame
     */
    FRAME_TYPE_PROPRIETARY           = 0x07,
}LoRaMacFrameType_t;

/**
 * Enum for all the possible MAC commands
 */
typedef enum eLoRaMacEDCommands {
	LINK_CHECK_REQ        = 0X02,
	LINK_ADR_ANS          = 0X03,
	DUTY_CYCLE_ANS        = 0X04,
	RX_PARAM_SETUP_ANS    = 0X05,
	DEV_STATUS_ANS        = 0X06,
	NEW_CHANNEL_ANS       = 0X07,
	RX_TIMING_SETUP_ANS   = 0X08
} loRaMacEDCommands_t;

typedef enum eLoRaMacGWCommands {
	LINK_CHECK_ANS        = 0X02,
	LINK_ADR_REQ          = 0X03,
	DUTY_CYCLE_REQ        = 0X04,
	RX_PARAM_SETUP_REQ    = 0X05,
	DEV_STATUS_REQ        = 0X06,
	NEW_CHANNEL_REQ       = 0X07,
	RX_TIMING_SETUP_REQ   = 0X08
} loRaMacGWCommands_t;

/*!
 * LoRaMAC channels parameters definition
 */
typedef struct DrRange_t
{
    /*!
     * Byte-access to the bits
     */
    //int8_t Value;
    /*!
     * Structure to store the minimum and the maximum datarate
     */
    //struct sFields
    //{
         /*!
         * Minimum data rate
         *
         * EU868 - [DR_0, DR_1, DR_2, DR_3, DR_4, DR_5, DR_6, DR_7]
         *
         * US915 - [DR_0, DR_1, DR_2, DR_3, DR_4]
         */
        int8_t Min;
        /*!
         * Maximum data rate
         *
         * EU868 - [DR_0, DR_1, DR_2, DR_3, DR_4, DR_5, DR_6, DR_7]
         *
         * US915 - [DR_0, DR_1, DR_2, DR_3, DR_4]
         */
        int8_t Max;
    //}Fields;
} DrRange_t;

/*!
 * LoRaMAC channel definition
 */
typedef struct ChannelParams_t
{
    /*!
     * Frequency in Hz
     */
    uint32_t Frequency;
    /*!
     * Data rate definition
     */
    //struct DrRange_t* DrRange;
    /*!
     * Band index
     */
    uint8_t Band;
} ChannelParams_t;

typedef union FCtrl_t {
    uint8_t fctrl;

    struct fCtrlFields_t {
        uint8_t FOptsLen : 4;
        uint8_t FPending : 1;
        uint8_t ACK : 1;
        uint8_t ADRACKReq : 1;
        uint8_t ADR : 1;
    } fCtrlFields_t;
} FCtrl_t;

typedef struct FHDRParams_t
{
    /*!
     * Device Address as assigned by the GW
     */
    uint32_t devAddr;
    /*!
     * F Control
     */
    union FCtrl_t* FCtrl;
    /*!
     * Uplink Frame counter
     */
    uint16_t FCounter;
    uint8_t FOpts[16];
} FHDRParams_t;

typedef struct MHDR_t {
    uint8_t type;
    uint8_t RFU;
    uint8_t Major;
} MHDR_t;

typedef struct loRaWANParams_t {
	int8_t uplink_datarate;                           // this can be between DR_0 through DR_4
	int8_t downlink_datarate_1;                       // this is a function of the uplink datarate
	int8_t downlink_datarate_2;                // as per spec this datarate is fixed
	int8_t req_datarate;
	int8_t uplink_power;
	uint8_t Rx1DrOffset;                              // DR offset for downlink, usually it is 0
	uint32_t ReceiveDelay1;                           // receive window delays, these values are in ms
	uint32_t ReceiveDelay2;
	uint16_t FCnt_up;                                 // Frame counters - up and down
	uint16_t FCnt_down;
	uint32_t LoRaMacNetID;
	uint32_t LoRaMacDevAddr;
} loRaWANParams_t;

typedef struct loRaWANPayloadParams_t {
	uint8_t tx_frm_payload[MAX_PAYLOAD_SIZE];
	uint8_t rx_frm_payload[MAX_PAYLOAD_SIZE];
} loRaWANPayloadParams_t;

typedef struct loRaWANSecurityParams_t {
	uint8_t DEVEUI[8];
	uint8_t APPEUI[8];
	uint8_t APPKEY[16];
	uint8_t NwkSKey[16];
	uint8_t AppSKey[16];
} loRaWANSecurityParams_t;

typedef struct loRaWANBandParams_t {
	struct ChannelParams_t* Channels[LORA_MAX_NB_CHANNELS];
	int currentChannel;
	uint8_t channelCounter;
	uint8_t maxChannelCounter;
	int rxFrequencies[8];
	uint16_t channelMask[6];
	uint8_t randChannelArr[72];
	int nbJoinTries;
} loRaWANBandParams_t;

typedef struct loRaWANMACCommand_t {
	union FCtrl_t* fctrl_tx;
	uint8_t macCommandLen_tx;
	uint8_t macCommandLen_rx;
	uint8_t macCommandFoptsBuf_tx[16];
	uint8_t macCommandFoptsBuf_rx[16];
} loRaWANMACCommand_t;

typedef struct loRaWANOTAAJoin_t {
	uint8_t lorawan_otaa_join_pack[LORAWAN_OTAA_JOIN_PACK_LEN];
	uint16_t devNonce;
} loRaWANOTAAJoin_t;

typedef struct _loRaWAN_context {
	//struct DrRange_t* DrRange;
	//struct ChannelParams_t* ChannelParams;
	struct FHDRParams_t* FHDRParams;
	struct MHDR_t* MHDR;
    struct loRaWANParams_t* loRaWANParams;
    struct loRaWANPayloadParams_t* loRaWANPayloadParams;
    struct loRaWANSecurityParams_t* loRaWANSecurityParams;
    struct loRaWANBandParams_t* loRaWANBandParams;
    struct loRaWANMACCommand_t* loRaWANMACCommand;
    struct loRaWANOTAAJoin_t* loRaWANOTAAJoin;
    sx1276_context sx1276_dev;
    uint8_t lorawan_tx_packet[256];
    uint8_t rx_buffer[256];
} *loRaWAN_context;

//static MHDR_t MHDR;

loRaWAN_context lorawan_init();

upm_result_t lorawan_otaa_join(loRaWAN_context lorawan_dev);

uint8_t lorawan_set_random_channel(loRaWAN_context lorawan_dev);

void lorawan_create_package(loRaWAN_context lorawan_dev);

void lorawan_set_dev_eui(loRaWAN_context lorawan_dev, uint8_t* dev_eui);

void lorawan_set_app_eui(loRaWAN_context lorawan_dev, uint8_t* app_eui);

void lorawan_set_app_key(loRaWAN_context lorawan_dev, uint8_t* app_key);

void onRxDone(loRaWAN_context lorawan_dev, uint8_t *payload, uint16_t size);

LoRaWanResult_t lorawan_transmit_packet(loRaWAN_context lorawan_dev, uint8_t* buf, uint16_t len);

LoRaWanResult_t lorawan_schedule_tx(loRaWAN_context lorawan_dev, int size);

LoRaWanResult_t lorawan_initiate_rx(loRaWAN_context lorawan_dev, LoRaWanMsg_t type);

void addEDMacCommand(loRaWAN_context lorawan_dev, loRaMacEDCommands_t cmd, uint8_t len, uint8_t* resp);

void addGWMacResponse(loRaWAN_context lorawan_dev, loRaMacGWCommands_t cmd);

void parseGWMacCommands(loRaWAN_context lorawan_dev, int size);

void generateRandomChannelArray(loRaWAN_context lorawan_dev, int size);

int nextChannelValid(loRaWAN_context lorawan_dev, int channel);

int setNextChannel(loRaWAN_context lorawan_dev);

void shuffleArray(loRaWAN_context lorawan_dev, uint8_t* arr, size_t n);

void cleanUpChannels(loRaWAN_context lorawan_dev);
