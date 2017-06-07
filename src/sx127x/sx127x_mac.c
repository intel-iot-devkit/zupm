/*
 * Author: Abhishek Malik <abhishek.malik@intel.com>
 *
 * This code is heavily based on the semtech lora library at:
 * https://github.com/Lora-net/LoRaMac-node
 * which is licensed under the BSD license at:
 * upm/src/sx127x/LICENSE.txt
 */

#include "sx127x_mac.h"
#include "zephyr.h"
#include <sys_clock.h>
#include "kernel.h"
#include "device.h"

#include "sx127x.h"
#include "unistd.h"
#include "mraa.h"
#include "loramac-definitions.h"
#include "loramaccrypto.h"
#include "aes.h"

static int8_t dr_table[16][2] =
{
    {10, 0},           // DR_0
    {9, 0},            // DR_1
    {8, 0},            // DR_2
    {7, 0},            // DR_3
    {8, 2},            // DR_4
    {-1, -1},          // DR_5
    {-1, -1},          // DR_6
    {-1, -1},          // DR_7
    {12, 2},           // DR_8
    {11, 2},           // DR_9
    {10, 2},           // DR_10
    {9, 2},            // DR_11
    {8, 2},            // DR_12
    {7, 2},            // DR_13
    {-1, -1},          // DR_14
    {-1, -1}           // DR_15
};

/*!
 * Up/Down link data rates offset definition
 */
static int8_t data_rate_offsets[5][4] =
{
    { DR_10, DR_9 , DR_8 , DR_8  }, // DR_0
    { DR_11, DR_10, DR_9 , DR_8  }, // DR_1
    { DR_12, DR_11, DR_10, DR_9  }, // DR_2
    { DR_13, DR_12, DR_11, DR_10 }, // DR_3
    { DR_13, DR_13, DR_12, DR_11 }, // DR_4
};

typedef struct DrRange_t* DrRange_ptr;
typedef struct ChannelParams_t* ChannelParams_ptr;
typedef struct FHDRParams_t* FHDRParams_ptr;
typedef struct MHDR_t* MHDR_ptr;
typedef struct loRaWANParams_t* loRaWANParams_ptr;
typedef struct loRaWANPayloadParams_t* loRaWANPayloadParams_ptr;
typedef struct loRaWANSecurityParams_t* loRaWANSecurityParams_ptr;
typedef struct loRaWANBandParams_t* loRaWANBandParams_ptr;
typedef union FCtrl_t* FCtrl_ptr;
typedef union FCtrl_t* FCtrl_ptr_2;
typedef struct loRaWANMACCommand_t* loRaWANMACCommand_ptr;
typedef struct loRaWANOTAAJoin_t* loRaWANOTAAJoin_ptr;

LoRaWanMsg_t msgType;

// this can be between DR_0 through DR_4
int8_t uplink_datarate;
int8_t req_datarate;
// this is a function of the uplink datarate
int8_t downlink_datarate_1;
// as per spec this datarate is fixed
int8_t downlink_datarate_2 = DR_8;

int8_t uplink_power;

// DR offset for downlink, usually it is 0
uint8_t Rx1DrOffset;

// receive window delays, these values are in ms
uint32_t ReceiveDelay1;
uint32_t ReceiveDelay2;

// Frame counters - up and down
uint16_t FCnt_up;
uint16_t FCnt_down;

// arrays to store the encrypted messages
uint8_t tx_frm_payload[MAX_PAYLOAD_SIZE] = {0};
uint8_t rx_frm_payload[MAX_PAYLOAD_SIZE] = {0};

/*!
 * LoRaMAC channels
 */
ChannelParams_t Channels[LORA_MAX_NB_CHANNELS];

// testing
//static aes_context AesContext;

uint16_t devNonce;

uint8_t lorawan_otaa_join_pack[LORAWAN_OTAA_JOIN_PACK_LEN];
uint8_t lorawan_tx_packet[256];

//static uint8_t rx_buffer[256] = {0};
//static uint8_t rx_dec_buffer[256] = {0};

uint8_t DEVEUI[8] = {0x00};

uint8_t APPEUI[8] = {0x00};

uint8_t APPKEY[16] = {0x00};

/*!
 * Network ID ( 3 bytes )
 */
uint32_t LoRaMacNetID = 0;

/*!
 * Mote Address
 */
uint32_t LoRaMacDevAddr = 0;

/*!
 * AES encryption/decryption cipher network session key
 */
uint8_t NwkSKey[] =
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/*!
 * AES encryption/decryption cipher application session key
 */
uint8_t AppSKey[] =
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// channel array to be randomized
uint8_t randChannelArr[72];

int currentChannel;

uint8_t channelCounter;
uint8_t maxChannelCounter;

int nbJoinTries;

bool joinSuccessful;

int rxFrequencies[] = {923.3e6, 923.9e6, 924.5e6, 925.1e6, 925.7e6, 926.3e6, 926.9e6, 927.5e6};

/**
 * MAC Command specifics
 */
FCtrl_t fctrl_tx;

uint8_t macCommandLen_tx;

uint8_t macCommandLen_rx;

uint8_t macCommandFoptsBuf_tx[16];

uint8_t macCommandFoptsBuf_rx[16];

uint16_t channeMask[6];

