/*
 * Author: Jon Trulson <jtrulson@ics.com>
 *         Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 *
 * This code was adapted from the Seeed Studio code at:
 * https://github.com/Seeed-Studio/NFC_Tag_M24LR6E
 *
 * Copyright (c) 2014 seeed technology inc.
 * Website    : www.seeed.cc
 * Author     : lawliet zou
 * Create Time: March 2014
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef M24LR64E_M24LR64E_H_
#define M24LR64E_M24LR64E_H_
#include "upm.h"
#include "mraa/i2c.h"

#define UPM_M24LR64E_I2C_BUS 0
#define UPM_M24LR64E_DEFAULT_I2C_ADDR 0x53
#define UPM_M24LR64E_DEFAULT_I2C_ADDR_E2 (UPM_M24LR64E_DEFAULT_I2C_ADDR | 0x04)

#define UPM_M24LR64E_EEPROM_I2C_LENGTH 8192
#define UPM_M24LR64E_PASSWORD_LENGTH 4
#define UPM_M24LR64E_SECTOR_SECURITY_STATUS_BASE_ADDR 0x800
#define UPM_M24LR64E_LOCK_PROTECT_BIT 0x01
#define UPM_M24LR64E_WRITE_READ_PROTECT_BIT 0x02
#define UPM_M24LR64E_PASSWORD_CTRL_BIT 0x04

#define UPM_M24LR64E_UID_LENGTH 8
#define UPM_M24LR64E_I2C_WRITE_TIME 5

/**
 * @brief Grove NFC Tag
 * @defgroup m24lr64e libupm-m24lr64e
 * @ingroup seeed i2c other
 */

/**
 * @library m24lr64e
 * @sensor m24lr64e
 * @comname Grove NFC Tag
 * @type other
 * @man seeed
 * @web http://www.seeedstudio.com/wiki/Grove_-_NFC_Tag
 * @con i2c
 *
 * @brief C++ API for the M24LR64E-based Grove NFC Tag
 *
 * Grove NFC tag is an 8KB electrically erasable programmable read-only memory (EEPROM)
 * that can be written to or read from using I2C and NFC-equipped devices.
 *
 * The user mode (default) allows read and write access to all 8KB
 * of space, provided the sector security status (SSS) allows it.
 * The root mode allows modification of the SSS data and other
 * information, provided the proper password is submitted.  The
 * default password for a new tag is 0x00000000. See the datasheet
 * for more details.
 *
 * The Seeed Studio* wiki page for this device includes a link to an
 * Android* application that can be used to also read and write the
 * device via NFC, as well as set NFC passwords, which cannot be
 * done via I2C.
 *
 * @image html m24lr64e.jpg
 * @snippet m24lr64e.cxx Interesting
 */

typedef enum {
    UPM_M24LR64E_I2C_PASSWORD_ADDRESS                  = 2304,
    UPM_M24LR64E_RF_PASSWORD_1_ADDRESS                 = 2308, // RF pwds not available in
    UPM_M24LR64E_RF_PASSWORD_2_ADDRESS                 = 2312, // I2C access modes
    UPM_M24LR64E_RF_PASSWORD_3_ADDRESS                 = 2316,
    UPM_M24LR64E_DSFID_ADDRESS                         = 2320, // 1 byte
    UPM_M24LR64E_AFI_ADDRESS                           = 2321, // 1 byte
    UPM_M24LR64E_RESV_ADDRESS                          = 2322, // 1 bytes
    UPM_M24LR64E_CONFIG_ADDRESS                        = 2323, // 1 bytes
    UPM_M24LR64E_UID_ADDRESS                           = 2324, // 8 bytes
    UPM_M24LR64E_MEM_SIZE_ADDRESS                      = 2332, // 3 bytes
    UPM_M24LR64E_IC_REF_ADDRESS                        = 2335, // 1 byte
    UPM_M24LR64E_PROG_COMP_ENERGY_HARVEST_ADDRESS      = 2339  // 1 byte
} M24LR64E_ADDR_T;

typedef enum {
    UPM_M24LR64E_USER_MODE = 0x0,   // offers simple read/write access right
    UPM_M24LR64E_ROOT_MODE = 0x1    // offers password change access right
} upm_m24lr64e_access_mode;

typedef enum {
    //      **********************************
    //      *  submit passWd *   no submit   *
    //b2,b1 *  Read * Write  *  Read * Write *
    // 00   *    1       1        1      0   *
    // 01   *    1       1        1      1   *
    // 10   *    1       1        0      0   *
    // 11   *    0       1        0      0   *
    //      **********************************
    upm_m24lr64e_access_1110 = 0,
    upm_m24lr64e_access_1111 = 1,
    upm_m24lr64e_access_1100 = 2,
    upm_m24lr64e_access_0111 = 3,
} sector_access_right;

typedef enum {
    //00 => no passwd protect
    //01 => passWd 1
    //10 => passWd 2
    //11 => passwd 3
    upm_m24lr64e_no_password = 0,
    upm_m24lr64e_password_1 = 1,
    upm_m24lr64e_password_2 = 2,
    upm_m24lr64e_password_3 = 3,
} sector_select_password;

const void* upm_m24lr64e_get_ft(upm_sensor_t sensor_type);

/*
 * Opaque pointer to the sensor struct
 */
