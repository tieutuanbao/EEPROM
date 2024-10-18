#include "EEPROM.h"

#ifdef __GD32__
#include "gd32e10x.h"

    #define FLASH_BYTE_READ(i)              (*((uint8_t *)(_addressDataStart + i)))
    #define FLASH_WORD_READ(i)              (*((uint32_t *)(_addressDataStart + i)))
#else
    #define FLASH_UNLOCK()                  Flash_Unlock()
    #define FLASH_LOCK()                    Flash_Lock()
    #define FLASH_ERASEPAGE(addr)           Flash_Erase(addr)
    #define FLASH_WORD_PROG(addr, data)     Flash_WordProgram(addr, data)
    #define FLASH_BYTE_READ(i)              Flash_ReadByte(i)
    #define FLASH_WORD_READ(i)              Flash_ReadWord(i)
#endif

static uint32_t _addressDataStart;
static uint32_t _sizeData;
static uint8_t _eepromBuffer[CFG_EEPROM_BUFFER_SIZE];

void EEPROM_Init(uint32_t size) {
    if(size >= CFG_FLASH_SIZE) {
        return;
    }
    _addressDataStart = CFG_FLASH_ADDRESS + CFG_FLASH_PAGE_SIZE * ((CFG_FLASH_SIZE - size) / CFG_FLASH_PAGE_SIZE);
    _sizeData = size;
}

uint32_t EEPROM_GetAddrData() {
    return _addressDataStart;
}

/**
 * @brief Lưu EEPROM
 * 
 * @param offset địa chỉ ảo
 * @param data con trỏ dữ liệu
 * @param size kích thước dữ liệu
 */
void EEPROM_Save(uint32_t offset, uint8_t *data, uint32_t size) {
    if((offset + size) > _sizeData) return;
    uint8_t beginPage = offset / CFG_FLASH_PAGE_SIZE;
    uint32_t endPage = ((offset + size) / CFG_FLASH_PAGE_SIZE) + 1;
    uint32_t skipByteBegin = 0;
    uint32_t skipByteEnd = 0;
    uint32_t byteWriten = 0;

    for(uint16_t idxPage = beginPage; idxPage < endPage; idxPage++) {
        uint32_t offsetThisPage = idxPage * CFG_FLASH_PAGE_SIZE;
        uint32_t offsetEndThisPage = offsetThisPage + CFG_FLASH_PAGE_SIZE;
        uint32_t sizeFirstBackup = 0;
        uint32_t sizeEndBackup = 0;
        uint32_t sizeCanWrite = 0;
        if(offset > offsetThisPage) {
            /* Backup dữ liệu */
            for(uint32_t i = offsetThisPage; i < offset; i++) {
                _eepromBuffer[i - offsetThisPage] = FLASH_BYTE_READ(_addressDataStart + i);
                sizeFirstBackup++;
            }
            /* Đưa dữ liệu Data vào Buffer */
            if(offset % 4) {
                for(uint32_t i = offset; i % 4; i++) {
                    _eepromBuffer[i - offsetThisPage] = data[i - offset];
                    skipByteBegin++;
                    sizeFirstBackup++;
                }
            }
        }
        if((offset + size) < offsetEndThisPage) {
            /* Tính kích thước dữ liệu có thể ghi */
            sizeCanWrite = (offset + size) - offsetThisPage;
            uint32_t startBkp = offset + size;
            if(sizeCanWrite > size) sizeCanWrite = size;

            if(startBkp % 4) {
                while(startBkp % 4) {
                    startBkp--;
                }
                for(uint32_t i = 0; startBkp < (offset + size); i++) {
                    _eepromBuffer[sizeFirstBackup + i] = data[(size - ((offset + size) - startBkp))];
                    startBkp++;
                    skipByteEnd++;
                    sizeEndBackup++;
                }
            }
            /* Backup dữ liệu */
            for(uint32_t i = 0; i < (offsetEndThisPage - startBkp); i++) {
                _eepromBuffer[sizeFirstBackup + skipByteEnd + i] = FLASH_BYTE_READ(_addressDataStart + startBkp + i);
                sizeEndBackup++;
            }
            sizeCanWrite -= skipByteEnd;
        }
        else {
            if(offset > offsetThisPage) {
                sizeCanWrite = offsetEndThisPage - (offset + skipByteBegin);
            }
            else if(offset <= offsetThisPage) {
                sizeCanWrite = CFG_FLASH_PAGE_SIZE;
            }
        }
        /*  */
        FLASH_UNLOCK();
        FLASH_ERASEPAGE(_addressDataStart + offsetThisPage);
        if(sizeFirstBackup) {
            for(uint32_t i = 0; i < sizeFirstBackup; i += 4) {
                FLASH_WORD_PROG(_addressDataStart + offsetThisPage + i, *((uint32_t *)(_eepromBuffer + i)));
            }            
        }
        for(uint32_t i = 0; i < sizeCanWrite; i += 4) {
            FLASH_WORD_PROG(_addressDataStart + offsetThisPage + sizeFirstBackup + i, *((uint32_t *)(data + skipByteBegin + byteWriten + i)));
        }
        if(sizeEndBackup) {
            uint32_t actualDataSize = (size - skipByteBegin - skipByteEnd) - byteWriten;
            for(uint32_t i = 0; i < sizeEndBackup; i += 4) {
                FLASH_WORD_PROG(_addressDataStart + offsetThisPage + sizeFirstBackup + actualDataSize + i, *((uint32_t *)(_eepromBuffer + sizeFirstBackup + i)));
            }            
        }
        byteWriten += sizeCanWrite;
        FLASH_LOCK();
    }
}
