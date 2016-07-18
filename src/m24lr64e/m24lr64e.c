#include "m24lr64e.h"

upm_result_t upm_m24lr64e_eeprom_read_byte(void* dev, uint32_t address, uint8_t* data);
upm_result_t upm_m24lr64e_eeprom_read_bytes(void* dev, uint32_t address, uint8_t* data, int len);
upm_result_t upm_m24lr64e_eeprom_write_byte(void* dev, uint32_t address, uint8_t data);
upm_result_t upm_m24lr64e_eeprom_write_bytes(void* dev, uint32_t address, uint8_t* data, int len);

struct _upm_m24lr64e {
    mraa_i2c_context            i2c;
    uint8_t                     bus;
    uint8_t                     address;
    upm_m24lr64e_access_mode    mode;
};

#if defined(CONFIG_BOARD_ARDUINO_101) || defined(CONFIG_BOARD_ARDUINO_101_SSS) || defined(CONFIG_BOARD_QUARK_D2000_CRB)
DEFINE_MEM_MAP(UPM_M24LR64E_MAP, 1, sizeof(struct _upm_m24lr64e));
const kmemory_map_t UPM_M24LR64E_MEM_MAP;
#elif defined(linux)
#define UPM_M24LR64E_MEM_MAP 0
#endif

const char upm_m24lr64e_name[] = "M24LR64E";
const char upm_m24lr64e_description[] = "Grove NFC Tag";
const upm_protocol_t upm_m24lr64e_protocol[] = {UPM_I2C};
const upm_sensor_t upm_m24lr64e_category[] = {UPM_NFC};

const upm_sensor_descriptor_t upm_m24lr64e_get_descriptor(){
    upm_sensor_descriptor_t usd;
    usd.name = upm_m24lr64e_name;
    usd.description = upm_m24lr64e_description;
    usd.protocol_size = 1;
    usd.protocol = upm_m24lr64e_protocol;
    usd.category_size = 1;
    usd.category = upm_m24lr64e_category;
    return usd;
}

static const upm_sensor_ft ft =
{
    .upm_sensor_init_name = &upm_m24lr64e_init_name,
    .upm_sensor_close = &upm_m24lr64e_close,
    .upm_sensor_get_descriptor = &upm_m24lr64e_get_descriptor
};

#if defined(FRAMEWORK_BUILD)
typedef const void* (*upm_get_ft) (upm_sensor_t sensor_type);

upm_get_ft upm_assign_ft(){
    return upm_m24lr64e_get_ft;
}
#endif

const void* upm_m24lr64e_get_ft(upm_sensor_t sensor_type){
    if(sensor_type == UPM_SENSOR){
        return &ft;
    }
    return NULL;
}

void* upm_m24lr64e_init_name(){
    return NULL;
}

void* upm_m24lr64e_init(int bus, upm_m24lr64e_access_mode mode){
    upm_m24lr64e dev = (upm_m24lr64e) malloc(sizeof(struct _upm_m24lr64e));

    dev->bus = bus;
    dev->mode = mode;
    if(dev->mode == UPM_M24LR64E_USER_MODE)
        dev->address = UPM_M24LR64E_DEFAULT_I2C_ADDR;
    else
        dev->address = UPM_M24LR64E_DEFAULT_I2C_ADDR_E2;

    dev->i2c = mraa_i2c_init(dev->bus);
    if(mraa_i2c_address(dev->i2c, dev->address) != MRAA_SUCCESS){
        return NULL;
    }

    return dev;
}

void upm_m24lr64e_close(void* dev){
    upm_m24lr64e device = (upm_m24lr64e) dev;
    mraa_i2c_stop(device->i2c);
    upm_free(UPM_M24LR64E_MEM_MAP, dev);
}

upm_result_t upm_m24lr64e_submit_password(void* dev, uint32_t password){
    upm_m24lr64e device = (upm_m24lr64e) dev;
    // this device actually uses two bytes to address a register
    const int pktLen = 11;
    uint8_t buf[pktLen];

    buf[0] = 0x09;
    buf[1] = 0x00;

    buf[2] = ((password >> 24) & 0xff);
    buf[3] = ((password >> 16) & 0xff);
    buf[4] = ((password >> 8) & 0xff);
    buf[5] = (password & 0xff);

    buf[6] = 0x09;

    // the password is written twice
    buf[7] = ((password >> 24) & 0xff);
    buf[8] = ((password >> 16) & 0xff);
    buf[9] = ((password >> 8) & 0xff);
    buf[10] = (password & 0xff);

    if(mraa_i2c_write(device->i2c, buf, pktLen) != MRAA_SUCCESS){
        return UPM_ERROR_OPERATION_FAILED;
    }
    return UPM_SUCCESS;
}

