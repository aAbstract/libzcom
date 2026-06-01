#ifndef LIBZCOM_H
#define LIBZCOM_H

#include <stdint.h>

uint16_t modbus_rtu_crc(uint8_t* data, int len);

#endif
