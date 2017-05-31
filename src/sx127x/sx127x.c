/*
 * Author: Abhishek Malik <abhishek.malik@intel.com>
 *
 * This code is heavily based on the semtech lora library at:
 * https://github.com/Lora-net/LoRaMac-node
 * which is licensed under the BSD license at:
 * upm/src/sx127x/LICENSE.txt
 */


#include <stdlib.h>
#include <unistd.h>

#include "sx127x.h"
#include "sx127x_fsk.h"

/*!
 * Constant values need to compute the RSSI value
 */
#define RSSI_OFFSET_LF                              -164
#define RSSI_OFFSET_HF                              -157

uint8_t rx_done = 0;

/*!
 * Radio registers definition
 */
typedef struct
{
    RadioModems_t Modem;
    uint8_t       Addr;
    uint8_t       Value;
}RadioRegisters_t;

/*!
 * Radio hardware registers initialization
 *
 * \remark RADIO_INIT_REGISTERS_VALUE is defined in sx127x.h file
 */
const RadioRegisters_t RadioRegsInit[] = RADIO_INIT_REGISTERS_VALUE;

/*!
 * Precomputed FSK bandwidth registers values
 */
const FskBandwidth_t FskBandwidths[] =
{
    { 2600  , 0x17 },
    { 3100  , 0x0F },
    { 3900  , 0x07 },
    { 5200  , 0x16 },
    { 6300  , 0x0E },
    { 7800  , 0x06 },
    { 10400 , 0x15 },
    { 12500 , 0x0D },
    { 15600 , 0x05 },
    { 20800 , 0x14 },
    { 25000 , 0x0C },
    { 31300 , 0x04 },
    { 41700 , 0x13 },
    { 50000 , 0x0B },
    { 62500 , 0x03 },
    { 83333 , 0x12 },
    { 100000, 0x0A },
    { 125000, 0x02 },
    { 166700, 0x11 },
    { 200000, 0x09 },
    { 250000, 0x01 },
    { 300000, 0x00 }, // Invalid Bandwidth
};

typedef struct RadioFskSettings_t* RadioFskSettings_ptr;
typedef struct RadioFskPacketHandler_t* RadioFskPacketHandler_ptr;
typedef struct RadioLoRaSettings_t* RadioLoRaSettings_ptr;
typedef struct RadioLoRaPacketHandler_t* RadioLoRaPacketHandler_ptr;
typedef struct RadioSettings_t* RadioSettings_ptr;

RadioStateExt_t st;

sx1276_context init(int ss, int reset, int dio0, long frequency)
{
  // setup pins
printf("coming into the init\n");
    int mraa_rv;
    if((mraa_rv = mraa_init()) != MRAA_SUCCESS) {
        printf("%s: mraa_init failed (%d).\n", __FUNCTION__, mraa_rv);
    }

    // Allocating memory for different structs used by the sensor context
    RadioFskSettings_ptr RadioFskSettings = (RadioFskSettings_ptr) malloc(sizeof(RadioFskSettings_t));
    if(RadioFskSettings == NULL) {
    	printf("Unable to allocate memory to RadioFskSettings_t\n");
    	return NULL;
    }

    RadioFskPacketHandler_ptr RadioFskPacketHandler = (RadioFskPacketHandler_ptr) malloc(sizeof(RadioFskPacketHandler_t));
    if(RadioFskPacketHandler == NULL) {
       printf("Unable to allocate memory to RadioFskPacketHandler_t\n");
       return NULL;
    }

    RadioLoRaSettings_ptr RadioLoRaSettings = (RadioLoRaSettings_ptr) malloc(sizeof(RadioLoRaSettings_t));
    if(RadioLoRaSettings == NULL) {
    	printf("Unable to allocate memory to RadioLoRaSettings_t\n");
    	return NULL;
    }

    RadioLoRaPacketHandler_ptr RadioLoRaPacketHandler = (RadioLoRaPacketHandler_ptr) malloc(sizeof(RadioLoRaPacketHandler_t));
    if(RadioLoRaPacketHandler == NULL) {
    	printf("Unable to allocate memory to RadioLoRaPacketHandler_t\n");
    	return NULL;
    }

    RadioSettings_ptr RadioSettings = (RadioSettings_ptr) malloc(sizeof(RadioSettings_t));
    if(RadioSettings == NULL) {
    	printf("Unable to allocate memory to RadioSettings_t\n");
    	return NULL;
    }

    // assigning all the previously allocated memory
    RadioSettings->Fsk = RadioFskSettings;
    RadioSettings->LoRa = RadioLoRaSettings;
    RadioSettings->FskPacketHandler = RadioFskPacketHandler;
    RadioSettings->LoRaPacketHandler = RadioLoRaPacketHandler;

    sx1276_context SX1276 = (sx1276_context) malloc(sizeof(struct _sx1276_context));
    if(SX1276 == NULL) {
    	printf("Unable to assign memory to _sx1276_context\n");
    	return NULL;
    }

    SX1276->Settings = RadioSettings;

    if(!(SX1276->spi = mraa_spi_init(0)))
        printf("Unable to Initialize Spi bus\n");

    if(mraa_spi_frequency(SX1276->spi, 4000000) != MRAA_SUCCESS) {
        printf("sx1276: Unable to set higher frequency\n");
    }

    SX1276->_frequency  = 0;
    SX1276->_packetIndex = 0;
    SX1276->_implicitHeaderMode = 0;

    SX1276->_ss = ss;
    SX1276->_reset = reset;
    SX1276->_dio0 = dio0;

    // Initialize the cs pin
    if(!(SX1276->gpio_ss = mraa_gpio_init(SX1276->_ss))) {
        printf("sx1276: unable to init SPI CS\n");
    }

    if(mraa_gpio_dir(SX1276->gpio_ss, MRAA_GPIO_OUT) != MRAA_SUCCESS) {
        printf("sx1276: unable to set direction on SPI CS\n");
    }

    // initialize the reset pin
    if(!(SX1276->gpio_reset = mraa_gpio_init(SX1276->_reset))) {
        printf("sx1276: unable to init chip reset\n");
    }

    if(mraa_gpio_dir(SX1276->gpio_reset, MRAA_GPIO_OUT) != MRAA_SUCCESS) {
        printf("sx1276: unable to set direction on reset pin\n");
    }

    // perform reset
    mraa_gpio_write(SX1276->gpio_reset, 1);
    upm_delay_us(1000);
    mraa_gpio_write(SX1276->gpio_reset, 0);
    upm_delay_us(1000);
    mraa_gpio_write(SX1276->gpio_reset, 1);
    upm_delay_us(15000);

    //dumpRegisters();
    SX1276RxChainCalibration(SX1276);

    // set SS high
    mraa_gpio_write(SX1276->gpio_ss, 1);

    // check version
    uint8_t version = readRegister(SX1276, REG_VERSION);
    if (version != 0x12) {
        printf("wrong version\n");
        return NULL;
    }
    printf("chip version: %x\n", version);

    // put in sleep mode
    SX1276sleep(SX1276);

    // initialize a bunch of registers here
    // no clue why this is needed, the driver should work without this too
    // but putting it in because its in the semtech/upm driver
    uint8_t i = 0;
    for( i = 0; i < sizeof( RadioRegsInit ) / sizeof( RadioRegisters_t ); i++ )
    {
        SX1276SetModem(SX1276, RadioRegsInit[i].Modem);
        writeRegister(SX1276, RadioRegsInit[i].Addr, RadioRegsInit[i].Value);
    }

    SX1276SetModem(SX1276, MODEM_LORA);

    // set frequency - to a default
    setFrequency(SX1276, frequency);

    // set base addresses
    writeRegister(SX1276, REG_FIFO_TX_BASE_ADDR, 0);
    writeRegister(SX1276, REG_FIFO_RX_BASE_ADDR, 0);

    // set LNA boost | can be used if needed
    //writeRegister(REG_LNA, readRegister(REG_LNA) | 0x23);

    // set auto AGC
    writeRegister(SX1276, REG_MODEM_CONFIG_3, 0x04);

    // set output power to 17 dBm
    // second argument is the default pin here
    // this power pin thing is weird, not a lot of mention of this
    // in the datasheet. Might be useful in conserving power.
    setTxPower(SX1276, 17, PA_OUTPUT_PA_BOOST_PIN);

    // put in standby mode
    idle(SX1276);

    return SX1276;
}

void end(sx1276_context SX1276) {
    // put in sleep mode
    SX1276sleep(SX1276);
}

int beginPacket(sx1276_context SX1276, int implicitHeader) {
    // put in standby mode
    idle(SX1276);

    // reset FIFO address and paload length
    writeRegister(SX1276, REG_FIFO_ADDR_PTR, 0);
    writeRegister(SX1276, REG_PAYLOAD_LENGTH, 0);

    return 1;
}

int endPacket(sx1276_context SX1276) {
    //printf("value of REG_IRQ_FLAGS very prior to clearing the interrupt: %x\n", readRegister(SX1276, REG_IRQ_FLAGS));

    // put in TX mode
    SX1276SetOpMode(SX1276, MODE_TX);

    // wait for TX done
    while((readRegister(SX1276, REG_IRQ_FLAGS) & RFLR_IRQFLAGS_TXDONE) == 0);

    printf("interrupt 0 detected\n");

    writeRegister(SX1276, REG_IRQ_FLAGS, 0x80);

    return 1;
}

// size is set to 0 by default
int parsePacket(sx1276_context SX1276, int size) {
    int packetLength = 0;
    int irqFlags = readRegister(SX1276, REG_IRQ_FLAGS);

    // clear IRQ's
    writeRegister(SX1276, REG_IRQ_FLAGS, irqFlags);

    if ((irqFlags & RFLR_IRQFLAGS_RXDONE_MASK) && (irqFlags & RFLR_IRQFLAGS_PAYLOADCRCERROR_MASK) == 0) {
        // received a packet
        SX1276->_packetIndex = 0;

        // read packet length
        if (SX1276->_implicitHeaderMode) {
            packetLength = readRegister(SX1276, REG_PAYLOAD_LENGTH);
        } else {
            packetLength = readRegister(SX1276, REG_RX_NB_BYTES);
        }

        // set FIFO address to current RX address
        writeRegister(SX1276, REG_FIFO_ADDR_PTR, readRegister(SX1276, REG_FIFO_RX_CURRENT_ADDR));

        // put in standby mode
        idle(SX1276);
    } else if (readRegister(SX1276, REG_OP_MODE) != (MODE_LONG_RANGE_MODE | MODE_RX_CONTINUOUS)) {
        // not currently in RX mode

        // reset FIFO address
        writeRegister(SX1276, REG_FIFO_ADDR_PTR, 0);

        // put in single RX mode
        // again using set opmode function
        //SX1276SetOpMode(MODE_RX_SINGLE);
        SX1276SetOpMode(SX1276, MODE_RX_CONTINUOUS);
    }

    return packetLength;
}

