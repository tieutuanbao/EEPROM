#ifndef __FLASH_H
#define __FLASH_H

#include <stdbool.h>
#include <stdint.h>

#define FLASH_PAGE      32
#define FLASH_SIZE      (FLASH_PAGE * 128)

void Flash_Lock();
void Flash_Unlock();
void Flash_Erase(uint32_t addrPage);
void Flash_WordProgram(uint32_t virtualAddress, uint32_t data);
void Flash_ByteProgram(uint32_t virtualAddress, uint8_t data);
uint8_t Flash_ReadByte(uint32_t virtualAddress);
uint32_t Flash_ReadWord(uint32_t virtualAddress);
uint32_t Flash_GetBaseAddress();

#endif