loRaWAN_context lorawan_init() {
    printf("coming into lorawan_init\n");

    DrRange_ptr DrRange = (DrRange_ptr) malloc(sizeof(DrRange_t));
    if(DrRange == NULL) {
    	printf("Unable to initialize Datarate Range\n");
    	return NULL;
    }

    FHDRParams_ptr FHDRParams = (FHDRParams_ptr) malloc(sizeof(FHDRParams_t));
    if(FHDRParams == NULL) {
    	printf("Unable to initialize the FHDR params\n");
    	return NULL;
    }

    MHDR_ptr MHDR = (MHDR_ptr) malloc(sizeof(MHDR_t));
    if(MHDR == NULL) {
    	printf("Unable to initialize the MHDR Params\n");
    	return NULL;
    }

    loRaWANParams_ptr loRaWANParams = (loRaWANParams_ptr) malloc(sizeof(loRaWANParams_t));
    if(loRaWANParams == NULL) {
    	printf("Unable to initialize loRaWANParams\n");
    	return NULL;
    }

    loRaWANPayloadParams_ptr loRaWANPayloadParams = (loRaWANPayloadParams_ptr) malloc(sizeof(loRaWANPayloadParams_t));
    if(loRaWANPayloadParams == NULL) {
    	printf("Unable to initialize loRaWANPayloadParams\n");
    	return NULL;
    }

    loRaWANSecurityParams_ptr loRaWANSecurityParams = (loRaWANSecurityParams_ptr) malloc(sizeof(loRaWANSecurityParams_t));
    if(loRaWANSecurityParams == NULL) {
    	printf("Unable to initialize loRaWANSecurityParams\n");
    	return NULL;
    }

    loRaWANBandParams_ptr loRaWANBandParams = (loRaWANBandParams_ptr) malloc(sizeof(loRaWANBandParams_t));
    if(loRaWANBandParams == NULL) {
    	printf("Unable to initialize loRaWANBandParams\n");
    	return NULL;
    }

    loRaWANMACCommand_ptr loRaWANMACCommand = (loRaWANMACCommand_ptr) malloc(sizeof(loRaWANMACCommand_t));
    if(loRaWANMACCommand == NULL) {
    	printf("Unable to initialize loRaWANMACCommand\n");
    	return NULL;
    }

    loRaWANOTAAJoin_ptr loRaWANOTAAJoin = (loRaWANOTAAJoin_ptr) malloc(sizeof(loRaWANOTAAJoin_t));
    if(loRaWANOTAAJoin == NULL) {
    	printf("Unable to initialize loRaWANOTAAJoin\n");
    	return NULL;
    }

    FCtrl_ptr FCtrl = (FCtrl_ptr) malloc(sizeof(FCtrl_t));
    if(FCtrl == NULL) {
    	printf("Unable to initialize FCtrl\n");
    	return NULL;
    }

    FCtrl_ptr_2 FCtrl_mac = (FCtrl_ptr_2) malloc(sizeof(FCtrl_t));
    if(FCtrl_mac == NULL) {
    	printf("Unable to initialize FCtrl\n");
    	return NULL;
    }

    FHDRParams->FCtrl = FCtrl;
    loRaWANMACCommand->fctrl_tx = FCtrl_mac;

    loRaWAN_context lora_dev = (loRaWAN_context) malloc(sizeof(struct _loRaWAN_context));
    if(lora_dev == NULL) {
    	printf("Unable to initialize _loRaWAN_context\n");
    	return NULL;
    }

    lora_dev->sx1276_dev = init(60, 82, 72, 915e6);
    printf("device init\n");

    for(uint8_t i = 0; i < LORA_MAX_NB_CHANNELS; i++) {
    	loRaWANBandParams->Channels[i] = (ChannelParams_ptr) malloc(sizeof(ChannelParams_t));
        if(loRaWANBandParams->Channels[i] == NULL) {
        	printf("Unable to initialize the channel: %d\n", i);
        	return NULL;
        }
    }

    //ChannelParams->DrRange = DrRange;

    //loRaWANBandParams->Channels = ChannelParams;

    //lora_dev->DrRange = DrRange;
    lora_dev->FHDRParams = FHDRParams;
    lora_dev->MHDR = MHDR;
    lora_dev->loRaWANBandParams = loRaWANBandParams;
    lora_dev->loRaWANParams = loRaWANParams;
    lora_dev->loRaWANPayloadParams = loRaWANPayloadParams;
    lora_dev->loRaWANSecurityParams = loRaWANSecurityParams;
    //lora_dev->ChannelParams = ChannelParams;
    lora_dev->loRaWANMACCommand = loRaWANMACCommand;
    lora_dev->loRaWANOTAAJoin = loRaWANOTAAJoin;
    //lora_dev->sx1276_dev = dev;
    //LoRaClass();
    //setPins(60, 82, 72);

    //init(9041e5);

    /*!
	 * LoRaMac default channels
	 */
    // Channel = { Frequency [Hz], { ( ( DrMax << 4 ) | DrMin ) }, Band }
    /*
	 * US band channels are initialized using a loop in LoRaMacInit function
	 * \code
	 */
	// 125 kHz channels

	for( uint8_t i = 0; i < LORA_MAX_NB_CHANNELS - 8; i++ ) {
		lora_dev->loRaWANBandParams->Channels[i]->Frequency = 902.3e6 + i * 200e3;
		//lora_dev->loRaWANBandParams->Channels->DrRange->Min = DR_0;
		//lora_dev->loRaWANBandParams->Channels->DrRange->Max = DR_3;
		lora_dev->loRaWANBandParams->Channels[i]->Band = 0;
	}

	for( uint8_t i = LORA_MAX_NB_CHANNELS - 8; i < LORA_MAX_NB_CHANNELS; i++ ) {
		lora_dev->loRaWANBandParams->Channels[i]->Frequency = 903.0e6 + ( i - ( LORA_MAX_NB_CHANNELS - 8 ) ) * 1.6e6;
		//lora_dev->loRaWANBandParams->Channels->DrRange->Min = DR_4;
		//lora_dev->loRaWANBandParams->Channels->DrRange->Max = DR_4;
		lora_dev->loRaWANBandParams->Channels[i]->Band = 0;
	}

    // Mac Commands defaults
	lora_dev->loRaWANMACCommand->fctrl_tx = 0;
    lora_dev->loRaWANMACCommand->macCommandLen_rx = 0;
    lora_dev->loRaWANMACCommand->macCommandLen_tx = 0;
    lora_dev->loRaWANParams->uplink_datarate = DR_0;
    lora_dev->loRaWANParams->req_datarate = DR_0;
    lora_dev->loRaWANParams->uplink_power = 20;

    // setting current default channel
    lora_dev->loRaWANBandParams->currentChannel = 0;
	//currentChannel = 0;

	// initializing channel mask
    lora_dev->loRaWANBandParams->channelMask[0] = 0x0000;
    lora_dev->loRaWANBandParams->channelMask[1] = 0xff00;
    lora_dev->loRaWANBandParams->channelMask[2] = 0x0000;
    lora_dev->loRaWANBandParams->channelMask[3] = 0x0000;
    lora_dev->loRaWANBandParams->channelMask[4] = 0x0000;
    lora_dev->loRaWANBandParams->channelMask[5] = 0x0000;

	// setting up the random array with not so random values
	for(uint8_t i = 0; i < 72; i++) {
		//randChannelArr[i] = i;
		lora_dev->loRaWANBandParams->randChannelArr[i] = i;
	}

	generateRandomChannelArray(lora_dev, 72);
	cleanUpChannels(lora_dev);

	for(uint8_t i = 0; i < 72; i++) {
		printf("%d, ", lora_dev->loRaWANBandParams->randChannelArr[i]);
	}
	printf("\n");
	lora_dev->loRaWANBandParams->channelCounter = 0;
	//channelCounter = 0;
	lora_dev->loRaWANBandParams->maxChannelCounter = 72;
	//maxChannelCounter = 72;
	lora_dev->loRaWANBandParams->nbJoinTries = 0;
	//nbJoinTries = 0;
	// not sure where to put something like join successful
	joinSuccessful = false;
    printf("init done\n");

    return lora_dev;
}

uint8_t lorawan_set_random_channel(loRaWAN_context lorawan_dev) {
    return (rand() % ((LORA_MAX_NB_CHANNELS-1) + 1 - LORA_MIN_NB_CHANNELS)) + LORA_MIN_NB_CHANNELS;
}

void generateRandomChannelArray(loRaWAN_context lorawan_dev, int size) {
	int j = 0, temp;
	for(uint8_t i = 0; i < size; i++) {
        j = i + rand() % (size - i);
        temp = lorawan_dev->loRaWANBandParams->randChannelArr[i];
        lorawan_dev->loRaWANBandParams->randChannelArr[i] = lorawan_dev->loRaWANBandParams->randChannelArr[j];
        lorawan_dev->loRaWANBandParams->randChannelArr[j] = temp;
        //temp = randChannelArr[i];
        //randChannelArr[i] = randChannelArr[j];
        //randChannelArr[j] = temp;
	}
}