int packetRssi(sx1276_context SX1276, RadioModems_t modem)
{
    int rssi = 0;
    switch(modem) {
        case MODEM_FSK:
            rssi = -(readRegister(SX1276, REG_PKT_RSSI_VALUE) >> 1);
            break;
        case MODEM_LORA:
            rssi = readRegister(SX1276, REG_PKT_RSSI_VALUE) - (SX1276->_frequency < 868E6 ? 164 : 157);
            break;
        default:
            rssi = -1;
            break;
    }

    return rssi;
}

float packetSnr(sx1276_context SX1276)
{
    return ((int8_t)readRegister(SX1276, REG_PKT_SNR_VALUE)) * 0.25;
}

int write_buf(sx1276_context SX1276, const uint8_t *buffer, size_t size)
{
    int currentLength = readRegister(SX1276, REG_PAYLOAD_LENGTH);

    printf("current length: %d\n", currentLength);
    // check size
    if ((currentLength + size) > MAX_PKT_LENGTH) {
        size = MAX_PKT_LENGTH - currentLength;
    }

    // write data
    for (size_t i = 0; i < size; i++) {
        writeRegister(SX1276, REG_FIFO, buffer[i]);
    }

    // update length
    writeRegister(SX1276, REG_PAYLOAD_LENGTH, currentLength + size);

    return size;
}

int available(sx1276_context SX1276)
{
    return (readRegister(SX1276, REG_RX_NB_BYTES) - SX1276->_packetIndex);
}

int SX1276read(sx1276_context SX1276) {
    SX1276->_packetIndex++;

    return readRegister(SX1276, REG_FIFO);
}

int peek(sx1276_context SX1276) {
    if (!available(SX1276)) {
        return -1;
    }

    // store current FIFO address
    int currentAddress = readRegister(SX1276, REG_FIFO_ADDR_PTR);

    // read
    uint8_t b = readRegister(SX1276, REG_FIFO);

    // restore FIFO address
    writeRegister(SX1276, REG_FIFO_ADDR_PTR, currentAddress);

    return b;
}

void flush(sx1276_context SX1276) {
}

// the default size here is 0
void receive(sx1276_context SX1276, int size) {
    SX1276SetOpMode(SX1276, MODE_RX_CONTINUOUS);
}

void idle(sx1276_context SX1276) {
    // setting to idle
    SX1276SetOpMode(SX1276, MODE_STDBY);
}

void SX1276sleep(sx1276_context SX1276) {
    SX1276SetOpMode(SX1276, MODE_SLEEP);
}

void setTxPower(sx1276_context SX1276, int level, int outputPin) {
    if (PA_OUTPUT_RFO_PIN == outputPin) {
        // RFO
        if (level < 0) {
            level = 0;
        } else if (level > 14) {
            level = 14;
        }

        //printf("value to be written to REG_PA_CONFIG; %x\n", 0x70 | level);
        writeRegister(SX1276, REG_PA_CONFIG, 0x70 | level);
        //printf("REG_PA_CONFIG: %x\n", readRegister(SX1276, REG_PA_CONFIG));
    } else {
        // PA BOOST
        if (level < 2) {
            level = 2;
        } else if (level > 17) {
            level = 17;
        }

        //printf("value to be written to REG_PA_CONFIG; %x\n", PA_BOOST | (level - 2));
        writeRegister(SX1276, REG_PA_CONFIG, PA_BOOST | (level - 2));
        //printf("REG_PA_CONFIG: %x\n", readRegister(SX1276, REG_PA_CONFIG));
    }
}

void setFrequency(sx1276_context SX1276, long frequency) {

    //printf("setting Frequency to: %d\n", frequency);
    SX1276->_frequency = frequency;
    SX1276->Settings->Channel = frequency;
    // specific for certain boards
    frequency = frequency/1.00001732945;

    uint64_t frf = ((uint64_t)frequency << 19) / 32000000;

    writeRegister(SX1276, REG_FRF_MSB, (uint8_t)(frf >> 16));
    writeRegister(SX1276, REG_FRF_MID, (uint8_t)(frf >> 8));
    writeRegister(SX1276, REG_FRF_LSB, (uint8_t)(frf >> 0));
}

void setSpreadingFactor(sx1276_context SX1276, int sf) {
    if (sf < 6) {
        sf = 6;
    } else if (sf > 12) {
        sf = 12;
    }

    if (sf == 6) {
        writeRegister(SX1276, REG_DETECTION_OPTIMIZE, 0xc5);
        writeRegister(SX1276, REG_DETECTION_THRESHOLD, 0x0c);
    } else {
        writeRegister(SX1276, REG_DETECTION_OPTIMIZE, 0xc3);
        writeRegister(SX1276, REG_DETECTION_THRESHOLD, 0x0a);
    }

    writeRegister(SX1276, REG_MODEM_CONFIG_2, (readRegister(SX1276, REG_MODEM_CONFIG_2) & 0x0f) | ((sf << 4) & 0xf0));
}

void setSignalBandwidth(sx1276_context SX1276, long sbw) {
    int bw;

    if (sbw <= 7.8E3) {
        bw = 0;
    } else if (sbw <= 10.4E3) {
        bw = 1;
    } else if (sbw <= 15.6E3) {
        bw = 2;
    } else if (sbw <= 20.8E3) {
        bw = 3;
    } else if (sbw <= 31.25E3) {
        bw = 4;
    } else if (sbw <= 41.7E3) {
        bw = 5;
    } else if (sbw <= 62.5E3) {
        bw = 6;
    } else if (sbw <= 125E3) {
        bw = 7;
    } else if (sbw <= 250E3) {
        bw = 8;
    } else /*if (sbw <= 250E3)*/ {
        bw = 9;
    }

    writeRegister(SX1276, REG_MODEM_CONFIG_1, (readRegister(SX1276, REG_MODEM_CONFIG_1) & 0x0f) | (bw << 4));
}

void setCodingRate4(sx1276_context SX1276, int denominator) {
    if (denominator < 5) {
        denominator = 5;
    } else if (denominator > 8) {
        denominator = 8;
    }

    int cr = denominator - 4;

    writeRegister(SX1276, REG_MODEM_CONFIG_1, (readRegister(SX1276, REG_MODEM_CONFIG_1) & 0xf1) | (cr << 1));
}

void setPreambleLength(sx1276_context SX1276, long length) {
    writeRegister(SX1276, REG_PREAMBLE_MSB, (uint8_t)(length >> 8));
    writeRegister(SX1276, REG_PREAMBLE_LSB, (uint8_t)(length >> 0));
}

void setSyncWord(sx1276_context SX1276, int sw)
{
    writeRegister(SX1276, REG_SYNC_WORD, sw);
}

void crc(sx1276_context SX1276) {
    writeRegister(SX1276, REG_MODEM_CONFIG_2, readRegister(SX1276, REG_MODEM_CONFIG_2) | 0x04);
}

void noCrc(sx1276_context SX1276) {
    writeRegister(SX1276, REG_MODEM_CONFIG_2, readRegister(SX1276, REG_MODEM_CONFIG_2) & 0xfb);
}

int8_t random(sx1276_context SX1276) {
    return readRegister(SX1276, REG_RSSI_WIDEBAND);
}

void dumpRegisters(sx1276_context SX1276)
{
  for (int i = 0; i < 128; i++) {
    printf("0x%x: 0x%x\n", i, readRegister(SX1276, i));
  }
}

void explicitHeaderMode(sx1276_context SX1276) {
    SX1276->_implicitHeaderMode = 0;

    writeRegister(SX1276, REG_MODEM_CONFIG_1, readRegister(SX1276, REG_MODEM_CONFIG_1) & 0xfe);
}

void implicitHeaderMode(sx1276_context SX1276) {
    SX1276->_implicitHeaderMode = 1;

    writeRegister(SX1276, REG_MODEM_CONFIG_1, readRegister(SX1276, REG_MODEM_CONFIG_1) | 0x01);
}

void handleDio0Rise(sx1276_context SX1276) {
    int irqFlags = readRegister(SX1276, REG_IRQ_FLAGS);

    // clear IRQ's
    writeRegister(SX1276, REG_IRQ_FLAGS, irqFlags);

    if ((irqFlags & RFLR_IRQFLAGS_PAYLOADCRCERROR_MASK) == 0) {
        // received a packet
        SX1276->_packetIndex = 0;

        // read packet length
        int packetLength = SX1276->_implicitHeaderMode ? readRegister(SX1276, REG_PAYLOAD_LENGTH) : readRegister(SX1276, REG_RX_NB_BYTES);

        // set FIFO address to current RX address
        writeRegister(SX1276, REG_FIFO_ADDR_PTR, readRegister(SX1276, REG_FIFO_RX_CURRENT_ADDR));

        // reset FIFO address
        writeRegister(SX1276, REG_FIFO_ADDR_PTR, 0);
    }
}

uint8_t readRegister(sx1276_context SX1276, uint8_t address) {

    uint8_t tx_buf[2] = { address & 0x7f, 0 };
    uint8_t rx_buf[2];

    if(SX1276->gpio_ss == NULL)
    	printf("gpio ss is null\n");
    mraa_gpio_write(SX1276->gpio_ss, 0);
    if (mraa_spi_transfer_buf(SX1276->spi, tx_buf, rx_buf, 2) != MRAA_SUCCESS) {
        printf("Unable to transfer data over the SPI bus\n");
        printf("SPI operation failed\n");
        mraa_gpio_write(SX1276->gpio_ss, 1);
        return -1;
    }
    mraa_gpio_write(SX1276->gpio_ss, 1);

    return rx_buf[1];
}

void writeRegister(sx1276_context SX1276, uint8_t address, uint8_t value) {

    uint8_t pkt[2] = {(uint8_t)(address | 0x80), value};

    mraa_gpio_write(SX1276->gpio_ss, 0);
    if (mraa_spi_transfer_buf(SX1276->spi, pkt, NULL, 2) != MRAA_SUCCESS) {
        printf("Unable to transfer data over the SPI bus\n");
        printf("SPI operation failed\n");
        mraa_gpio_write(SX1276->gpio_ss, 1);
    }
    mraa_gpio_write(SX1276->gpio_ss, 1);
}

// taken from the semtech driver
// modified
void SX1276SetModem(sx1276_context SX1276, RadioModems_t modem) {
    SX1276->Settings->Modem = modem;
    switch(modem) {
        default:
        case MODEM_LORA:
            SX1276sleep(SX1276);
            writeRegister(SX1276, REG_OP_MODE, readRegister(SX1276, REG_OP_MODE) | MODE_LONG_RANGE_MODE);

            // this part is right out of the driver
            writeRegister(SX1276, REG_DIO_MAPPING_1, 0x00);
            writeRegister(SX1276, REG_DIO_MAPPING_2, 0x00);
            break;
        case MODEM_FSK:
            SX1276sleep(SX1276);
            writeRegister(SX1276, REG_OP_MODE, readRegister(SX1276, REG_OP_MODE) & MODE_LONG_RANGE_MODE_OFF);

            // this part is right out of the driver
            writeRegister(SX1276, REG_DIO_MAPPING_1, 0x00);
            writeRegister(SX1276, REG_DIO_MAPPING_2, 0x30);    // DIO5 = ModeReady
            break;
    }
}

