#ifndef __EEPROM_H
#define __EEPROM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#ifdef __GD32__
    #define FLASH_ADDRESS       0x08000000
    #define FLASH_SIZE          0x020000
    #define FLASH_PAGE_SIZE     0x0400

    #define EEPROM_BUFFER_SIZE  (2 * 0x0400)
#else
    #include "Flash.h"

    #define CFG_FLASH_ADDRESS       0
    #define CFG_FLASH_SIZE          FLASH_SIZE
    #define CFG_FLASH_PAGE_SIZE     FLASH_PAGE

    #define CFG_EEPROM_BUFFER_SIZE  (1024)
#endif

void EEPROM_Init(uint32_t size);
uint32_t EEPROM_GetAddrData();
void EEPROM_Save(uint32_t offset, uint8_t *data, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif
