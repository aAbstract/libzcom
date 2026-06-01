#include "inc/libzcom.h"

uint16_t modbus_rtu_crc(uint8_t* data, int len) {
    uint16_t crc = 0xFFFF;

    for (int i = 0; i < len; i++) {
        crc ^= (uint16_t)data[i];

        for (int j = 0; j < 8; j++) {
            if ((crc & 0x0001) != 0) {
                crc >>= 1;
                crc ^= 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }

    return crc;
}