void SX1276SetOpMode(sx1276_context SX1276, uint8_t mode) {
    writeRegister(SX1276, REG_OP_MODE, (readRegister(SX1276, REG_OP_MODE)&OP_MODE_MASK) | mode);
}

void SX1276RxChainCalibration(sx1276_context SX1276) {
    printf("Entering rx chain calibration\n");
    uint8_t regPaConfigInitVal;
    uint8_t initialFreq_msb;
    uint8_t initialFreq_mid;
    uint8_t initialFreq_lsb;

    // Save context
    // this part has been changed somewhat from the original semtech/upm driver
    // avoiding a call to the set frequency function would be easier here
    regPaConfigInitVal = readRegister(SX1276, REG_PA_CONFIG);
    initialFreq_msb = readRegister(SX1276, REG_FRF_MSB);
    initialFreq_mid = readRegister(SX1276, REG_FRF_MID);
    initialFreq_lsb = readRegister(SX1276, REG_FRF_LSB);

    // Cut the PA just in case, RFO output, power = -1 dBm
    writeRegister(SX1276, REG_PA_CONFIG, 0x00);

    // Launch Rx chain calibration for LF band
    writeRegister(SX1276, REG_IMAGE_CAL, ( readRegister(SX1276, REG_IMAGE_CAL) & RF_IMAGECAL_IMAGECAL_MASK ) | RF_IMAGECAL_IMAGECAL_START );
    while( ( readRegister(SX1276, REG_IMAGE_CAL) & RF_IMAGECAL_IMAGECAL_RUNNING ) == RF_IMAGECAL_IMAGECAL_RUNNING )
    {}

    // Sets a Frequency in HF band
    setFrequency(SX1276, 868000000);

    // Launch Rx chain calibration for HF band
    writeRegister(SX1276, REG_IMAGE_CAL, ( readRegister(SX1276, REG_IMAGE_CAL) & RF_IMAGECAL_IMAGECAL_MASK ) | RF_IMAGECAL_IMAGECAL_START );
    while( ( readRegister(SX1276, REG_IMAGE_CAL) & RF_IMAGECAL_IMAGECAL_RUNNING ) == RF_IMAGECAL_IMAGECAL_RUNNING )
    {}

    // Restore context
    writeRegister(SX1276, REG_PA_CONFIG, regPaConfigInitVal);

    writeRegister(SX1276, REG_FRF_MSB, initialFreq_msb);
    writeRegister(SX1276, REG_FRF_MID, initialFreq_mid);
    writeRegister(SX1276, REG_FRF_LSB, initialFreq_lsb);
}

// not sure of the use of this function but putting it in
bool SX1276IsChannelFree(sx1276_context SX1276, RadioModems_t modem, uint32_t freq, int16_t rssiThresh)
{
    int rssi = 0;

    SX1276SetModem(SX1276, modem);

    setFrequency(SX1276, freq);

    SX1276SetOpMode(SX1276, MODE_RX_CONTINUOUS);

    upm_delay_us(1000);

    rssi = packetRssi(SX1276, modem);

    SX1276sleep(SX1276);

    if( rssi > rssiThresh )
    {
        return false;
    }
    return true;
}

uint32_t SX1276Random(sx1276_context SX1276)
{
    uint8_t i;
    uint32_t rnd = 0;
    /*
     * Radio setup for random number generation
     */
    // Set LoRa modem ON
    SX1276SetModem(SX1276, MODEM_LORA);

    // Disable LoRa modem interrupts
    writeRegister(SX1276, REG_LR_IRQ_FLAGS_MASK, RFLR_IRQFLAGS_RXTIMEOUT |
                  RFLR_IRQFLAGS_RXDONE |
                  RFLR_IRQFLAGS_PAYLOADCRCERROR |
                  RFLR_IRQFLAGS_VALIDHEADER |
                  RFLR_IRQFLAGS_TXDONE |
                  RFLR_IRQFLAGS_CADDONE |
                  RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL |
                  RFLR_IRQFLAGS_CADDETECTED );

    // Set radio in continuous reception
    SX1276SetOpMode(SX1276, MODE_RX_CONTINUOUS);

    for( i = 0; i < 32; i++ )
    {
        upm_delay_us(1000);
        // Unfiltered RSSI value reading. Only takes the LSB value
        rnd |= ( ( uint32_t )readRegister(SX1276, REG_RSSI_WIDEBAND) & 0x01 ) << i;
    }

    SX1276sleep(SX1276);

    return rnd;
}

/*!
 * Returns the known FSK bandwidth registers value
 *
 * \param [IN] bandwidth Bandwidth value in Hz
 * \retval regValue Bandwidth register value.
 */
static uint8_t SX1276GetFskBandwidthRegValue(sx1276_context SX1276, uint32_t bandwidth)
{
    uint8_t i;

    for( i = 0; i < ( sizeof( FskBandwidths ) / sizeof( FskBandwidth_t ) ) - 1; i++ )
    {
        if( ( bandwidth >= FskBandwidths[i].bandwidth ) && ( bandwidth < FskBandwidths[i + 1].bandwidth ) )
        {
            return FskBandwidths[i].RegValue;
        }
    }
    // ERROR: Value not found
    //while( 1 );
    printf("BW value not found\n");
    return 0;
}

void SX1276SetRxConfig(sx1276_context SX1276, RadioModems_t modem, uint32_t bandwidth,
                         uint32_t datarate, uint8_t coderate,
                         uint32_t bandwidthAfc, uint16_t preambleLen,
                         uint16_t symbTimeout, bool fixLen,
                         uint8_t payloadLen,
                         bool crcOn, bool freqHopOn, uint8_t hopPeriod,
                         bool iqInverted, bool rxContinuous ) {

    SX1276SetModem(SX1276, modem);

    switch(modem){
        case MODEM_FSK:
            {
            SX1276->Settings->Fsk->Bandwidth = bandwidth;
            SX1276->Settings->Fsk->Datarate = datarate;
            SX1276->Settings->Fsk->BandwidthAfc = bandwidthAfc;
            SX1276->Settings->Fsk->FixLen = fixLen;
            SX1276->Settings->Fsk->PayloadLen = payloadLen;
            SX1276->Settings->Fsk->CrcOn = crcOn;
            SX1276->Settings->Fsk->IqInverted = iqInverted;
            SX1276->Settings->Fsk->RxContinuous = rxContinuous;
            SX1276->Settings->Fsk->PreambleLen = preambleLen;

            datarate = ( uint16_t )( ( double )XTAL_FREQ / ( double )datarate );
            writeRegister( SX1276, REG_BITRATE_MSB, ( uint8_t )( datarate >> 8 ) );
            writeRegister( SX1276, REG_BITRATE_LSB, ( uint8_t )( datarate & 0xFF ) );

            writeRegister( SX1276, REG_RXBW, SX1276GetFskBandwidthRegValue( SX1276, bandwidth ) );
            writeRegister( SX1276, REG_AFCBW, SX1276GetFskBandwidthRegValue( SX1276, bandwidthAfc ) );

            writeRegister( SX1276, REG_PREAMBLEMSB, ( uint8_t )( ( preambleLen >> 8 ) & 0xFF ) );
            writeRegister( SX1276, REG_PREAMBLELSB, ( uint8_t )( preambleLen & 0xFF ) );

            if( fixLen == 1 )
            {
                writeRegister( SX1276, REG_FSK_PAYLOADLENGTH, payloadLen );
            }
            else
            {
                writeRegister( SX1276, REG_FSK_PAYLOADLENGTH, 0xFF ); // Set payload length to the maximum
            }

            writeRegister( SX1276, REG_PACKETCONFIG1,
                         ( readRegister( SX1276, REG_PACKETCONFIG1 ) &
                           RF_PACKETCONFIG1_CRC_MASK &
                           RF_PACKETCONFIG1_PACKETFORMAT_MASK ) |
                           ( ( fixLen == 1 ) ? RF_PACKETCONFIG1_PACKETFORMAT_FIXED : RF_PACKETCONFIG1_PACKETFORMAT_VARIABLE ) |
                           ( crcOn << 4 ) );
            writeRegister( SX1276, REG_PACKETCONFIG2, ( readRegister( SX1276, REG_PACKETCONFIG2 ) | RF_PACKETCONFIG2_DATAMODE_PACKET ) );
        }
        break;

        case MODEM_LORA:
        {
            if( bandwidth > 2 )
            {
                // Fatal error: When using LoRa modem only bandwidths 125, 250 and 500 kHz are supported
                while( 1 );
            }
            bandwidth += 7;
            SX1276->Settings->LoRa->Bandwidth = bandwidth;
            SX1276->Settings->LoRa->Datarate = datarate;
            SX1276->Settings->LoRa->Coderate = coderate;
            SX1276->Settings->LoRa->PreambleLen = preambleLen;
            SX1276->Settings->LoRa->FixLen = fixLen;
            SX1276->Settings->LoRa->PayloadLen = payloadLen;
            SX1276->Settings->LoRa->CrcOn = crcOn;
            SX1276->Settings->LoRa->FreqHopOn = freqHopOn;
            SX1276->Settings->LoRa->HopPeriod = hopPeriod;
            SX1276->Settings->LoRa->IqInverted = iqInverted;
            SX1276->Settings->LoRa->RxContinuous = rxContinuous;

            if( datarate > 12 )
            {
                datarate = 12;
            }
            else if( datarate < 6 )
            {
                datarate = 6;
            }

            if( ( ( bandwidth == 7 ) && ( ( datarate == 11 ) || ( datarate == 12 ) ) ) ||
                ( ( bandwidth == 8 ) && ( datarate == 12 ) ) )
            {
                SX1276->Settings->LoRa->LowDatarateOptimize = 0x01;
            }
            else
            {
                SX1276->Settings->LoRa->LowDatarateOptimize = 0x00;
            }

            writeRegister( SX1276, REG_MODEM_CONFIG_1,
                         ( readRegister( SX1276, REG_MODEM_CONFIG_1 ) &
                           RFLR_MODEMCONFIG1_BW_MASK &
                           RFLR_MODEMCONFIG1_CODINGRATE_MASK &
                           RFLR_MODEMCONFIG1_IMPLICITHEADER_MASK ) |
                           ( bandwidth << 4 ) | ( coderate << 1 ) |
                           fixLen );

            writeRegister( SX1276, REG_MODEM_CONFIG_2,
                         ( readRegister( SX1276, REG_MODEM_CONFIG_2 ) &
                           RFLR_MODEMCONFIG2_SF_MASK &
                           RFLR_MODEMCONFIG2_RXPAYLOADCRC_MASK &
                           RFLR_MODEMCONFIG2_SYMBTIMEOUTMSB_MASK ) |
                           ( datarate << 4 ) | ( crcOn << 2 ) |
                           ( ( symbTimeout >> 8 ) & ~RFLR_MODEMCONFIG2_SYMBTIMEOUTMSB_MASK ) );

            writeRegister( SX1276, REG_MODEM_CONFIG_3,
                         ( readRegister( SX1276, REG_MODEM_CONFIG_3 ) &
                           RFLR_MODEMCONFIG3_LOWDATARATEOPTIMIZE_MASK ) |
                           ( SX1276->Settings->LoRa->LowDatarateOptimize << 3 ) );

            writeRegister( SX1276, REG_LR_SYMBTIMEOUT_LSB, ( uint8_t )( symbTimeout & 0xFF ) );

            writeRegister( SX1276, REG_PREAMBLE_MSB, ( uint8_t )( ( preambleLen >> 8 ) & 0xFF ) );
            writeRegister( SX1276, REG_PREAMBLE_LSB, ( uint8_t )( preambleLen & 0xFF ) );

            if( fixLen == 1 )
            {
                writeRegister( SX1276, REG_PAYLOAD_LENGTH, payloadLen );
            }

            if( SX1276->Settings->LoRa->FreqHopOn == true )
            {
                writeRegister( SX1276, REG_LR_PLLHOP, ( readRegister( SX1276, REG_LR_PLLHOP ) & RFLR_PLLHOP_FASTHOP_MASK ) | RFLR_PLLHOP_FASTHOP_ON );
                writeRegister( SX1276, REG_LR_HOPPERIOD, SX1276->Settings->LoRa->HopPeriod );
            }

            if( ( bandwidth == 9 ) && ( SX1276->Settings->Channel > RF_MID_BAND_THRESH ) )
            {
                // ERRATA 2.1 - Sensitivity Optimization with a 500 kHz Bandwidth
                writeRegister( SX1276, REG_LR_TEST36, 0x02 );
                writeRegister( SX1276, REG_LR_TEST3A, 0x64 );
            }
            else if( bandwidth == 9 )
            {
                // ERRATA 2.1 - Sensitivity Optimization with a 500 kHz Bandwidth
                writeRegister( SX1276, REG_LR_TEST36, 0x02 );
                writeRegister( SX1276, REG_LR_TEST3A, 0x7F );
            }
            else
            {
                // ERRATA 2.1 - Sensitivity Optimization with a 500 kHz Bandwidth
                writeRegister( SX1276, REG_LR_TEST36, 0x03 );
            }

            if( datarate == 6 )
            {
                writeRegister( SX1276, REG_LR_DETECTOPTIMIZE,
                             ( readRegister( SX1276, REG_LR_DETECTOPTIMIZE ) &
                               RFLR_DETECTIONOPTIMIZE_MASK ) |
                               RFLR_DETECTIONOPTIMIZE_SF6 );
                writeRegister( SX1276, REG_LR_DETECTIONTHRESHOLD,
                             RFLR_DETECTIONTHRESH_SF6 );
            }
            else
            {
                writeRegister( SX1276, REG_LR_DETECTOPTIMIZE,
                             ( readRegister( SX1276, REG_LR_DETECTOPTIMIZE ) &
                             RFLR_DETECTIONOPTIMIZE_MASK ) |
                             RFLR_DETECTIONOPTIMIZE_SF7_TO_SF12 );
                writeRegister( SX1276, REG_LR_DETECTIONTHRESHOLD,
                             RFLR_DETECTIONTHRESH_SF7_TO_SF12 );
            }
        }
        break;
    }
}