upm_result_t upm_m24lr64e_write_password(void* dev, uint32_t password){
    upm_m24lr64e device = (upm_m24lr64e) dev;
    const int pktLen = 11;
    uint8_t buf[pktLen];

    buf[0] = 0x09;
    buf[1] = 0x00;

    buf[2] = ((password >> 24) & 0xff);
    buf[3] = ((password >> 16) & 0xff);
    buf[4] = ((password >> 8) & 0xff);
    buf[5] = (password & 0xff);

    buf[6] = 0x07;

    // the password is written twice
    buf[7] = ((password >> 24) & 0xff);
    buf[8] = ((password >> 16) & 0xff);
    buf[9] = ((password >> 8) & 0xff);
    buf[10] = (password & 0xff);

    if(mraa_i2c_write(device->i2c, buf, pktLen) != MRAA_SUCCESS){
        return UPM_ERROR_OPERATION_FAILED;
    }
    return UPM_SUCCESS;
}

upm_result_t upm_m24lr64e_sector_protect_config(void* dev,
        uint32_t sector_number,
        bool protect_enable,
        sector_access_right access_right,
        sector_select_password password){
    upm_m24lr64e device = (upm_m24lr64e) dev;
    if(!protect_enable){
        upm_m24lr64e_write_byte(device, sector_number, 0x0);
    }
    else{
        upm_m24lr64e_write_byte(device, sector_number, (protect_enable | (access_right << 1) | (password << 2)));
    }
    return UPM_SUCCESS;
}

upm_result_t upm_m24lr64e_clear_sector_protect(void* dev){
    upm_m24lr64e device = (upm_m24lr64e) dev;
    uint8_t buf[64]={0x0};
    return upm_m24lr64e_eeprom_write_bytes(device, 0, buf, 64);
}

upm_result_t upm_m24lr64e_sector_write_lock_bit(void* dev,
        uint32_t sector_number,
        bool sock_enable){
    upm_m24lr64e device = (upm_m24lr64e) dev;
    uint32_t sector_address = UPM_M24LR64E_SECTOR_SECURITY_STATUS_BASE_ADDR
        + (sector_number/8);

    uint8_t sector_bit = sector_number % 8;
    // byte read from the EEPROM into pre_status
    uint8_t pre_status;
    upm_m24lr64e_eeprom_read_byte(device, sector_address, &pre_status);

    bool status = (pre_status >> sector_bit) & 0x01;
    if(status != sock_enable){
        if(status == true){
            // Call to write byte function
            upm_m24lr64e_write_byte(device, sector_address, pre_status&(~(1<<sector_bit)));
        }
        else{
            // another call to write byte function
            upm_m24lr64e_write_byte(device, sector_address, pre_status|(1<<sector_bit));
        }
    }
    return UPM_SUCCESS;
}

upm_result_t upm_m24lr64e_get_dsfid(void* dev, uint8_t* dsfid){
    upm_m24lr64e device = (upm_m24lr64e) dev;
    // EEPROM read byte function call
    return upm_m24lr64e_eeprom_read_byte(device, UPM_M24LR64E_DSFID_ADDRESS, dsfid);
}

upm_result_t upm_m24lr64e_get_afi(void* dev, uint8_t* afi){
    upm_m24lr64e device = (upm_m24lr64e) dev;
    // call to EEPROM read byte
    return upm_m24lr64e_eeprom_read_byte(device, UPM_M24LR64E_AFI_ADDRESS, afi);
}

upm_result_t upm_m24lr64e_get_uid(void* dev, uint8_t* uid){
    upm_m24lr64e device = (upm_m24lr64e) dev;
    uint8_t* buffer;
    uint8_t arr[UPM_M24LR64E_UID_LENGTH];
    buffer = arr;
    // call to EEPROM read bytes
    upm_m24lr64e_eeprom_read_bytes(device, UPM_M24LR64E_UID_ADDRESS, buffer, UPM_M24LR64E_UID_LENGTH);
    // not so sure about this
    *uid = *buffer;
    return UPM_SUCCESS;
}

upm_result_t upm_m24lr64e_get_memory_size(void* dev, uint32_t* memory_size){
    upm_m24lr64e device = (upm_m24lr64e) dev;
    uint32_t volume = 0x0;
    uint8_t temp_vol = 0x0;
    // us the EEPROM read byte function to calculate the volume
    if(upm_m24lr64e_eeprom_read_byte(device, UPM_M24LR64E_MEM_SIZE_ADDRESS, &temp_vol) != UPM_SUCCESS){
        return UPM_ERROR_OPERATION_FAILED;
    }
    volume = temp_vol;
    if(upm_m24lr64e_eeprom_read_byte(device, UPM_M24LR64E_MEM_SIZE_ADDRESS+1, &temp_vol) != UPM_SUCCESS){
        return UPM_ERROR_OPERATION_FAILED;
    }
    volume = volume<<8|temp_vol;
    temp_vol = 0x0;
    if(upm_m24lr64e_eeprom_read_byte(device, UPM_M24LR64E_MEM_SIZE_ADDRESS+2, &temp_vol) != UPM_SUCCESS){
        return UPM_ERROR_OPERATION_FAILED;
    }
    volume = volume<<8|temp_vol;
    *memory_size = volume;
    return UPM_SUCCESS;
}