typedef struct _upm_m24lr64e* upm_m24lr64e;

/* function comment as soon as a basic implementation is in place */
void* upm_m24lr64e_init_name();

/**
 * M24LR64E Init function
 *
 * @param bus I2C bus to use
 * @param mode Access mode (user or root) to use
 */
void* upm_m24lr64e_init(int bus, upm_m24lr64e_access_mode mode);

/**
 * M24LR64E close function
 *
 * @param dev void pointer to sensor struct
 */
void upm_m24lr64e_close(void* dev);

/**
 * Submits an I2C access password
 *
 * @param dev void pointer to sensor struct
 * @param passwd 4-byte access password
 */
upm_result_t upm_m24lr64e_submit_password(void* dev, uint32_t password);

/**
 * Writes a new I2C password
 *
 * @param dev void pointer to sensor struct
 * @param passwd 4-byte access password
 */
upm_result_t upm_m24lr64e_write_password(void* dev, uint32_t password);

/**
 * Sets a protection bit for a sector. Must be in the root mode
 *
 * @param dev void pointer to sensor struct
 * @param sectorNumber Sector whose protection you are modifying
 * @param protectEnable True if you are enabling protection
 * @param accessRight Access rights to set
 * @param passwd Password number to enable, if any
 */
upm_result_t upm_m24lr64e_sector_protect_config(void* dev,
        uint32_t sector_number,
        bool protect_enable,
        sector_access_right access_right,
        sector_select_password password);

/**
 * Clears sector protection bits. Must be in the root mode.
 *
 * @param dev void pointer to sensor struct
 */
upm_result_t upm_m24lr64e_clear_sector_protect(void* dev);

/**
 * Sets or clears a sector security status lock bit for a sector.
 * Must be in the root mode.
 *
 * @param dev void pointer to sensor struct
 * @param sectorNumber Sector whose SSS you want to modify
 * @param sockEnable True to set the bit, false to clear it
 */
upm_result_t upm_m24lr64e_sector_write_lock_bit(void* dev,
        uint32_t sector_number,
        bool sock_enable);

/**
 * Returns a data storage family identifier (DSFID)
 * Must be in the root mode.
 *
 * @param dev void pointer to sensor struct
 * @param dsfid pointer to hold return value
 */
upm_result_t upm_m24lr64e_get_dsfid(void* dev, uint8_t* dsfid);

/**
 * Returns an application family identifier (AFI)
 * Must be in the root mode.
 *
 * @param dev void pointer to sensor struct
 * @param afi pointer to hold return value
 */
upm_result_t upm_m24lr64e_get_afi(void* dev, uint8_t* afi);

/**
 * Returns a unique ID.
 * Must be in the root mode.
 * Maintained to preserve compatibility with older code.
 *
 * @param dev void pointer to sensor struct
 * @param uid pointer to hold return value
 */
upm_result_t upm_m24lr64e_get_uid(void* dev, uint8_t* uid);

/**
 * Returns the memory size
 * Must be in the root mode.
 *
 * @param dev void pointer to sensor struct
 * @param memory_size pointer to hold return value
 */
upm_result_t upm_m24lr64e_get_memory_size(void* dev, uint32_t* memory_size);

/**
 * Sets all memory to 0, if permissions allow
 *
 * @param dev void pointer to sensor struct
 */
upm_result_t upm_m24lr64e_clear_memory(void* dev);

/**
 * Writes a byte to the EEPROM
 *
 * @param dev void pointer to sensor struct
 * @param address Address to write to
 * @param data Data to write
 */
upm_result_t upm_m24lr64e_write_byte(void* dev, uint32_t address, uint8_t data);

/**
 * Writes bytes to the EEPROM
 *
 * @param dev void pointer to sensor struct
 * @param address Address to write to
 * @param data Data to write
 * @param data Length of the data buffer
 */
upm_result_t upm_m24lr64e_write_bytes(void* dev, uint32_t address, uint8_t* buffer, int len);

/**
 * Reads a byte from the EEPROM
 *
 * @param dev void pointer to sensor struct
 * @param address Address to read from
 * @param data holds return value
 */
upm_result_t upm_m24lr64e_read_byte(void* dev, uint32_t address, uint8_t* data);

/**
 * Reads multiple bytes from the EEPROM
 *
 * @param dev void pointer to sensor struct
 * @param address Address to read from
 * @param buffer Buffer to store data
 * @param len Number of bytes to read
 */
upm_result_t upm_m24lr64e_read_bytes(void* dev, uint32_t address, uint8_t* buffer, int len);

/**
 * Generic read function for the sensor. Returns
 * raw value.
 *
 * @param void* pointer to the sensor struct
 * @param void* value stores the value that was read
 * @param int len length of the elements of the
 * value that has been read
 * @return upm_result_t UPM success/error code
 */
upm_result_t upm_m24lr64e_read(const void* dev, void* data, int len);

/**
 * Generic write function for the sensor.
 *
 * @param void* pointer to the sensor struct
 * @param void* value stores the value to write
 * @param int len length of the elements of the
 * value to be written
 * @return upm_result_t UPM success/error code
 */
upm_result_t upm_m24lr64e_write(const void* dev, void* value, int len);

#endif /* M24LR64E_M24LR64E_H_ */