void SX1276SetTxConfig( sx1276_context SX1276, RadioModems_t modem, int8_t power, uint32_t fdev,
                        uint32_t bandwidth, uint32_t datarate,
                        uint8_t coderate, uint16_t preambleLen,
                        bool fixLen, bool crcOn, bool freqHopOn,
                        uint8_t hopPeriod, bool iqInverted, uint32_t timeout ) {

    SX1276SetModem(SX1276, modem);
    // not sure why this function is only called to set tx power
    // might have to set this explicitly for the rx side
    setTxPower(SX1276, power, PA_OUTPUT_PA_BOOST_PIN);

    switch( modem )
    {
    case MODEM_FSK:
        {
        	SX1276->Settings->Fsk->Power = power;
            SX1276->Settings->Fsk->Fdev = fdev;
            SX1276->Settings->Fsk->Bandwidth = bandwidth;
            SX1276->Settings->Fsk->Datarate = datarate;
            SX1276->Settings->Fsk->PreambleLen = preambleLen;
            SX1276->Settings->Fsk->FixLen = fixLen;
            SX1276->Settings->Fsk->CrcOn = crcOn;
            SX1276->Settings->Fsk->IqInverted = iqInverted;
            SX1276->Settings->Fsk->TxTimeout = timeout;

            fdev = ( uint16_t )( ( double )fdev / ( double )FREQ_STEP );
            writeRegister( SX1276, REG_FDEVMSB, ( uint8_t )( fdev >> 8 ) );
            writeRegister( SX1276, REG_FDEVLSB, ( uint8_t )( fdev & 0xFF ) );

            datarate = ( uint16_t )( ( double )XTAL_FREQ / ( double )datarate );
            writeRegister( SX1276, REG_BITRATE_MSB, ( uint8_t )( datarate >> 8 ) );
            writeRegister( SX1276, REG_BITRATE_LSB, ( uint8_t )( datarate & 0xFF ) );

            writeRegister( SX1276, REG_PREAMBLEMSB, ( preambleLen >> 8 ) & 0x00FF );
            writeRegister( SX1276, REG_PREAMBLELSB, preambleLen & 0xFF );

            writeRegister( SX1276, REG_PACKETCONFIG1,
                         ( readRegister( SX1276, REG_PACKETCONFIG1 ) &
                           RF_PACKETCONFIG1_CRC_MASK &
                           RF_PACKETCONFIG1_PACKETFORMAT_MASK ) |
                           ( ( fixLen == 1 ) ? RF_PACKETCONFIG1_PACKETFORMAT_FIXED : RF_PACKETCONFIG1_PACKETFORMAT_VARIABLE ) |
                           ( crcOn << 4 ) );
            writeRegister( SX1276, REG_PACKETCONFIG2, ( readRegister( SX1276, REG_PACKETCONFIG2 ) | RF_PACKETCONFIG2_DATAMODE_PACKET ) );
        }
        break;
    case MODEM_LORA:
        {
            SX1276->Settings->LoRa->Power = power;
            if( bandwidth > 2 )
            {
                // Fatal error: When using LoRa modem only bandwidths 125, 250 and 500 kHz are supported
                while( 1 );
            }
            bandwidth += 7;
            SX1276->Settings->LoRa->Bandwidth = bandwidth;
            SX1276->Settings->LoRa->Datarate = datarate;
            SX1276->Settings->LoRa->Coderate = coderate;
            SX1276->Settings->LoRa->PreambleLen = preambleLen;
            SX1276->Settings->LoRa->FixLen = fixLen;
            SX1276->Settings->LoRa->FreqHopOn = freqHopOn;
            SX1276->Settings->LoRa->HopPeriod = hopPeriod;
            SX1276->Settings->LoRa->CrcOn = crcOn;
            SX1276->Settings->LoRa->IqInverted = iqInverted;
            SX1276->Settings->LoRa->TxTimeout = timeout;

            if( datarate > 12 )
            {
                datarate = 12;
            }
            else if( datarate < 6 )
            {
                datarate = 6;
            }
            if( ( ( bandwidth == 7 ) && ( ( datarate == 11 ) || ( datarate == 12 ) ) ) ||
                ( ( bandwidth == 8 ) && ( datarate == 12 ) ) )
            {
                SX1276->Settings->LoRa->LowDatarateOptimize = 0x01;
            }
            else
            {
                SX1276->Settings->LoRa->LowDatarateOptimize = 0x00;
            }

            if( SX1276->Settings->LoRa->FreqHopOn == true )
            {
                writeRegister( SX1276, REG_LR_PLLHOP, ( readRegister( SX1276, REG_LR_PLLHOP ) & RFLR_PLLHOP_FASTHOP_MASK ) | RFLR_PLLHOP_FASTHOP_ON );
                writeRegister( SX1276, REG_LR_HOPPERIOD, SX1276->Settings->LoRa->HopPeriod );
            }

            writeRegister( SX1276, REG_MODEM_CONFIG_1,
                         ( readRegister( SX1276, REG_MODEM_CONFIG_1 ) &
                           RFLR_MODEMCONFIG1_BW_MASK &
                           RFLR_MODEMCONFIG1_CODINGRATE_MASK &
                           RFLR_MODEMCONFIG1_IMPLICITHEADER_MASK ) |
                           ( bandwidth << 4 ) | ( coderate << 1 ) |
                           fixLen );

            writeRegister( SX1276, REG_MODEM_CONFIG_2,
                         ( readRegister( SX1276, REG_MODEM_CONFIG_2 ) &
                           RFLR_MODEMCONFIG2_SF_MASK &
                           RFLR_MODEMCONFIG2_RXPAYLOADCRC_MASK ) |
                           ( datarate << 4 ) | ( crcOn << 2 ) );

            writeRegister( SX1276, REG_MODEM_CONFIG_3,
                         ( readRegister( SX1276, REG_MODEM_CONFIG_3 ) &
                           RFLR_MODEMCONFIG3_LOWDATARATEOPTIMIZE_MASK ) |
                           ( SX1276->Settings->LoRa->LowDatarateOptimize << 3 ) );

            writeRegister( SX1276, REG_PREAMBLE_MSB, ( preambleLen >> 8 ) & 0x00FF );
            writeRegister( SX1276, REG_PREAMBLE_LSB, preambleLen & 0xFF );

            if( datarate == 6 )
            {
                writeRegister( SX1276, REG_LR_DETECTOPTIMIZE,
                             ( readRegister( SX1276, REG_LR_DETECTOPTIMIZE ) &
                               RFLR_DETECTIONOPTIMIZE_MASK ) |
                               RFLR_DETECTIONOPTIMIZE_SF6 );
                writeRegister( SX1276, REG_LR_DETECTIONTHRESHOLD,
                             RFLR_DETECTIONTHRESH_SF6 );
            }
            else
            {
                writeRegister( SX1276, REG_LR_DETECTOPTIMIZE,
                             ( readRegister( SX1276, REG_LR_DETECTOPTIMIZE ) &
                             RFLR_DETECTIONOPTIMIZE_MASK ) |
                             RFLR_DETECTIONOPTIMIZE_SF7_TO_SF12 );
                writeRegister( SX1276, REG_LR_DETECTIONTHRESHOLD,
                             RFLR_DETECTIONTHRESH_SF7_TO_SF12 );
            }
        }
        break;
    }
}