upm_result_t upm_m24lr64e_clear_memory(void* dev){
    upm_m24lr64e device = (upm_m24lr64e) dev;
    int i=0;
    for(i = 0; i < UPM_M24LR64E_EEPROM_I2C_LENGTH; i++){
        // call to write byte function
        upm_m24lr64e_write_byte(device, i, 0x0);
    }
    return UPM_SUCCESS;
}

upm_result_t upm_m24lr64e_write_byte(void* dev, uint32_t address, uint8_t data){
    upm_m24lr64e device = (upm_m24lr64e) dev;
    // call to EEPROM write byte
    return upm_m24lr64e_eeprom_write_byte(device, address, data);
}

upm_result_t upm_m24lr64e_write_bytes(void* dev, uint32_t address, uint8_t* buffer, int len){
    upm_m24lr64e device = (upm_m24lr64e) dev;
    // call to EEPROM write bytes
    return upm_m24lr64e_eeprom_write_bytes(device, address, buffer, len);
}

upm_result_t upm_m24lr64e_read_byte(void* dev, uint32_t address, uint8_t* data){
    upm_m24lr64e device = (upm_m24lr64e) dev;
    // call to EEPROM read byte
    return upm_m24lr64e_eeprom_read_byte(device, address, data);
}

upm_result_t upm_m24lr64e_read_bytes(void* dev, uint32_t address, uint8_t* buffer, int len){
    upm_m24lr64e device = (upm_m24lr64e) dev;
    // call to EEPROM read bytes
    return upm_m24lr64e_eeprom_write_bytes(device, address, buffer, len);
}

upm_result_t upm_m24lr64e_eeprom_write_byte(void* dev, uint32_t address, uint8_t data){
    upm_m24lr64e device = (upm_m24lr64e) dev;
    int pkt_len = 3;
    uint8_t buf[pkt_len];
    buf[0] = ((address >> 8) & 0xff);
    buf[1] = (address & 0xff);
    buf[2] = data;

    if(mraa_i2c_write(device->i2c, buf, pkt_len) != MRAA_SUCCESS){
        return UPM_ERROR_OPERATION_FAILED;
    }

    upm_delay_us(UPM_M24LR64E_I2C_WRITE_TIME*1000);
    return UPM_SUCCESS;
}

upm_result_t upm_m24lr64e_eeprom_write_bytes(void* dev, uint32_t address, uint8_t* data, int len){
    upm_m24lr64e device = (upm_m24lr64e) dev;
    uint32_t pkt_len = 2 + len;
    uint8_t buf[pkt_len];

    buf[0] = ((address >> 8) & 0xff);
    buf[1] = (address & 0xff);

    int i = 0;
    for (i=0; i<len; i++)
        buf[2+i] = data[i];

    if(mraa_i2c_write(device->i2c, buf, pkt_len) != MRAA_SUCCESS){
        return UPM_ERROR_OPERATION_FAILED;
    }
    upm_delay_us(UPM_M24LR64E_I2C_WRITE_TIME*1000);
    return UPM_SUCCESS;
}

upm_result_t upm_m24lr64e_eeprom_read_byte(void* dev, uint32_t address, uint8_t* data){
    upm_m24lr64e device = (upm_m24lr64e) dev;
    uint32_t pkt_len = 2;
    uint8_t buf[pkt_len];

    buf[0] = ((address >> 8) & 0xff);
    buf[1] = (address & 0xff);

    if(mraa_i2c_write(device->i2c, buf, pkt_len) != MRAA_SUCCESS){
        return UPM_ERROR_OPERATION_FAILED;
    }

    pkt_len = 1;
    uint8_t abuf[pkt_len];
    abuf[0] = 0;

    if(mraa_i2c_read(device->i2c, abuf, pkt_len) != pkt_len){
        return UPM_ERROR_OPERATION_FAILED;
    }
    *data = abuf[0];
    return UPM_SUCCESS;
}

upm_result_t upm_m24lr64e_eeprom_read_bytes(void* dev, uint32_t address, uint8_t* data, int len){
    upm_m24lr64e device = (upm_m24lr64e) dev;
    uint32_t pkt_len = 2;
    uint8_t buf[pkt_len];

    buf[0] = ((address >> 8) & 0xff);
    buf[1] = (address & 0xff);

    if(mraa_i2c_write(device->i2c, buf, pkt_len) != MRAA_SUCCESS){
        return UPM_ERROR_OPERATION_FAILED;
    }

    if(mraa_i2c_read(device->i2c, data, len) != len){
        return UPM_ERROR_OPERATION_FAILED;
    }

    return UPM_SUCCESS;
}
