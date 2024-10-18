#include "Flash.h"
#include <string.h>

static bool isLock = true;

uint8_t _FLASH_BUFFER[FLASH_SIZE];

void Flash_Unlock() {
    isLock = false;
}

void Flash_Lock() {
    isLock = true;
}

void Flash_WordProgram(uint32_t virtualAddress, uint32_t data) {
    if(isLock) return;
    *((uint32_t *)(_FLASH_BUFFER + virtualAddress)) = data;
}

void Flash_ByteProgram(uint32_t virtualAddress, uint8_t data) {
    if(isLock) return;
    _FLASH_BUFFER[virtualAddress] = data;
}

uint32_t Flash_ReadWord(uint32_t virtualAddress) {
    return ((uint32_t *)_FLASH_BUFFER)[virtualAddress];
}

uint8_t Flash_ReadByte(uint32_t virtualAddress) {
    return _FLASH_BUFFER[virtualAddress];
}

void Flash_Erase(uint32_t addrPage) {
    addrPage = (addrPage / FLASH_PAGE) * FLASH_PAGE;
    memset(_FLASH_BUFFER + addrPage, 0xFF, FLASH_PAGE);
}

uint32_t Flash_GetBaseAddress() {
    return (uint32_t)_FLASH_BUFFER;
}