uint32_t SX1276GetTimeOnAir( sx1276_context SX1276, RadioModems_t modem, uint8_t pktLen )
{
    uint32_t airTime = 0;

    switch( modem )
    {
    case MODEM_FSK:
        {
            airTime = round( ( 8 * ( SX1276->Settings->Fsk->PreambleLen +
                                     ( ( readRegister( SX1276, REG_SYNCCONFIG ) & ~RF_SYNCCONFIG_SYNCSIZE_MASK ) + 1 ) +
                                     ( ( SX1276->Settings->Fsk->FixLen == 0x01 ) ? 0.0 : 1.0 ) +
                                     ( ( ( readRegister( SX1276, REG_PACKETCONFIG1 ) & ~RF_PACKETCONFIG1_ADDRSFILTERING_MASK ) != 0x00 ) ? 1.0 : 0 ) +
                                     pktLen +
                                     ( ( SX1276->Settings->Fsk->CrcOn == 0x01 ) ? 2.0 : 0 ) ) /
                                     SX1276->Settings->Fsk->Datarate ) * 1e3 );
        }
        break;
    case MODEM_LORA:
        {
            double bw = 0.0;
            // REMARK: When using LoRa modem only bandwidths 125, 250 and 500 kHz are supported
            switch( SX1276->Settings->LoRa->Bandwidth )
            {
            //case 0: // 7.8 kHz
            //    bw = 78e2;
            //    break;
            //case 1: // 10.4 kHz
            //    bw = 104e2;
            //    break;
            //case 2: // 15.6 kHz
            //    bw = 156e2;
            //    break;
            //case 3: // 20.8 kHz
            //    bw = 208e2;
            //    break;
            //case 4: // 31.2 kHz
            //    bw = 312e2;
            //    break;
            //case 5: // 41.4 kHz
            //    bw = 414e2;
            //    break;
            //case 6: // 62.5 kHz
            //    bw = 625e2;
            //    break;
            case 7: // 125 kHz
                bw = 125e3;
                break;
            case 8: // 250 kHz
                bw = 250e3;
                break;
            case 9: // 500 kHz
                bw = 500e3;
                break;
            }

            // Symbol rate : time for one symbol (secs)
            double rs = bw / ( 1 << SX1276->Settings->LoRa->Datarate );
            double ts = 1 / rs;
            // time of preamble
            double tPreamble = ( SX1276->Settings->LoRa->PreambleLen + 4.25 ) * ts;
            // Symbol length of payload and time
            double tmp = ceil( ( 8 * pktLen - 4 * SX1276->Settings->LoRa->Datarate +
                                 28 + 16 * SX1276->Settings->LoRa->CrcOn -
                                 ( SX1276->Settings->LoRa->FixLen ? 20 : 0 ) ) /
                                 ( double )( 4 * ( SX1276->Settings->LoRa->Datarate -
                                 ( ( SX1276->Settings->LoRa->LowDatarateOptimize > 0 ) ? 2 : 0 ) ) ) ) *
                                 ( SX1276->Settings->LoRa->Coderate + 4 );
            double nPayload = 8 + ( ( tmp > 0 ) ? tmp : 0 );
            double tPayload = nPayload * ts;
            // Time on air
            double tOnAir = tPreamble + tPayload;
            // return ms secs
            airTime = floor( tOnAir * 1e3 + 0.999 );
        }
        break;
    }
    return airTime;
}

// this will have a call to begin packet
void SX1276Send(sx1276_context SX1276, uint8_t *buffer, uint8_t size)
{
#if LORA_DEBUG
    printf("coming into SX1276Send, modem: %d\n", SX1276->Settings->Modem);
#endif
    uint32_t txTimeout = 0;

    switch(SX1276->Settings->Modem)
    {
    case MODEM_FSK:
        {
            SX1276->Settings->FskPacketHandler->NbBytes = 0;
            SX1276->Settings->FskPacketHandler->Size = size;

            if( SX1276->Settings->Fsk->FixLen == false )
            {
                write_buf(SX1276, ( uint8_t* )&size, 1);
            }
            else
            {
                writeRegister( SX1276, REG_PAYLOADLENGTH, size );
            }

            if( ( size > 0 ) && ( size <= 64 ) )
            {
                SX1276->Settings->FskPacketHandler->ChunkSize = size;
            }
            else
            {
            	memcpy( SX1276->RxTxBuffer, buffer, size );
                SX1276->Settings->FskPacketHandler->ChunkSize = 32;
            }

            // Write payload buffer
            write_buf( SX1276, buffer, SX1276->Settings->FskPacketHandler->ChunkSize );
            SX1276->Settings->FskPacketHandler->NbBytes += SX1276->Settings->FskPacketHandler->ChunkSize;
            txTimeout = SX1276->Settings->Fsk->TxTimeout;
        }
        break;
    case MODEM_LORA:
        {
            if( SX1276->Settings->LoRa->IqInverted == true )
            {
                writeRegister(SX1276, REG_LR_INVERTIQ, ((readRegister(SX1276, REG_LR_INVERTIQ) & RFLR_INVERTIQ_TX_MASK & RFLR_INVERTIQ_RX_MASK) | RFLR_INVERTIQ_RX_OFF | RFLR_INVERTIQ_TX_ON));
                writeRegister( SX1276, REG_LR_INVERTIQ2, RFLR_INVERTIQ2_ON );
            }
            else
            {
                writeRegister( SX1276, REG_LR_INVERTIQ, ( ( readRegister( SX1276, REG_LR_INVERTIQ ) & RFLR_INVERTIQ_TX_MASK & RFLR_INVERTIQ_RX_MASK ) | RFLR_INVERTIQ_RX_OFF | RFLR_INVERTIQ_TX_OFF ) );
                writeRegister( SX1276, REG_LR_INVERTIQ2, RFLR_INVERTIQ2_OFF );
            }

            SX1276->Settings->LoRaPacketHandler->Size = size;

            // Initializes the payload size
            writeRegister( SX1276, REG_PAYLOAD_LENGTH, 0 );

            // Full buffer used for Tx
            // make sure you get these right
            writeRegister(SX1276, REG_FIFO_TX_BASE_ADDR, 0);  // not sure
            // this is usual
            writeRegister(SX1276, REG_FIFO_ADDR_PTR, 0);

            // FIFO operations can not take place in Sleep mode
            if( ( readRegister( SX1276, REG_OP_MODE ) & ~OP_MODE_MASK ) == MODE_SLEEP )
            {
                // setting to standby mode
                idle(SX1276);
                upm_delay_us(1000);
            }

            // Write payload buffer
            write_buf(SX1276, buffer, size);
            txTimeout = SX1276->Settings->LoRa->TxTimeout;
        }
        break;
    }

    SX1276SetTx(SX1276, txTimeout);
}

void SX1276SetTx(sx1276_context SX1276, uint32_t timeout)
{
    switch(SX1276->Settings->Modem)
    {
    case MODEM_FSK:
        {
            // DIO0=PacketSent
            // DIO1=FifoEmpty
            // DIO2=FifoFull
            // DIO3=FifoEmpty
            // DIO4=LowBat
            // DIO5=ModeReady
            writeRegister( SX1276, REG_DIO_MAPPING_1, ( readRegister( SX1276, REG_DIO_MAPPING_1 ) & RF_DIOMAPPING1_DIO0_MASK &
                                                                            RF_DIOMAPPING1_DIO1_MASK &
                                                                            RF_DIOMAPPING1_DIO2_MASK ) |
                                                                            RF_DIOMAPPING1_DIO1_01 );

            writeRegister( SX1276, REG_DIO_MAPPING_2, ( readRegister( SX1276, REG_DIO_MAPPING_2 ) & RF_DIOMAPPING2_DIO4_MASK &
                                                                            RF_DIOMAPPING2_MAP_MASK ) );
            SX1276->Settings->FskPacketHandler->FifoThresh = readRegister( SX1276, REG_FIFOTHRESH ) & 0x3F;
        }
        break;
    case MODEM_LORA:
        {
            if( SX1276->Settings->LoRa->FreqHopOn == true )
            {
                writeRegister( SX1276, REG_LR_IRQ_FLAGS_MASK, RFLR_IRQFLAGS_RXTIMEOUT |
                                                  RFLR_IRQFLAGS_RXDONE |
                                                  RFLR_IRQFLAGS_PAYLOADCRCERROR |
                                                  RFLR_IRQFLAGS_VALIDHEADER |
                                                  //RFLR_IRQFLAGS_TXDONE |
                                                  RFLR_IRQFLAGS_CADDONE |
                                                  //RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL |
                                                  RFLR_IRQFLAGS_CADDETECTED );

                // DIO0=TxDone, DIO2=FhssChangeChannel
                writeRegister(SX1276, REG_DIO_MAPPING_1, (readRegister(SX1276, REG_DIO_MAPPING_1) & RFLR_DIOMAPPING1_DIO0_MASK & RFLR_DIOMAPPING1_DIO2_MASK ) | RFLR_DIOMAPPING1_DIO0_01 | RFLR_DIOMAPPING1_DIO2_00 );
            }
            else
            {
                writeRegister( SX1276, REG_LR_IRQ_FLAGS_MASK, RFLR_IRQFLAGS_RXTIMEOUT |
                                                  RFLR_IRQFLAGS_RXDONE |
                                                  RFLR_IRQFLAGS_PAYLOADCRCERROR |
                                                  RFLR_IRQFLAGS_VALIDHEADER |
                                                  //RFLR_IRQFLAGS_TXDONE |
                                                  RFLR_IRQFLAGS_CADDONE |
                                                  RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL |
                                                  RFLR_IRQFLAGS_CADDETECTED );

                // DIO0=TxDone
                writeRegister(SX1276, REG_DIO_MAPPING_1, (readRegister(SX1276, REG_DIO_MAPPING_1) & RFLR_DIOMAPPING1_DIO0_MASK ) | RFLR_DIOMAPPING1_DIO0_01 );
            }
        }
        break;
    }

    SX1276->Settings->State = RF_TX_RUNNING;
    SX1276SetOpMode(SX1276, MODE_TX);

    int count = 0;
    int irqFlags;
    // wait for TX done
    while(count < timeout) {
        irqFlags = readRegister(SX1276, REG_IRQ_FLAGS);
        // condition here for tx done
        writeRegister(SX1276, REG_IRQ_FLAGS, irqFlags);
        if(irqFlags & RFLR_IRQFLAGS_TXDONE) {
            // basically here we've got a tx done flag
            // clearing up the flags here
            printf("Transmission done\n");
            SX1276OnDio0Irq(SX1276);
            break;
        } else if (irqFlags & RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL_MASK) {
            // fhss change channel interrupt if dio2 is set to 00
            SX1276OnDio2Irq(SX1276);
        }
        count++;
        // 1 ms delay
        upm_delay_us(1000);
    }
    // this is where the timeout needs to be handled

    // clearing the register in case the interrupt occured
}