void cleanUpChannels(loRaWAN_context lorawan_dev) {
	int arrCounter = 0;
	for(uint8_t c = 0; c < 5; c++){
		for(uint8_t x = 0;x<16;x++) {
			if(((lorawan_dev->loRaWANBandParams->channelMask[c]>>x)&0x0001) == 1) {
				lorawan_dev->loRaWANBandParams->randChannelArr[arrCounter++] = c*16 + x;
			}
			//if(((channeMask[c]>>x)&0x0001) == 1) {
			//	randChannelArr[arrCounter++] = c*16 + x;
			//}
		}
	}
	// size here will become arrCounter
	// we will randomize it after this
	for (uint8_t c = arrCounter; c<LORA_MAX_NB_CHANNELS; c++) {
		lorawan_dev->loRaWANBandParams->randChannelArr[c] = 0;
	}
	printf("Array Counter: %d\n", arrCounter);
	generateRandomChannelArray(lorawan_dev, arrCounter);
	lorawan_dev->loRaWANBandParams->maxChannelCounter = arrCounter;
	//maxChannelCounter = arrCounter;
}

int setNextChannel(loRaWAN_context lorawan_dev) {
    int tryChannel = 0;
	while(1) {
		if(lorawan_dev->loRaWANBandParams->channelCounter < lorawan_dev->loRaWANBandParams->maxChannelCounter) {
		//if(channelCounter < maxChannelCounter) {
			tryChannel = lorawan_dev->loRaWANBandParams->randChannelArr[lorawan_dev->loRaWANBandParams->channelCounter];
			lorawan_dev->loRaWANBandParams->channelCounter++;
			//channelCounter++;

            if(nextChannelValid(lorawan_dev, tryChannel) != UPM_SUCCESS)
			    continue;
		    else
		 	    break;
		} else {
			lorawan_dev->loRaWANBandParams->channelCounter = 0;
			//channelCounter = 0;
	    }
	}

	return tryChannel;
}

int nextChannelValid(loRaWAN_context lorawan_dev, int channel) {
	printf("Trying channel number: %d\n", channel);
	uint8_t sub_band_bit = channel%16;
	uint8_t sub_band_number = channel/16;
	if(((lorawan_dev->loRaWANBandParams->channelMask[sub_band_number]>>sub_band_bit)&0x0001) == 1) {
	//if(((channeMask[sub_band_number]>>sub_band_bit)&0x0001) == 1) {
        return UPM_SUCCESS;
	}

	printf("******** CHANNEL IS INVALID *********\n");
	return UPM_ERROR_UNSPECIFIED;
}

upm_result_t lorawan_otaa_join(loRaWAN_context lorawan_dev) {
	printf("entering otaa join function\n");
    uint32_t mic = 0;
    msgType = JOIN_TX;
    // since this is join
    lorawan_dev->loRaWANOTAAJoin->lorawan_otaa_join_pack[0] = 0;
    //lorawan_otaa_join_pack[0] = 0;
    printf("aaaa\n");
    memcpy(lorawan_dev->loRaWANOTAAJoin->lorawan_otaa_join_pack+1, lorawan_dev->loRaWANSecurityParams->APPEUI, 8);
    printf("bbbb\n");
    memcpy(lorawan_dev->loRaWANOTAAJoin->lorawan_otaa_join_pack+9, lorawan_dev->loRaWANSecurityParams->DEVEUI, 8);
    //memcpy(lorawan_otaa_join_pack+1, APPEUI, 8);
    //memcpy(lorawan_otaa_join_pack+9, DEVEUI, 8);

    if(lorawan_dev->sx1276_dev == NULL)
    	printf("SX1276 DEV IS NULL\n");
    uint16_t nonce = SX1276Random(lorawan_dev->sx1276_dev);
    printf("cccc\n");
    lorawan_dev->loRaWANOTAAJoin->devNonce = nonce;
    //devNonce = nonce;
    lorawan_dev->loRaWANOTAAJoin->lorawan_otaa_join_pack[17] = nonce & 0xff;
    lorawan_dev->loRaWANOTAAJoin->lorawan_otaa_join_pack[18] = (nonce >> 8) & 0xff;
    printf("dddd\n");
    //lorawan_otaa_join_pack[17] = nonce & 0xff;
    //lorawan_otaa_join_pack[18] = (nonce >> 8) & 0xff;

    LoRaMacJoinComputeMic(lorawan_dev->loRaWANOTAAJoin->lorawan_otaa_join_pack, 19, lorawan_dev->loRaWANSecurityParams->APPKEY, &mic);
    //LoRaMacJoinComputeMic(lorawan_otaa_join_pack, 19, APPKEY, &mic);
    printf("eeee\n");

    lorawan_dev->loRaWANOTAAJoin->lorawan_otaa_join_pack[19] = mic&0xff;
    lorawan_dev->loRaWANOTAAJoin->lorawan_otaa_join_pack[20] = (mic >> 8) & 0xff;
	lorawan_dev->loRaWANOTAAJoin->lorawan_otaa_join_pack[21] = (mic >> 16) & 0xff;
	lorawan_dev->loRaWANOTAAJoin->lorawan_otaa_join_pack[22] = (mic >> 24) & 0xff;

#if 1
    int c = 0;
    for(c=0; c<LORAWAN_OTAA_JOIN_PACK_LEN; c++)
        printf("%x, ", lorawan_dev->loRaWANOTAAJoin->lorawan_otaa_join_pack[c]);
    printf("\n");
#endif

    // setting random channel
    while(1) {
		lorawan_dev->loRaWANBandParams->currentChannel = setNextChannel(lorawan_dev);
    	//currentChannel = setNextChannel();
		lorawan_dev->loRaWANBandParams->nbJoinTries++;
		printf("\n**************current Channel: %d**************\n", lorawan_dev->loRaWANBandParams->currentChannel);
		printf("Trying to join on frequency : %d\n", lorawan_dev->loRaWANBandParams->Channels[lorawan_dev->loRaWANBandParams->currentChannel]->Frequency);
		setFrequency(lorawan_dev->sx1276_dev, lorawan_dev->loRaWANBandParams->Channels[lorawan_dev->loRaWANBandParams->currentChannel]->Frequency);

		if(lorawan_dev->loRaWANBandParams->currentChannel >= 64 && lorawan_dev->loRaWANBandParams->currentChannel <= 71) {
		//if(currentChannel >= 64 && currentChannel <=71) {
			lorawan_dev->loRaWANParams->uplink_datarate = DR_4;
		} else {
			lorawan_dev->loRaWANParams->uplink_datarate = DR_0;
		}

		SX1276SetTxConfig(lorawan_dev->sx1276_dev, MODEM_LORA, 20, 0, dr_table[lorawan_dev->loRaWANParams->uplink_datarate][1], dr_table[lorawan_dev->loRaWANParams->uplink_datarate][0], 1, 8, false, true, 0, 0, false, 3000);
		SX1276SetPublicNetwork(lorawan_dev->sx1276_dev, true);
		SX1276Send(lorawan_dev->sx1276_dev, lorawan_dev->loRaWANOTAAJoin->lorawan_otaa_join_pack, LORAWAN_OTAA_JOIN_PACK_LEN);
		//SX1276SetTxConfig(MODEM_LORA, 20, 0, dr_table[uplink_datarate][1], dr_table[uplink_datarate][0], 1, 8, false, true, 0, 0, false, 3000);
		//SX1276SetPublicNetwork(true);
		//SX1276Send(lorawan_otaa_join_pack, LORAWAN_OTAA_JOIN_PACK_LEN);
		// following not yet implemented
#if 0
		tx_st = SX1276Send(pack, 23);
		printf("tx_st value: %d\n", tx_st);
		if(tx_st == TX_TIMEOUT) {
			// call the timeout function
		} else if(tx_st == TX_DONE) {
			// call the tx done function
		}
#endif

		//currentChannel = tryChannel;
		msgType = JOIN_RX;
		if (lorawan_initiate_rx(lorawan_dev, JOIN_RX) == LORAMAC_RECEIVE_SUCCESS) {
			printf("************************************\n");
			printf("Successfuly Joined on: %d\n", lorawan_dev->loRaWANBandParams->Channels[lorawan_dev->loRaWANBandParams->currentChannel]->Frequency);
			printf("************************************\n");
			lorawan_dev->loRaWANBandParams->nbJoinTries = 0;
			lorawan_dev->loRaWANBandParams->channelCounter = 0;
#if MULTITECH
			int sub_band_available = 0;
			sub_band_available = lorawan_dev->loRaWANBandParams->currentChannel/16;
			for (uint8_t i = 0; i < 6; i++)
				if(i == sub_band_available)
					continue;
				else
					lorawan_dev->loRaWANBandParams->channelMask[i] = 0x0000;
			lorawan_dev->loRaWANBandParams->channelMask[4] = (1<<(sub_band_available*2)) | (1<<(sub_band_available*2 + 1));
#endif
			break;
		}
    }

    // unrandomizing array
    for(uint8_t i = 0; i < 72; i++) {
    	lorawan_dev->loRaWANBandParams->randChannelArr[i] = i;
    }
    cleanUpChannels(lorawan_dev);
    printf("Randoimizing the array after join\n");
    for(uint8_t i = 0; i < 72; i++) {
    	printf("%d, ", lorawan_dev->loRaWANBandParams->randChannelArr[i]);
    }
    printf("\n");

    return UPM_SUCCESS;
}

