#include "EEPROM.h"
#include <stdio.h>

uint8_t templateData[50];

#define EEPROM_SIZE         (FLASH_PAGE * 127)

void main() {
    EEPROM_Init(EEPROM_SIZE);
    /* Tạo dữ liệu ghi đè */
    for(uint16_t i = 0; i < sizeof(templateData); i++) {
        templateData[i] = i;
    }
    /* Ghi dữ liệu bất kỳ vào Flash */
    Flash_Unlock();
    for(uint16_t i = 0; i < 100; i++) {
        Flash_ByteProgram(EEPROM_GetAddrData() + i, i);
    }
    Flash_Lock();
    /* in ra */
    printf("Old data: \r\n");
    for(uint32_t i = 0; i < 100; i++) {
        printf("%04d, ", Flash_ReadByte(EEPROM_GetAddrData() + i));
    }
    printf("\r\n");
    /* Thay đổi dữ liệu ở vị trí x */
    uint8_t x = 30;
    EEPROM_Save(x, templateData, sizeof(templateData));
    /* in ra */
    printf("\r\nNew data: \r\n");
    for(uint32_t i = 0; i < 100; i++) {
        printf("%04d, ", Flash_ReadByte(EEPROM_GetAddrData() + i));
    }
    printf("\r\n");
}