// comments to be added
RadioStateExt_t SX1276SetRx( sx1276_context SX1276, uint32_t timeout )
{
    bool rxContinuous = false;

    switch( SX1276->Settings->Modem )
    {
    case MODEM_FSK:
        {
            rxContinuous = SX1276->Settings->Fsk->RxContinuous;

            // DIO0=PayloadReady
            // DIO1=FifoLevel
            // DIO2=SyncAddr
            // DIO3=FifoEmpty
            // DIO4=Preamble
            // DIO5=ModeReady
            writeRegister( SX1276, REG_DIO_MAPPING_1, ( readRegister( SX1276, REG_DIO_MAPPING_1 ) & RF_DIOMAPPING1_DIO0_MASK &
                                                                            RF_DIOMAPPING1_DIO1_MASK &
                                                                            RF_DIOMAPPING1_DIO2_MASK ) |
                                                                            RF_DIOMAPPING1_DIO0_00 |
                                                                            RF_DIOMAPPING1_DIO1_00 |
                                                                            RF_DIOMAPPING1_DIO2_11 );

            writeRegister( SX1276, REG_DIO_MAPPING_2, ( readRegister( SX1276, REG_DIO_MAPPING_2 ) & RF_DIOMAPPING2_DIO4_MASK &
                                                                            RF_DIOMAPPING2_MAP_MASK ) |
                                                                            RF_DIOMAPPING2_DIO4_11 |
                                                                            RF_DIOMAPPING2_MAP_PREAMBLEDETECT );

            SX1276->Settings->FskPacketHandler->FifoThresh = readRegister( SX1276, REG_FIFOTHRESH ) & 0x3F;

            writeRegister( SX1276, REG_RXCONFIG, RF_RXCONFIG_AFCAUTO_ON | RF_RXCONFIG_AGCAUTO_ON | RF_RXCONFIG_RXTRIGER_PREAMBLEDETECT );

            SX1276->Settings->FskPacketHandler->PreambleDetected = false;
            SX1276->Settings->FskPacketHandler->SyncWordDetected = false;
            SX1276->Settings->FskPacketHandler->NbBytes = 0;
            SX1276->Settings->FskPacketHandler->Size = 0;
        }
        break;
    case MODEM_LORA:
        {
            if( SX1276->Settings->LoRa->IqInverted == true )
            {
                writeRegister( SX1276, REG_LR_INVERTIQ, ( ( readRegister( SX1276, REG_LR_INVERTIQ ) & RFLR_INVERTIQ_TX_MASK & RFLR_INVERTIQ_RX_MASK ) | RFLR_INVERTIQ_RX_ON | RFLR_INVERTIQ_TX_OFF ) );
                writeRegister( SX1276, REG_LR_INVERTIQ2, RFLR_INVERTIQ2_ON );
            }
            else
            {
                writeRegister( SX1276, REG_LR_INVERTIQ, ( ( readRegister( SX1276, REG_LR_INVERTIQ ) & RFLR_INVERTIQ_TX_MASK & RFLR_INVERTIQ_RX_MASK ) | RFLR_INVERTIQ_RX_OFF | RFLR_INVERTIQ_TX_OFF ) );
                writeRegister( SX1276, REG_LR_INVERTIQ2, RFLR_INVERTIQ2_OFF );
            }

            // ERRATA 2.3 - Receiver Spurious Reception of a LoRa Signal
            if( SX1276->Settings->LoRa->Bandwidth < 9 )
            {
                writeRegister( SX1276, REG_LR_DETECTOPTIMIZE, readRegister( SX1276, REG_LR_DETECTOPTIMIZE ) & 0x7F );
                writeRegister( SX1276, REG_LR_TEST30, 0x00 );
                switch( SX1276->Settings->LoRa->Bandwidth )
                {
                case 0: // 7.8 kHz
                    writeRegister( SX1276, REG_LR_TEST2F, 0x48 );
                    setFrequency(SX1276, SX1276->Settings->Channel + 7.81e3 );
                    break;
                case 1: // 10.4 kHz
                    writeRegister( SX1276, REG_LR_TEST2F, 0x44 );
                    setFrequency(SX1276, SX1276->Settings->Channel + 10.42e3 );
                    break;
                case 2: // 15.6 kHz
                    writeRegister( SX1276, REG_LR_TEST2F, 0x44 );
                    setFrequency(SX1276, SX1276->Settings->Channel + 15.62e3 );
                    break;
                case 3: // 20.8 kHz
                    writeRegister( SX1276, REG_LR_TEST2F, 0x44 );
                    setFrequency(SX1276, SX1276->Settings->Channel + 20.83e3 );
                    break;
                case 4: // 31.2 kHz
                    writeRegister( SX1276, REG_LR_TEST2F, 0x44 );
                    setFrequency(SX1276, SX1276->Settings->Channel + 31.25e3 );
                    break;
                case 5: // 41.4 kHz
                    writeRegister( SX1276, REG_LR_TEST2F, 0x44 );
                    setFrequency(SX1276, SX1276->Settings->Channel + 41.67e3 );
                    break;
                case 6: // 62.5 kHz
                    writeRegister( SX1276, REG_LR_TEST2F, 0x40 );
                    break;
                case 7: // 125 kHz
                    writeRegister( SX1276, REG_LR_TEST2F, 0x40 );
                    break;
                case 8: // 250 kHz
                    writeRegister( SX1276, REG_LR_TEST2F, 0x40 );
                    break;
                }
            }
            else
            {
                writeRegister( SX1276, REG_LR_DETECTOPTIMIZE, readRegister( SX1276, REG_LR_DETECTOPTIMIZE ) | 0x80 );
            }

            rxContinuous = SX1276->Settings->LoRa->RxContinuous;

            if( SX1276->Settings->LoRa->FreqHopOn == true )
            {
                writeRegister( SX1276, REG_LR_IRQ_FLAGS_MASK, //RFLR_IRQFLAGS_RXTIMEOUT |
                                                  //RFLR_IRQFLAGS_RXDONE |
                                                  //RFLR_IRQFLAGS_PAYLOADCRCERROR |
                                                  RFLR_IRQFLAGS_VALIDHEADER |
                                                  RFLR_IRQFLAGS_TXDONE |
                                                  RFLR_IRQFLAGS_CADDONE |
                                                  //RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL |
                                                  RFLR_IRQFLAGS_CADDETECTED );

                // DIO0=RxDone, DIO2=FhssChangeChannel
                writeRegister( SX1276, REG_DIO_MAPPING_1, ( readRegister( SX1276, REG_DIO_MAPPING_1 ) & RFLR_DIOMAPPING1_DIO0_MASK & RFLR_DIOMAPPING1_DIO2_MASK  ) | RFLR_DIOMAPPING1_DIO0_00 | RFLR_DIOMAPPING1_DIO2_00 );
            }
            else
            {
                writeRegister( SX1276, REG_LR_IRQ_FLAGS_MASK, //RFLR_IRQFLAGS_RXTIMEOUT |
                                                  //RFLR_IRQFLAGS_RXDONE |
                                                  //RFLR_IRQFLAGS_PAYLOADCRCERROR |
                                                  RFLR_IRQFLAGS_VALIDHEADER |
                                                  RFLR_IRQFLAGS_TXDONE |
                                                  RFLR_IRQFLAGS_CADDONE |
                                                  RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL |
                                                  RFLR_IRQFLAGS_CADDETECTED );

                // DIO0=RxDone
                writeRegister( SX1276, REG_DIO_MAPPING_1, ( readRegister( SX1276, REG_DIO_MAPPING_1 ) & RFLR_DIOMAPPING1_DIO0_MASK ) | RFLR_DIOMAPPING1_DIO0_00 );
            }
            // again just like setTx be careful with these
            writeRegister( SX1276, REG_FIFO_RX_BASE_ADDR, 0 );
            writeRegister( SX1276, REG_FIFO_ADDR_PTR, 0 );
        }
        break;
    }

    memset( SX1276->RxTxBuffer, 0, ( size_t )RX_BUFFER_SIZE );

    SX1276->Settings->State = RF_RX_RUNNING;

    if( SX1276->Settings->Modem == MODEM_FSK )
    {
        SX1276SetOpMode( SX1276, MODE_RX_CONTINUOUS );

        if( rxContinuous == false )
        {
/*
            TimerSetValue( &RxTimeoutSyncWord, ceil( ( 8.0 * ( SX1276.Settings.Fsk.PreambleLen +
                                                             ( ( SX1276Read( REG_SYNCCONFIG ) &
                                                                ~RF_SYNCCONFIG_SYNCSIZE_MASK ) +
                                                                1.0 ) + 10.0 ) /
                                                             ( double )SX1276.Settings.Fsk.Datarate ) * 1e3 ) + 4 );
            TimerStart( &RxTimeoutSyncWord );
*/
        }
    }
    else
    {
        if( rxContinuous == true )
        {
            SX1276SetOpMode( SX1276, MODE_RX_CONTINUOUS );
        }
        else
        {
            SX1276SetOpMode( SX1276, MODE_RX_SINGLE );
        }
    }

    // here we listen for the packet
    st = RADIO_IDLE;
    int count = 0;
    int irqFlags;
    int packetLength = 0;
    // clearing the rx done flag
    rx_done = 0;
    while(count < timeout) {
        irqFlags = readRegister(SX1276, REG_IRQ_FLAGS);
        if (irqFlags & RFLR_IRQFLAGS_RXDONE_MASK) {
            printf("interrupt occured: %x\n", irqFlags);
            SX1276OnDio0Irq(SX1276);
            break;
        } else if (irqFlags & RFLR_IRQFLAGS_RXTIMEOUT_MASK) {
            SX1276OnDio1Irq(SX1276);
            break;
        } else if (irqFlags & RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL_MASK) {
            // fhss change channel interrupt if dio2 is set to 00
            SX1276OnDio2Irq(SX1276);
        }
        count++;
        upm_delay_us(1000);
    }

    if(st == RADIO_IDLE) {
        // call the timeout function
        printf("timeout occured STATE: %d\n", SX1276->Settings->State);
        SX1276OnTimeoutIrq(SX1276);
    }

    return st;
}

void SX1276StartCad(sx1276_context SX1276)
{
    switch(SX1276->Settings->Modem)
    {
    case MODEM_FSK:
        {
            // nothing here \O/
        }
        break;
    case MODEM_LORA:
        {
            writeRegister ( SX1276, REG_LR_IRQ_FLAGS_MASK, RFLR_IRQFLAGS_RXTIMEOUT |
                                        RFLR_IRQFLAGS_RXDONE |
                                        RFLR_IRQFLAGS_PAYLOADCRCERROR |
                                        RFLR_IRQFLAGS_VALIDHEADER |
                                        RFLR_IRQFLAGS_TXDONE |
                                        //RFLR_IRQFLAGS_CADDONE |
                                        RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL // |
                                        //RFLR_IRQFLAGS_CADDETECTED
                                        );

            // DIO3=CADDone
            writeRegister( SX1276, REG_DIO_MAPPING_1, ( readRegister( SX1276, REG_DIO_MAPPING_1 ) & RFLR_DIOMAPPING1_DIO3_MASK ) | RFLR_DIOMAPPING1_DIO3_00 );

            SX1276->Settings->State = RF_CAD;
            SX1276SetOpMode(SX1276, MODE_CAD);
        }
        break;
    default:
        break;
    }
}