void onRxDone(loRaWAN_context lorawan_dev, uint8_t *payload, uint16_t size) {
    printf("coming into: OnRadioRxDone\n");
    //MHDR_t macHDR;
    uint32_t mic = 0;
    uint32_t micRx;
    uint8_t rx_dec_buffer[256];
    uint8_t nonce[2] = {0};
    //FHDRParams_t FHDRParams;
    uint16_t sequenceCounter;
    uint8_t multicast = 0; // 1 means its a multicast
    int foptsLenCnt = 0;
    //int rxPacketLen = 0;
    printf("payload: \n");
    for(uint8_t i = 0; i < size; i++)
    	printf("%x, ", payload[i]);
    printf("\n");
    lorawan_dev->MHDR->type = (payload[0] >> 5)&0x07;
    lorawan_dev->MHDR->Major = payload[0]&0x03;
    //macHDR.type = (payload[0] >> 5)&0x07;
    //macHDR.Major = payload[0]&0x03;
#if 1
    switch(lorawan_dev->MHDR->type){
        case FRAME_TYPE_JOIN_ACCEPT:
            LoRaMacJoinDecrypt(payload+1, size-1, lorawan_dev->loRaWANSecurityParams->APPKEY, rx_dec_buffer+1);
            rx_dec_buffer[0] = payload[0];

            nonce[0] = lorawan_dev->loRaWANOTAAJoin->devNonce&0xff;
            nonce[1] = (lorawan_dev->loRaWANOTAAJoin->devNonce>>8)&0xff;

            LoRaMacJoinComputeMic(rx_dec_buffer, size - LORAMAC_MFR_LEN, lorawan_dev->loRaWANSecurityParams->APPKEY, &mic);
            int c = 0;
            for(c = 0; c<size; c++)
                printf("%x ,", rx_dec_buffer[c]);
            printf("\n");
            micRx = (rx_dec_buffer[size-1]<<24) |
            		(rx_dec_buffer[size-2]<<16) |
					(rx_dec_buffer[size-3]<<8) |
					(rx_dec_buffer[size-4]);
            if(mic == micRx) {
            	printf("the mics are the same\n");
            	LoRaMacJoinComputeSKeys(lorawan_dev->loRaWANSecurityParams->APPKEY, rx_dec_buffer+1, lorawan_dev->loRaWANOTAAJoin->devNonce, lorawan_dev->loRaWANSecurityParams->NwkSKey, lorawan_dev->loRaWANSecurityParams->AppSKey);
            	c = 0;
            	printf("LoRaMacNwkSKey: ");
            	for(c = 0; c<16; c++)
            		printf("%x, ", lorawan_dev->loRaWANSecurityParams->NwkSKey[c]);
            	printf("\n");
            	c = 0;
            	printf("LoRaMacAppSKey: ");
            	for(c = 0; c<16; c++)
            	    printf("%x, ", lorawan_dev->loRaWANSecurityParams->AppSKey[c]);
            	printf("\n");
            }

            lorawan_dev->loRaWANParams->LoRaMacNetID = ( uint32_t )rx_dec_buffer[4];
            lorawan_dev->loRaWANParams->LoRaMacNetID |= ( ( uint32_t )rx_dec_buffer[5] << 8 );
            lorawan_dev->loRaWANParams->LoRaMacNetID |= ( ( uint32_t )rx_dec_buffer[6] << 16 );

            lorawan_dev->loRaWANParams->LoRaMacDevAddr = ( uint32_t )rx_dec_buffer[7];
            lorawan_dev->loRaWANParams->LoRaMacDevAddr |= ( ( uint32_t )rx_dec_buffer[8] << 8 );
            lorawan_dev->loRaWANParams->LoRaMacDevAddr |= ( ( uint32_t )rx_dec_buffer[9] << 16 );
            lorawan_dev->loRaWANParams->LoRaMacDevAddr |= ( ( uint32_t )rx_dec_buffer[10] << 24 );
            printf("LoRa n/w ID: %.6x and Dev Addr: %.8x\n", lorawan_dev->loRaWANParams->LoRaMacNetID, lorawan_dev->loRaWANParams->LoRaMacDevAddr);

            // DLSettings
            lorawan_dev->loRaWANParams->Rx1DrOffset = ( rx_dec_buffer[11] >> 4 ) & 0x07;
            lorawan_dev->loRaWANParams->downlink_datarate_2 = rx_dec_buffer[11] & 0x0F;

            // RxDelay
            lorawan_dev->loRaWANParams->ReceiveDelay1 = ( rx_dec_buffer[12] & 0x0F );
            printf("initial ReceiveDelay1: %d\n", lorawan_dev->loRaWANParams->ReceiveDelay1);
            if(lorawan_dev->loRaWANParams->ReceiveDelay1 == 0)
            {
            	lorawan_dev->loRaWANParams->ReceiveDelay1 = 1;
            }
            lorawan_dev->loRaWANParams->ReceiveDelay1 *= 1000;
            lorawan_dev->loRaWANParams->ReceiveDelay2 = lorawan_dev->loRaWANParams->ReceiveDelay1 + 1000;
            //printf("Rx1DrOffset: %x, rx2_datarate: %x, ReceiveDelay1: %d, ReceiveDelay2: %d\n", Rx1DrOffset, downlink_datarate_2, ReceiveDelay1, ReceiveDelay2);
            lorawan_dev->loRaWANParams->FCnt_up = 0;
            lorawan_dev->loRaWANParams->FCnt_down = 0;
            joinSuccessful = true;
        break;
        case FRAME_TYPE_DATA_CONFIRMED_DOWN:
        case FRAME_TYPE_DATA_UNCONFIRMED_DOWN:
        	lorawan_dev->FHDRParams->devAddr = payload[1] |
			          (payload[2] << 8) |
					  (payload[3] << 16) |
					  (payload[4] << 24);
            if(lorawan_dev->FHDRParams->devAddr != lorawan_dev->loRaWANParams->LoRaMacDevAddr) {
                // this is a multicast
            	multicast = 1;
            }

            lorawan_dev->FHDRParams->FCtrl->fctrl = payload[5];
            //lorawan_dev->FHDRParams->FCtrl->fctrl
            //FHDRParams.FCtrl.fctrl = payload[5];
            sequenceCounter = (payload[7] << 8) | payload[6];
            int macCmdStart = 8;

            micRx = (payload[size-1]<<24) |
                    (payload[size-2]<<16) |
            		(payload[size-3]<<8) |
            		(payload[size-4]);

#if LORAMAC_DEBUG
            printf("Device address from GW: %.8x\n", lorawan_dev->FHDRParams->devAddr);
            //lorawan_dev->FHDRParams->FCtrl->fCtrlFields_t.ACK;
            printf("F Ctrl Byte ACK Bit: %d\n", lorawan_dev->FHDRParams->FCtrl->fCtrlFields_t.ACK);
            printf("F Ctrl Byte ADR Bit: %d\n", lorawan_dev->FHDRParams->FCtrl->fCtrlFields_t.ADR);
            printf("F Ctrl Byte ADRACKReq bit: %d\n", lorawan_dev->FHDRParams->FCtrl->fCtrlFields_t.ADRACKReq);
            printf("F Ctrl Byte FPending bit: %d\n", lorawan_dev->FHDRParams->FCtrl->fCtrlFields_t.FPending);
            printf("F Control Byte Opts Len: %d\n", lorawan_dev->FHDRParams->FCtrl->fCtrlFields_t.FOptsLen);
            printf("Sequence Counter: %d\n", sequenceCounter);
            printf("Received Mic: %x\n", micRx);
#endif
            // take a look at sequence counter, the difference should not be too much
            if((sequenceCounter - lorawan_dev->loRaWANParams->FCnt_down) < 1<<15) {
            	lorawan_dev->loRaWANParams->FCnt_down += (sequenceCounter - lorawan_dev->loRaWANParams->FCnt_down);
            	LoRaMacComputeMic(payload, size - 4, lorawan_dev->loRaWANSecurityParams->NwkSKey, lorawan_dev->FHDRParams->devAddr, DOWN_LINK, lorawan_dev->loRaWANParams->FCnt_down, &mic);
            } else {
            	// sort issues out with the downlink counter
            }

            if(mic == micRx) {
                printf("the mics are equal\n");
                if(lorawan_dev->FHDRParams->FCtrl->fCtrlFields_t.FOptsLen != 0) {
                //if(FHDRParams.FCtrl.fCtrlFields_t.FOptsLen != 0) {
                	printf("Process MAC Commands\n");
                    for(foptsLenCnt = 0; foptsLenCnt < lorawan_dev->FHDRParams->FCtrl->fCtrlFields_t.FOptsLen; foptsLenCnt++) {
                    	lorawan_dev->loRaWANMACCommand->macCommandFoptsBuf_rx[foptsLenCnt] = payload[macCmdStart + foptsLenCnt];
                    	//macCommandFoptsBuf_rx[foptsLenCnt] = payload[macCmdStart + foptsLenCnt];
                    }
                    printf("Processing MAC Commands\n");
                    parseGWMacCommands(lorawan_dev, foptsLenCnt);
                } else {
                	printf("No MAC commands present\n");
                }
            }
        break;
    }
#endif
}