void SX1276SetTxContinuousWave(sx1276_context SX1276, uint32_t freq, int8_t power, uint16_t time)
{
    uint32_t timeout = ( uint32_t )( time * 1e3 );

    setFrequency(SX1276, freq);

    // power = power
    // fdev = 0
    // bw = 0 | 125KHz
    // datarate = 4800
    // coderate = 0
    // preamble len = 5
    // fixlen = false
    // crcOn = false
    // freqHopOn = 0
    // hopPeriod = 0
    // iqInverted = 0
    // timeout = time*1000
    SX1276SetTxConfig( SX1276, MODEM_FSK, power, 0, 0, 4800, 0, 5, false, false, 0, 0, 0, timeout );

    writeRegister( SX1276, REG_PACKETCONFIG2, ( readRegister( SX1276, REG_PACKETCONFIG2 ) & RF_PACKETCONFIG2_DATAMODE_MASK ) );
    // Disable radio interrupts
    writeRegister(SX1276, REG_DIO_MAPPING_1, RF_DIOMAPPING1_DIO0_11 | RF_DIOMAPPING1_DIO1_11 );
    writeRegister(SX1276, REG_DIO_MAPPING_2, RF_DIOMAPPING2_DIO4_10 | RF_DIOMAPPING2_DIO5_10 );

    //TimerSetValue( &TxTimeoutTimer, timeout );

    SX1276->Settings->State = RF_TX_RUNNING;
    //TimerStart( &TxTimeoutTimer );
    SX1276SetOpMode( SX1276, MODE_TX );

    int count = 0;
    while(count < timeout) {
        // implement interrupt handling here
        count++;
        upm_delay_us(1000);
    }
}

void SX1276SetMaxPayloadLength(sx1276_context SX1276, RadioModems_t modem, uint8_t max)
{
    SX1276SetModem(SX1276, modem);

    switch(modem)
    {
    case MODEM_FSK:
        if( SX1276->Settings->Fsk->FixLen == false )
        {
            writeRegister(SX1276, REG_PAYLOADLENGTH, max);
        }
        break;
    case MODEM_LORA:
        writeRegister( SX1276, REG_PAYLOAD_LENGTH, max );
        break;
    }
}

void SX1276SetPublicNetwork(sx1276_context SX1276, bool enable)
{
    SX1276SetModem(SX1276, MODEM_LORA);
    if(enable == true)
    {
        // Change LoRa modem SyncWord
        writeRegister(SX1276, REG_LR_SYNCWORD, LORA_MAC_PUBLIC_SYNCWORD);
    }
    else
    {
        // Change LoRa modem SyncWord
        writeRegister( SX1276, REG_LR_SYNCWORD, LORA_MAC_PRIVATE_SYNCWORD );
    }
}

// this function will be called everytime timeout occurs
void SX1276OnTimeoutIrq(sx1276_context SX1276)
{
    switch(SX1276->Settings->State)
    {
    case RF_RX_RUNNING:
        if(SX1276->Settings->Modem == MODEM_FSK)
        {
            SX1276->Settings->FskPacketHandler->PreambleDetected = false;
            SX1276->Settings->FskPacketHandler->SyncWordDetected = false;
            SX1276->Settings->FskPacketHandler->NbBytes = 0;
            SX1276->Settings->FskPacketHandler->Size = 0;

            // Clear Irqs
            writeRegister( SX1276, REG_IRQFLAGS1, RF_IRQFLAGS1_RSSI |
                                        RF_IRQFLAGS1_PREAMBLEDETECT |
                                        RF_IRQFLAGS1_SYNCADDRESSMATCH );
            writeRegister( SX1276, REG_IRQFLAGS2, RF_IRQFLAGS2_FIFOOVERRUN );

            if( SX1276->Settings->Fsk->RxContinuous == true )
            {
                // Continuous mode restart Rx chain
                writeRegister( SX1276, REG_RXCONFIG, readRegister( SX1276, REG_RXCONFIG ) | RF_RXCONFIG_RESTARTRXWITHOUTPLLLOCK );
                //TimerStart( &RxTimeoutSyncWord );
            }
            else
            {
                SX1276->Settings->State = RF_IDLE;
                //TimerStop( &RxTimeoutSyncWord );
            }
        }
// ** CALLBACKS HERE ** WILL HAVE TO BE IMPLMENTED WITH THE MAC LAYER **
        st = RX_TIMEOUT;
        break;
    case RF_TX_RUNNING:
        SX1276->Settings->State = RF_IDLE;
// ** CALLBACKS HERE ** WILL HAVE TO BE IMPLMENTED WITH THE MAC LAYER **
        st = TX_TIMEOUT;
        break;
    default:
        break;
    }
}

void SX1276OnDio0Irq(sx1276_context SX1276)
{
    volatile uint8_t irqFlags = 0;

    switch(SX1276->Settings->State)
    {
        case RF_RX_RUNNING:
            switch(SX1276->Settings->Modem)
            {
            case MODEM_FSK:
                if( SX1276->Settings->Fsk->CrcOn == true )
                {
                    irqFlags = readRegister( SX1276, REG_IRQFLAGS2 );
                    if( ( irqFlags & RF_IRQFLAGS2_CRCOK ) != RF_IRQFLAGS2_CRCOK )
                    {
                        // Clear Irqs
                        writeRegister( SX1276, REG_IRQFLAGS1, RF_IRQFLAGS1_RSSI |
                                                    RF_IRQFLAGS1_PREAMBLEDETECT |
                                                    RF_IRQFLAGS1_SYNCADDRESSMATCH );
                        writeRegister( SX1276, REG_IRQFLAGS2, RF_IRQFLAGS2_FIFOOVERRUN );


                        if( SX1276->Settings->Fsk->RxContinuous == false )
                        {
                            SX1276->Settings->State = RF_IDLE;
                        }
                        else
                        {
                            // Continuous mode restart Rx chain
                            writeRegister( SX1276, REG_RXCONFIG, readRegister( SX1276, REG_RXCONFIG ) | RF_RXCONFIG_RESTARTRXWITHOUTPLLLOCK );
                        }
                        printf("FSK: RX Error has occured\n");
                        // ** CALLBACKS HERE ** TO BE DETERMINED **
                        st = RX_ERROR;
                        SX1276->Settings->FskPacketHandler->PreambleDetected = false;
                        SX1276->Settings->FskPacketHandler->SyncWordDetected = false;
                        SX1276->Settings->FskPacketHandler->NbBytes = 0;
                        SX1276->Settings->FskPacketHandler->Size = 0;
                        break;
                    }
                }


                // Read received packet size
                if( ( SX1276->Settings->FskPacketHandler->Size == 0 ) && ( SX1276->Settings->FskPacketHandler->NbBytes == 0 ) )
                {
                    if( SX1276->Settings->Fsk->FixLen == false )
                    {
                        SX1276->Settings->FskPacketHandler->Size = readRegister(SX1276, REG_FIFO);
                    }
                    else
                    {
                        SX1276->Settings->FskPacketHandler->Size = readRegister( SX1276, REG_PAYLOADLENGTH );
                    }

                    SX1276->_packetIndex = 0;
                    if(SX1276->Settings->FskPacketHandler->Size - SX1276->Settings->FskPacketHandler->NbBytes) {
                        while((SX1276->Settings->FskPacketHandler->Size - SX1276->Settings->FskPacketHandler->NbBytes) - SX1276->_packetIndex) {
                        	SX1276->RxTxBuffer[SX1276->Settings->FskPacketHandler->NbBytes + SX1276->_packetIndex] = SX1276read(SX1276);
                        }
                    }
                    SX1276->Settings->FskPacketHandler->NbBytes += ( SX1276->Settings->FskPacketHandler->Size - SX1276->Settings->FskPacketHandler->NbBytes );
                }
                else
                {
                    SX1276->_packetIndex = 0;
                    if(SX1276->Settings->FskPacketHandler->Size - SX1276->Settings->FskPacketHandler->NbBytes) {
                        while((SX1276->Settings->FskPacketHandler->Size - SX1276->Settings->FskPacketHandler->NbBytes) - SX1276->_packetIndex) {
                        	SX1276->RxTxBuffer[SX1276->Settings->FskPacketHandler->NbBytes + SX1276->_packetIndex] = SX1276read(SX1276);
                        }
                    }
                    SX1276->Settings->FskPacketHandler->NbBytes += ( SX1276->Settings->FskPacketHandler->Size - SX1276->Settings->FskPacketHandler->NbBytes );
                }

                if( SX1276->Settings->Fsk->RxContinuous == false )
                {
                    SX1276->Settings->State = RF_IDLE;
                }
                else
                {
                    // Continuous mode restart Rx chain
                    writeRegister( SX1276, REG_RXCONFIG, readRegister( SX1276, REG_RXCONFIG ) | RF_RXCONFIG_RESTARTRXWITHOUTPLLLOCK );
                }
                printf("FSK: RX Done\n");

                st = RX_DONE;
                SX1276->Settings->FskPacketHandler->PreambleDetected = false;
                SX1276->Settings->FskPacketHandler->SyncWordDetected = false;
                SX1276->Settings->FskPacketHandler->NbBytes = 0;
                SX1276->Settings->FskPacketHandler->Size = 0;
                break;
            case MODEM_LORA:
                {
                    int8_t snr = 0;

                    // Clear Irq
                    writeRegister( SX1276, REG_IRQ_FLAGS, RFLR_IRQFLAGS_RXDONE );

                    irqFlags = readRegister( SX1276, REG_IRQ_FLAGS );
                    if( ( irqFlags & RFLR_IRQFLAGS_PAYLOADCRCERROR_MASK ) == RFLR_IRQFLAGS_PAYLOADCRCERROR )
                    {
                        // Clear Irq
                        writeRegister( SX1276, REG_IRQ_FLAGS, RFLR_IRQFLAGS_PAYLOADCRCERROR );

                        if( SX1276->Settings->LoRa->RxContinuous == false )
                        {
                            SX1276->Settings->State = RF_IDLE;
                        }
                        printf("LORA: RX Error has occured\n");

                        st = RX_ERROR;
                        break;
                    }

                    SX1276->Settings->LoRaPacketHandler->SnrValue = readRegister( SX1276, REG_LR_PKTSNRVALUE );
                    if( SX1276->Settings->LoRaPacketHandler->SnrValue & 0x80 ) // The SNR sign bit is 1
                    {
                        // Invert and divide by 4
                        snr = ( ( ~SX1276->Settings->LoRaPacketHandler->SnrValue + 1 ) & 0xFF ) >> 2;
                        snr = -snr;
                    }
                    else
                    {
                        // Divide by 4
                        snr = ( SX1276->Settings->LoRaPacketHandler->SnrValue & 0xFF ) >> 2;
                    }

                    int16_t rssi = readRegister( SX1276, REG_PKT_RSSI_VALUE );
                    if( snr < 0 )
                    {
                        if( SX1276->Settings->Channel > RF_MID_BAND_THRESH )
                        {
                            SX1276->Settings->LoRaPacketHandler->RssiValue = RSSI_OFFSET_HF + rssi + ( rssi >> 4 ) +
                                                                          snr;
                        }
                        else
                        {
                            SX1276->Settings->LoRaPacketHandler->RssiValue = RSSI_OFFSET_LF + rssi + ( rssi >> 4 ) +
                                                                          snr;
                        }
                    }
                    else
                    {
                        if( SX1276->Settings->Channel > RF_MID_BAND_THRESH )
                        {
                            SX1276->Settings->LoRaPacketHandler->RssiValue = RSSI_OFFSET_HF + rssi + ( rssi >> 4 );
                        }
                        else
                        {
                            SX1276->Settings->LoRaPacketHandler->RssiValue = RSSI_OFFSET_LF + rssi + ( rssi >> 4 );
                        }
                    }

                    if (SX1276->Settings->LoRa->FixLen) {
                        SX1276->Settings->LoRaPacketHandler->Size = readRegister(SX1276, REG_PAYLOAD_LENGTH);
                    } else {
                        SX1276->Settings->LoRaPacketHandler->Size = readRegister(SX1276, REG_RX_NB_BYTES);
                    }
                    printf("packet size retrieved: %d\n", SX1276->Settings->LoRaPacketHandler->Size);

                    writeRegister(SX1276, REG_FIFO_ADDR_PTR, readRegister(SX1276, REG_FIFO_RX_CURRENT_ADDR));

                    SX1276->_packetIndex = 0;
                    if(SX1276->Settings->LoRaPacketHandler->Size) {
                        while(SX1276->Settings->LoRaPacketHandler->Size - SX1276->_packetIndex) {
                        	SX1276->RxTxBuffer[SX1276->_packetIndex] = SX1276read(SX1276);
                        }
                    }
#if 0
                    // this is just for debug
                    int c = 0;
                    printf("Packet: ");
                    for(c=0; c<SX1276->Settings->LoRaPacketHandler->Size; c++)
                        printf("%c", (char)SX1276->RxTxBuffer[c]);
                        //printf("%x, ", RxTxBuffer[c]);
                    printf("\n");
#endif

                    if( SX1276->Settings->LoRa->RxContinuous == false )
                    {
                        SX1276->Settings->State = RF_IDLE;
                    }
                    printf("LORA: RX Done\n");
                    rx_done = 1;
                    st = RX_DONE;
                }
                break;
            default:
                break;
            }
            break;
        case RF_TX_RUNNING:
            // TxDone interrupt
            switch( SX1276->Settings->Modem )
            {
            case MODEM_LORA:
                // Clear Irq
                writeRegister( SX1276, REG_IRQ_FLAGS, RFLR_IRQFLAGS_TXDONE );
                // Intentional fall through
            case MODEM_FSK:
            default:
                SX1276->Settings->State = RF_IDLE;
                printf("LORA & FSK: TX Done\n");
                st = TX_DONE;
                break;
            }
            break;
        default:
            break;
    }
}

void SX1276OnDio1Irq(sx1276_context SX1276)
{
    switch( SX1276->Settings->State )
    {
        case RF_RX_RUNNING:
            switch(SX1276->Settings->Modem)
            {
            case MODEM_FSK:
                // FifoLevel interrupt
                // Read received packet size
                if( ( SX1276->Settings->FskPacketHandler->Size == 0 ) && ( SX1276->Settings->FskPacketHandler->NbBytes == 0 ) )
                {
                    if( SX1276->Settings->Fsk->FixLen == false )
                    {
                        SX1276->Settings->FskPacketHandler->Size = readRegister(SX1276, REG_FIFO);
                    }
                    else
                    {
                        SX1276->Settings->FskPacketHandler->Size = readRegister( SX1276, REG_PAYLOADLENGTH );
                    }
                }

                if( ( SX1276->Settings->FskPacketHandler->Size - SX1276->Settings->FskPacketHandler->NbBytes ) > SX1276->Settings->FskPacketHandler->FifoThresh )
                {
                    // add fifo related code here carefully
                    SX1276->_packetIndex = 0;
                    if(SX1276->Settings->FskPacketHandler->FifoThresh) {
                        while(SX1276->Settings->FskPacketHandler->FifoThresh - SX1276->_packetIndex) {
                        	SX1276->RxTxBuffer[SX1276->Settings->FskPacketHandler->NbBytes + SX1276->_packetIndex] = SX1276read(SX1276);
                        }
                    }
                    SX1276->Settings->FskPacketHandler->NbBytes += SX1276->Settings->FskPacketHandler->FifoThresh;
                }
                else
                {
                    // fifo
                    SX1276->_packetIndex = 0;
                    if(SX1276->Settings->FskPacketHandler->Size - SX1276->Settings->FskPacketHandler->NbBytes) {
                        while((SX1276->Settings->FskPacketHandler->Size - SX1276->Settings->FskPacketHandler->NbBytes) - SX1276->_packetIndex) {
                            SX1276->RxTxBuffer[SX1276->Settings->FskPacketHandler->NbBytes + SX1276->_packetIndex] = SX1276read(SX1276);
                        }
                    }
                    SX1276->Settings->FskPacketHandler->NbBytes += ( SX1276->Settings->FskPacketHandler->Size - SX1276->Settings->FskPacketHandler->NbBytes );
                }
                break;
            case MODEM_LORA:
                // Sync time out
                // Clear Irq
                writeRegister( SX1276, REG_IRQ_FLAGS, RFLR_IRQFLAGS_RXTIMEOUT );

                SX1276->Settings->State = RF_IDLE;
                printf("FIFO: DIO1: RX Timeout Occured\n");
                st = RX_TIMEOUT;
                break;
            default:
                break;
            }
            break;
        case RF_TX_RUNNING:
            switch( SX1276->Settings->Modem )
            {
            case MODEM_FSK:
                // FifoEmpty interrupt
                if( ( SX1276->Settings->FskPacketHandler->Size - SX1276->Settings->FskPacketHandler->NbBytes ) > SX1276->Settings->FskPacketHandler->ChunkSize )
                {
                    // write Buf here
                	write_buf(SX1276, ( SX1276->RxTxBuffer + SX1276->Settings->FskPacketHandler->NbBytes ), SX1276->Settings->FskPacketHandler->ChunkSize);
                    SX1276->Settings->FskPacketHandler->NbBytes += SX1276->Settings->FskPacketHandler->ChunkSize;
                }
                else
                {
                    // Write the last chunk of data
                	write_buf(SX1276, SX1276->RxTxBuffer + SX1276->Settings->FskPacketHandler->NbBytes, SX1276->Settings->FskPacketHandler->Size - SX1276->Settings->FskPacketHandler->NbBytes);
                    SX1276->Settings->FskPacketHandler->NbBytes += SX1276->Settings->FskPacketHandler->Size - SX1276->Settings->FskPacketHandler->NbBytes;
                }
                break;
            case MODEM_LORA:
                break;
            default:
                break;
            }
            break;
        default:
            break;
    }
}

void SX1276OnDio2Irq(sx1276_context SX1276)
{
    switch(SX1276->Settings->State)
    {
        case RF_RX_RUNNING:
            switch(SX1276->Settings->Modem)
            {
            case MODEM_FSK:
                    SX1276->Settings->FskPacketHandler->PreambleDetected = true;

                if( ( SX1276->Settings->FskPacketHandler->PreambleDetected == true ) && ( SX1276->Settings->FskPacketHandler->SyncWordDetected == false ) )
                {
                    SX1276->Settings->FskPacketHandler->SyncWordDetected = true;

                    SX1276->Settings->FskPacketHandler->RssiValue = -( readRegister( SX1276, REG_RSSIVALUE ) >> 1 );

                    SX1276->Settings->FskPacketHandler->AfcValue = ( int32_t )( double )( ( ( uint16_t )readRegister( SX1276, REG_AFCMSB ) << 8 ) |
                                                                           ( uint16_t )readRegister( SX1276, REG_AFCLSB ) ) *
                                                                           ( double )FREQ_STEP;
                    SX1276->Settings->FskPacketHandler->RxGain = ( readRegister( SX1276, REG_LNA ) >> 5 ) & 0x07;
                }
                break;
            case MODEM_LORA:
                if( SX1276->Settings->LoRa->FreqHopOn == true )
                {
                    // Clear Irq
                    writeRegister( SX1276, REG_IRQ_FLAGS, RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL );

                    // ** NOT IMPLEMENTED YET ** TBD **
                    st = FHSS_CHANGE_CHANNEL;
                }
                break;
            default:
                break;
            }
            break;
        case RF_TX_RUNNING:
            switch(SX1276->Settings->Modem)
            {
            case MODEM_FSK:
                break;
            case MODEM_LORA:
                if(SX1276->Settings->LoRa->FreqHopOn == true)
                {
                    // Clear Irq
                    writeRegister( SX1276, REG_IRQ_FLAGS, RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL );
                    // ** NOT IMPLEMENTED YET ** TBD **
                    st = FHSS_CHANGE_CHANNEL;
                }
                break;
            default:
                break;
            }
            break;
        default:
            break;
    }
}

void SX1276OnDio3Irq(sx1276_context SX1276)
{
    switch(SX1276->Settings->Modem)
    {
    case MODEM_FSK:
        break;
    case MODEM_LORA:
        if( ( readRegister( SX1276, REG_IRQ_FLAGS ) & RFLR_IRQFLAGS_CADDETECTED ) == RFLR_IRQFLAGS_CADDETECTED )
        {
            // Clear Irq
            writeRegister( SX1276, REG_IRQ_FLAGS, RFLR_IRQFLAGS_CADDETECTED | RFLR_IRQFLAGS_CADDONE );

            // ** NOT IMPLEMENTED YET **
            st = CAD_DONE;
        }
        else
        {
            // Clear Irq
            writeRegister( SX1276, REG_IRQ_FLAGS, RFLR_IRQFLAGS_CADDONE );
            // ** NOT IMPLEMENTED YET **
            st = CAD_DONE;
        }
        break;
    default:
        break;
    }
}

void SX1276OnDio4Irq(sx1276_context SX1276)
{
    switch( SX1276->Settings->Modem )
    {
    case MODEM_FSK:
        {
            if( SX1276->Settings->FskPacketHandler->PreambleDetected == false )
            {
                SX1276->Settings->FskPacketHandler->PreambleDetected = true;
            }
        }
        break;
    case MODEM_LORA:
        break;
    default:
        break;
    }
}

uint8_t SX1276GetRxBufferlen(sx1276_context SX1276) {
    return SX1276->Settings->LoRaPacketHandler->Size;
}

void SX1276GetRxBuffer(sx1276_context SX1276, uint8_t* buf) {
    int c = 0;
    for (c = 0; c < SX1276->Settings->LoRaPacketHandler->Size; c++)
    	buf[c] = SX1276->RxTxBuffer[c];
}