LoRaWanResult_t lorawan_transmit_packet(loRaWAN_context lorawan_dev, uint8_t* buf, uint16_t len) {
	LoRaWanResult_t ret;
	printf("lorawan_transmit_packet: preparing data packet\n");
	//RadioStateExt_t rx_st;
	uint16_t FRM_packet_len = len;
	msgType = TX;
    uint8_t fport = 1;
    FCtrl_t ctrl_byte;

    uint32_t mic;
    int packet_len = 0;
    // adding the MHDR
    lorawan_dev->lorawan_tx_packet[packet_len++] = 0x80;

    // adding the FHDR
	lorawan_dev->lorawan_tx_packet[packet_len++] = lorawan_dev->loRaWANParams->LoRaMacDevAddr & 0xff;
    lorawan_dev->lorawan_tx_packet[packet_len++] = (lorawan_dev->loRaWANParams->LoRaMacDevAddr >> 8) & 0xff;
    lorawan_dev->lorawan_tx_packet[packet_len++] = (lorawan_dev->loRaWANParams->LoRaMacDevAddr >> 16) & 0xff;
    lorawan_dev->lorawan_tx_packet[packet_len++] = (lorawan_dev->loRaWANParams->LoRaMacDevAddr >> 24) & 0xff;

    // FCtrl
    ctrl_byte.fCtrlFields_t.ADR = 1;
    ctrl_byte.fCtrlFields_t.ADRACKReq = 1;
    ctrl_byte.fCtrlFields_t.FOptsLen = 1;
    lorawan_dev->lorawan_tx_packet[packet_len++] = ctrl_byte.fctrl;

    // FCntr
    lorawan_dev->lorawan_tx_packet[packet_len++] = lorawan_dev->loRaWANParams->FCnt_up & 0xff;
    lorawan_dev->lorawan_tx_packet[packet_len++] = (lorawan_dev->loRaWANParams->FCnt_up >> 8) & 0xff;

    // Adding a MAC command - Test
    lorawan_dev->lorawan_tx_packet[packet_len++] = 0x02;

    // No FOpts
#if LORAMAC_DEBUG
    int c = 0;
    printf("nw key: ");
    for(c= 0;c<16;c++)
    	printf("%x, ", lorawan_dev->loRaWANSecurityParams->NwkSKey[c]);
    printf("\n");
    printf("app key: ");
    for(c= 0;c<16;c++)
        printf("%x, ", lorawan_dev->loRaWANSecurityParams->AppSKey[c]);
    printf("\n");
#endif
    // adding the frame port
    lorawan_dev->lorawan_tx_packet[packet_len++] = fport;

#if 1
    // encrypting the payload
    if(fport == 0) {
    	printf("encrypting using nwk key\n");
    	LoRaMacPayloadEncrypt((uint8_t*) buf, FRM_packet_len, lorawan_dev->loRaWANSecurityParams->NwkSKey, lorawan_dev->loRaWANParams->LoRaMacDevAddr, UP_LINK, lorawan_dev->loRaWANParams->FCnt_up, lorawan_dev->loRaWANPayloadParams->tx_frm_payload);
        //LoRaMacPayloadEncrypt((uint8_t*) buf, FRM_packet_len, NwkSKey, LoRaMacDevAddr, UP_LINK, FCnt_up, tx_frm_payload);
    } else {
    	printf("encrypting using app key\n");
    	LoRaMacPayloadEncrypt((uint8_t*) buf, FRM_packet_len, lorawan_dev->loRaWANSecurityParams->AppSKey, lorawan_dev->loRaWANParams->LoRaMacDevAddr, UP_LINK, lorawan_dev->loRaWANParams->FCnt_up, lorawan_dev->loRaWANPayloadParams->tx_frm_payload);
    	//LoRaMacPayloadEncrypt((uint8_t*) buf, FRM_packet_len, AppSKey, LoRaMacDevAddr, UP_LINK, FCnt_up, tx_frm_payload);

#if LORAMAC_DEBUG
    uint8_t some_buf[25] = {0};
    LoRaMacPayloadDecrypt(lorawan_dev->loRaWANPayloadParams->tx_frm_payload, FRM_packet_len, lorawan_dev->loRaWANSecurityParams->AppSKey, lorawan_dev->loRaWANParams->LoRaMacDevAddr, UP_LINK, lorawan_dev->loRaWANParams->FCnt_up, some_buf);
    //LoRaMacPayloadDecrypt(tx_frm_payload, FRM_packet_len, AppSKey, LoRaMacDevAddr, UP_LINK, FCnt_up, some_buf);
    printf("**************Decrpyting Done**************\n");
    int lol = 0;
    for (lol=0;lol<25;lol++)
        printf("%x, ", some_buf[lol]);
    printf("\n");
#endif
    }

    memcpy(lorawan_dev->lorawan_tx_packet+packet_len, lorawan_dev->loRaWANPayloadParams->tx_frm_payload, FRM_packet_len);
    //memcpy(lorawan_tx_packet+packet_len, tx_frm_payload, FRM_packet_len);
#else
    memcpy(lorawan_dev->lorawan_tx_packet+packet_len, buf, FRM_packet_len);
    //memcpy(lorawan_tx_packet+packet_len, buf, FRM_packet_len);
#endif
    packet_len += FRM_packet_len;

    LoRaMacComputeMic(lorawan_dev->lorawan_tx_packet, packet_len, lorawan_dev->loRaWANSecurityParams->NwkSKey, lorawan_dev->loRaWANParams->LoRaMacDevAddr, UP_LINK, lorawan_dev->loRaWANParams->FCnt_up, &mic);
    //LoRaMacComputeMic(lorawan_tx_packet, packet_len, NwkSKey, LoRaMacDevAddr, UP_LINK, FCnt_up, &mic);

    lorawan_dev->lorawan_tx_packet[packet_len++] = mic & 0xff;
    lorawan_dev->lorawan_tx_packet[packet_len++] = (mic >> 8) & 0xff;
    lorawan_dev->lorawan_tx_packet[packet_len++] = (mic >> 16) & 0xff;
    lorawan_dev->lorawan_tx_packet[packet_len++] = (mic >> 24) & 0xff;

#if LORAMAC_DEBUG
    printf("total packet length: %d\n", packet_len);
    for(c=0;c<packet_len;c++) {
        printf("%x, ", lorawan_dev->lorawan_tx_packet[c]);
    }
    printf("\n");
#endif

    lorawan_dev->loRaWANParams->FCnt_up++;
    ret = lorawan_schedule_tx(lorawan_dev, packet_len);
    msgType = RX_1;
#if 0
    // receive window 1 starts here
    upm_delay_us(850000);
    setFrequency(9239e5);

    SX1276SetRxConfig(MODEM_LORA, 2, 10,
                      1, 0, 8,
                      8, false,
                      0, false, 0, 0, true, true);

    rx_st = SX1276SetRx(3000);
#endif
    ret = lorawan_initiate_rx(lorawan_dev, msgType);

    return ret;
}

LoRaWanResult_t lorawan_initiate_rx(loRaWAN_context lorawan_dev, LoRaWanMsg_t type) {
	LoRaWanResult_t ret;
	RadioStateExt_t rx_st = 0;
	int rx_buf_len;

    printf("*********************************************************\n");
    printf("************************ Enter RX ***********************\n");
    printf("*********************************************************\n");

    switch(type) {
    case JOIN_RX:
        // this needs to be extremely accurate
        upm_delay_us(4900000);
        // opening up the first rx window, if the downlink packet is received in this window then
        // we wont go to the second rx window. The wait should be 5 seconds between tx and rx1 window 1
        // but I'm paranoid and therefore will open it up 4 seconds after and will keep it open for 8 seconds
        // pointlessly.
        // again hard-coded, will have to change, this and the above tx freq form a pair of sorts
        printf("********************************************************\n");
        printf("****************RX Frequency: %d****************\n", rxFrequencies[lorawan_dev->loRaWANBandParams->currentChannel%8]);
        printf("********************************************************\n");
        setFrequency(lorawan_dev->sx1276_dev, rxFrequencies[lorawan_dev->loRaWANBandParams->currentChannel%8]);

        // there can be way too much variation in terms of the time it takes for a packet to travel
        // from the GW to the Edge Node, therefore one solution would be to keep the window open a
        // little longer than whats proposed by the semtech driver. What we're trying to do here is essentially
        // not miss out on a response from the GW.
        SX1276SetRxConfig(lorawan_dev->sx1276_dev, MODEM_LORA, dr_table[data_rate_offsets[lorawan_dev->loRaWANParams->uplink_datarate][0]][1], dr_table[data_rate_offsets[lorawan_dev->loRaWANParams->uplink_datarate][0]][0],
                          1, 0, 8,
                          80, false,
                          0, false, 0, 0, true, false);

        rx_st = SX1276SetRx(lorawan_dev->sx1276_dev, 3000);
        //uint8_t rx_dec_buffer[256];

        printf("rx_st value: %d\n", rx_st);
    break;
    case RX_1:
    	// receive window 1 starts here
    	// the delay here needs some fine tuning for now
    	upm_delay_us(950000);

    	setFrequency(lorawan_dev->sx1276_dev, rxFrequencies[lorawan_dev->loRaWANBandParams->currentChannel%8]);
    	printf("Data rate: %d\n", dr_table[data_rate_offsets[lorawan_dev->loRaWANParams->uplink_datarate][0]][0]);
    	printf("bandwidth set; %d\n", dr_table[data_rate_offsets[lorawan_dev->loRaWANParams->uplink_datarate][0]][1]);

    	SX1276SetRxConfig(lorawan_dev->sx1276_dev, MODEM_LORA, dr_table[data_rate_offsets[lorawan_dev->loRaWANParams->uplink_datarate][0]][1], dr_table[data_rate_offsets[lorawan_dev->loRaWANParams->uplink_datarate][0]][0],
    	                  1, 0, 8,
    	                  180, false,
    	                  0, false, 0, 0, true, false);

    	rx_st = SX1276SetRx(lorawan_dev->sx1276_dev, 3000);
    break;
    default:
    	return LORAMAC_GENERAL_FAILURE;
    }
    switch (rx_st) {
        case RX_TIMEOUT:
            printf("LORAMAC:RX TIMEOUT\n");
            ret = LORAMAC_RX_TIMEOUT;
            joinSuccessful = false;
        break;
        case RX_DONE:
            rx_buf_len = SX1276GetRxBufferlen(lorawan_dev->sx1276_dev);
            SX1276GetRxBuffer(lorawan_dev->sx1276_dev, lorawan_dev->rx_buffer);
            onRxDone(lorawan_dev, lorawan_dev->rx_buffer, rx_buf_len);
            ret = LORAMAC_RECEIVE_SUCCESS;
            printf("LORAMAC:RX DONE\n");
        break;
        case RX_ERROR:
        default:
            printf("LORAMAC:RX ERROR\n");
            ret = LORAMAC_RECEIVE_FAILURE;
            joinSuccessful = false;
        break;
    }

    return ret;
}

LoRaWanResult_t lorawan_schedule_tx(loRaWAN_context lorawan_dev, int size) {
	// if dr is preset to 500
	if(lorawan_dev->loRaWANParams->req_datarate == DR_4) {
		while(1) {
			lorawan_dev->loRaWANBandParams->currentChannel = setNextChannel(lorawan_dev);
			if(lorawan_dev->loRaWANBandParams->currentChannel >= 64 && lorawan_dev->loRaWANBandParams->currentChannel <= 71) {
				lorawan_dev->loRaWANParams->uplink_datarate = DR_4;
				break;
			}
		}
    // datarate preset to 125
	} else {
		lorawan_dev->loRaWANBandParams->currentChannel = setNextChannel(lorawan_dev);
		// but could still go up to 500
		if(lorawan_dev->loRaWANBandParams->currentChannel >= 64 && lorawan_dev->loRaWANBandParams->currentChannel <= 71)
			lorawan_dev->loRaWANParams->uplink_datarate = DR_4;
		else
			lorawan_dev->loRaWANParams->uplink_datarate = DR_0;
	}
	setFrequency(lorawan_dev->sx1276_dev, lorawan_dev->loRaWANBandParams->Channels[lorawan_dev->loRaWANBandParams->currentChannel]->Frequency);

	printf("Uplink DR: %d, Uplink Power: %d, Uplink BW: %d\n", dr_table[lorawan_dev->loRaWANParams->uplink_datarate][0], lorawan_dev->loRaWANParams->uplink_power, dr_table[lorawan_dev->loRaWANParams->uplink_datarate][1]);
    SX1276SetTxConfig(lorawan_dev->sx1276_dev, MODEM_LORA, lorawan_dev->loRaWANParams->uplink_power, 0, dr_table[lorawan_dev->loRaWANParams->uplink_datarate][1], dr_table[lorawan_dev->loRaWANParams->uplink_datarate][0], 1, 8, false, true, 0, 0, false, 3000);
    SX1276SetPublicNetwork(lorawan_dev->sx1276_dev, true);
    SX1276Send(lorawan_dev->sx1276_dev, lorawan_dev->lorawan_tx_packet, size);
    // check for error and return the appropriate error
    printf("coming back from send\n");
    return LORAMAC_TRANSMIT_SUCCESS;
}

void lorawan_set_dev_eui(loRaWAN_context lorawan_dev, uint8_t* dev_eui) {
    int c = 0;
    for(c=0; c<8; c++)
    	lorawan_dev->loRaWANSecurityParams->DEVEUI[c] = dev_eui[c];
}

void lorawan_set_app_eui(loRaWAN_context lorawan_dev, uint8_t* app_eui) {
    int c = 0;
    for(c=0; c<8; c++)
    	lorawan_dev->loRaWANSecurityParams->APPEUI[c] = app_eui[c];
}

void lorawan_set_app_key(loRaWAN_context lorawan_dev, uint8_t* app_key) {
    int c = 0;
    for(c=0; c<16; c++)
    	lorawan_dev->loRaWANSecurityParams->APPKEY[c] = app_key[c];
}

void addEDMacCommand(loRaWAN_context lorawan_dev, loRaMacEDCommands_t cmd, uint8_t len, uint8_t* resp) {

	if(lorawan_dev->loRaWANMACCommand->macCommandLen_tx < 16) {
		switch(cmd) {
		case LINK_CHECK_REQ:
			lorawan_dev->loRaWANMACCommand->macCommandFoptsBuf_tx[lorawan_dev->loRaWANMACCommand->macCommandLen_tx++] = LINK_CHECK_REQ;
		break;
		case LINK_ADR_ANS:
			lorawan_dev->loRaWANMACCommand->macCommandFoptsBuf_tx[lorawan_dev->loRaWANMACCommand->macCommandLen_tx++] = LINK_ADR_ANS;
			lorawan_dev->loRaWANMACCommand->macCommandFoptsBuf_tx[lorawan_dev->loRaWANMACCommand->macCommandLen_tx++] = resp[0];
			printf("Response to the ADR command: %x\n", resp[0]);
		break;
		case DUTY_CYCLE_ANS:
			lorawan_dev->loRaWANMACCommand->macCommandFoptsBuf_tx[lorawan_dev->loRaWANMACCommand->macCommandLen_tx++] = DUTY_CYCLE_ANS;
		break;
		case RX_PARAM_SETUP_ANS:
			lorawan_dev->loRaWANMACCommand->macCommandFoptsBuf_tx[lorawan_dev->loRaWANMACCommand->macCommandLen_tx++] = RX_PARAM_SETUP_ANS;
			lorawan_dev->loRaWANMACCommand->macCommandFoptsBuf_tx[lorawan_dev->loRaWANMACCommand->macCommandLen_tx++] = resp[0];
		break;
		case DEV_STATUS_ANS:
			lorawan_dev->loRaWANMACCommand->macCommandFoptsBuf_tx[lorawan_dev->loRaWANMACCommand->macCommandLen_tx++] = DEV_STATUS_ANS;
			int cnt = 0;
			while(cnt < len) {
				lorawan_dev->loRaWANMACCommand->macCommandFoptsBuf_tx[lorawan_dev->loRaWANMACCommand->macCommandLen_tx++] = resp[cnt];
				cnt++;
			}
        break;
		case NEW_CHANNEL_ANS:
			lorawan_dev->loRaWANMACCommand->macCommandFoptsBuf_tx[lorawan_dev->loRaWANMACCommand->macCommandLen_tx++] = NEW_CHANNEL_ANS;
			lorawan_dev->loRaWANMACCommand->macCommandFoptsBuf_tx[lorawan_dev->loRaWANMACCommand->macCommandLen_tx++] = resp[0];
		break;
		case RX_TIMING_SETUP_ANS:
			lorawan_dev->loRaWANMACCommand->macCommandFoptsBuf_tx[lorawan_dev->loRaWANMACCommand->macCommandLen_tx++] = RX_TIMING_SETUP_ANS;
		break;
		default:
			printf("No MAC Commands to compose\n");
		}
	} else {
		printf("can't add more MAC Commands\n");
	}
}

/*
void addGWMacResponse(loRaMacGWCommands_t cmd, uint8_t len, uint8_t* resp) {
    int counter = 0;
    while(counter < 0) {
    	switch(cmd) {
    	case LINK_ADR_ANS:

    	}
    }
}
*/

void parseGWMacCommands(loRaWAN_context lorawan_dev, int size) {
    int counter = 0;
    //int counter_resp = 0;
    loRaMacGWCommands_t cmd;
    while(counter <size) {
    	// figure out the first command
    	cmd = lorawan_dev->loRaWANMACCommand->macCommandFoptsBuf_rx[counter++];
    	switch(cmd) {
    	    case LINK_CHECK_ANS: {
        		uint8_t pl_lca[2];
        		pl_lca[0] = lorawan_dev->loRaWANMACCommand->macCommandFoptsBuf_rx[counter++];
        		pl_lca[1] = lorawan_dev->loRaWANMACCommand->macCommandFoptsBuf_rx[counter++];
                printf("MAC Command: LINK_CHECK_ANS\n");
                printf("Margin: %x and GW Cnt: %x\n", pl_lca[0], pl_lca[1]);
    	    }
            break;
    	    case LINK_ADR_REQ: {
    	    	uint8_t pl_adr[4];
    	    	pl_adr[0] = lorawan_dev->loRaWANMACCommand->macCommandFoptsBuf_rx[counter++];
    	    	pl_adr[1] = lorawan_dev->loRaWANMACCommand->macCommandFoptsBuf_rx[counter++];
    	    	pl_adr[2] = lorawan_dev->loRaWANMACCommand->macCommandFoptsBuf_rx[counter++];
    	    	pl_adr[3] = lorawan_dev->loRaWANMACCommand->macCommandFoptsBuf_rx[counter++];
    	    	printf("MAC Command: LINK_ADR_REQ\n");
    	    	printf("Data TX Power Proposed: %x\n", pl_adr[0]&0x0f);
    	    	printf("DataRate Proposed: %x\n", ((pl_adr[0]&0xf0)>>4));
    	    	printf("Channel Mask: %x\n", ((pl_adr[1]<<8)|pl_adr[2]));
    	    	printf("Redundancy: %x\n", pl_adr[3]);
    	    	lorawan_dev->loRaWANParams->req_datarate = ((pl_adr[0]&0xf0)>>4);
    	    	// ChMaskCntl number
                uint8_t ChMaskCntl = (pl_adr[3]>>4)&0x07;
                lorawan_dev->loRaWANBandParams->channelMask[ChMaskCntl] = pl_adr[1]|(pl_adr[2]<<8);
                //channeMask[ChMaskCntl] = pl_adr[1]|(pl_adr[2]<<8);
#if MULTITECH
                // this is weird, but the multitech GW works only on 8 channels within
                // a particular frequency sub-band at a time, and also 1 500 KHz channel
                // within that same frequency sub band
                int c = 0;
                for (c = 0; c < 6; c++)
                    if(c != ChMaskCntl)
                    	lorawan_dev->loRaWANBandParams->channelMask[c] = 0x0000;

                if((lorawan_dev->loRaWANBandParams->channelMask[ChMaskCntl]& 0x00ff) != 0)
                	lorawan_dev->loRaWANBandParams->channelMask[4] = 1<<(ChMaskCntl*2);
                else if(((lorawan_dev->loRaWANBandParams->channelMask[ChMaskCntl]& 0xff00)>>8) != 0)
                	lorawan_dev->loRaWANBandParams->channelMask[4] = 1<<(ChMaskCntl*2 + 1);
                else if((lorawan_dev->loRaWANBandParams->channelMask[ChMaskCntl]& 0x00ff) != 0 && ((lorawan_dev->loRaWANBandParams->channelMask[ChMaskCntl]& 0xff00)>>8) != 0)
                	lorawan_dev->loRaWANBandParams->channelMask[4] = (1<<(ChMaskCntl*2)) | (1<<(ChMaskCntl*2 + 1));
#endif
                // setting up BW 500 channels

                //channeMask[4] = (1<<(ChMaskCntl*2)) | (1<<(ChMaskCntl*2 + 1));
                // this should be common for all implementations, hopefully
                for(uint8_t i = 0; i < 72; i++) {
                    lorawan_dev->loRaWANBandParams->randChannelArr[i] = i;
                }
                cleanUpChannels(lorawan_dev);
                printf("channels after adr resp: \n");
                for(uint8_t i = 0; i<72; i++) {
                	printf("%d, ", lorawan_dev->loRaWANBandParams->randChannelArr[i]);
                }
                printf("\n");
    	    	// values 11 and above not supported
    	    	if((pl_adr[0]&0x0f) < 11)
    	    	    lorawan_dev->loRaWANParams->uplink_power = BASE_TX_POWER - 2*(pl_adr[0]&0x0f);
    	    	// response will be one byte long
    	    	uint8_t resp = 0x07;
    	    	addEDMacCommand(lorawan_dev, LINK_ADR_ANS, 1, &resp);
    	    }
    	    break;
    	    case DUTY_CYCLE_REQ: {
                uint8_t pl_dcr;
                pl_dcr = lorawan_dev->loRaWANMACCommand->macCommandFoptsBuf_rx[counter++];
                printf("MAC Command: DUTY_CYCLE_REQ\n");
                printf("Duty Cycle Requested: %x\n", pl_dcr);
                // response has no payload
                addEDMacCommand(lorawan_dev, DUTY_CYCLE_ANS, 0, NULL);
    	    }
            break;
    	    case RX_PARAM_SETUP_REQ: {
    	    	uint8_t pl_psr[4];
    	    	pl_psr[0] = lorawan_dev->loRaWANMACCommand->macCommandFoptsBuf_rx[counter++];
    	    	pl_psr[1] = lorawan_dev->loRaWANMACCommand->macCommandFoptsBuf_rx[counter++];
    	    	pl_psr[2] = lorawan_dev->loRaWANMACCommand->macCommandFoptsBuf_rx[counter++];
    	    	pl_psr[3] = lorawan_dev->loRaWANMACCommand->macCommandFoptsBuf_rx[counter++];
    	    	printf("MAC Command: RX_PARAM_SETUP_REQ\n");
    	    	printf("DL Settings: %x, Frequency: %x%x%x\n", pl_psr[0], pl_psr[1], pl_psr[2], pl_psr[3]);
    	    	uint8_t resp = 0x07;
    	    	addEDMacCommand(lorawan_dev, RX_PARAM_SETUP_ANS, 1, &resp);
    	    }
    	    break;
    	    case DEV_STATUS_REQ: {
    	        printf("MAC Command: DEV_STATUS_REQ\n");
    	        // response has 2 bytes in payload
    	        uint8_t resp[2];
    	        resp[0] = 0x00;
    	        resp[1] = 0x00;
    	        addEDMacCommand(lorawan_dev, DEV_STATUS_ANS, 2, resp);
    	    }
    	    break;
    	    case NEW_CHANNEL_REQ: {
    	    	uint8_t pl_ncr[5];
    	    	pl_ncr[0] = lorawan_dev->loRaWANMACCommand->macCommandFoptsBuf_rx[counter++];
    	    	pl_ncr[1] = lorawan_dev->loRaWANMACCommand->macCommandFoptsBuf_rx[counter++];
    	    	pl_ncr[2] = lorawan_dev->loRaWANMACCommand->macCommandFoptsBuf_rx[counter++];
    	    	pl_ncr[3] = lorawan_dev->loRaWANMACCommand->macCommandFoptsBuf_rx[counter++];
    	    	pl_ncr[4] = lorawan_dev->loRaWANMACCommand->macCommandFoptsBuf_rx[counter++];
    	    	printf("MAC Command: NEW_CHANNEL_REQ\n");
    	    	printf("Channel Index: %x, Frequency: %x%x%x, DrRange: %x\n", pl_ncr[0], pl_ncr[1], pl_ncr[2], pl_ncr[3], pl_ncr[4]);
    	    	// response has 1 byte
    	    	uint8_t resp = 0x03;
    	    	addEDMacCommand(lorawan_dev, NEW_CHANNEL_ANS, 1, &resp);
    	    }
    	    break;
    	    case RX_TIMING_SETUP_REQ: {
    	    	uint8_t pl_rtsr;
    	    	pl_rtsr = lorawan_dev->loRaWANMACCommand->macCommandFoptsBuf_rx[counter++];
    	    	printf("MAC Command: RX_TIMING_SETUP_REQ\n");
    	    	printf("Setting: %x\n", pl_rtsr);
    	    	// no payload
    	    	addEDMacCommand(lorawan_dev, RX_TIMING_SETUP_ANS, 0, NULL);
    	    }
    	    break;
    	    default:
    	    	printf("MAC Command not understood\n");
    	    	// no response
    	}
    }
}
