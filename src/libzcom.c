#include "inc/libzcom.h"

// Utils
uint16_t set_bit(uint16_t x, uint8_t pos) {
    return x | (1 << pos);
}
uint16_t clear_bit(uint16_t x, uint8_t pos) {
    return (x & ~(1 << pos));
}
uint16_t toggle_bit(uint16_t x, uint8_t pos) {
    return x ^ (1 << pos);
}
uint16_t check_bit(uint16_t x, uint8_t pos) {
    return (x & (1 << pos)) != 0;
}

// ModBus
uint16_t modbus_rtu_crc(const uint8_t* data, int len) {
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

// LTBus
uint8_t slave_id = 0;
uint8_t* config_buffer = 0;
uint8_t* data_buffer = 0;
LTBUS_RC ltbus_init_device(uint8_t _slave_id, uint8_t* _config_buffer, uint8_t* _data_buffer) {
    slave_id = _slave_id;
    config_buffer = _config_buffer;
    data_buffer = _data_buffer;
    return LTBUS_RC_OK;
}

LTBUS_RC ltbus_decode_device_register_config(const uint8_t* request_packet, DeviceRegisterConfig* out_conf) {
    uint16_t register_address = 0;
    ((uint8_t*)&register_address)[0] = request_packet[3];
    ((uint8_t*)&register_address)[1] = request_packet[4];
    out_conf->register_address = register_address;

    uint16_t register_size = 0;
    ((uint8_t*)&register_size)[0] = request_packet[5];
    ((uint8_t*)&register_size)[1] = request_packet[6];
    out_conf->register_size = register_size;

    uint16_t buffer_address = register_address & 0xF000;
    uint16_t register_offset = register_address & 0x0FFF;
    if (buffer_address == 0xA000)
        out_conf->register_ptr = &(config_buffer[register_offset]);
    else if (buffer_address == 0xD000)
        out_conf->register_ptr = &(data_buffer[register_offset]);
    else
        return LTBUS_RC_ERR_UNK_DEVICE_BUFFER;

    return LTBUS_RC_OK;
}

LTBUS_RC ltbus_read_request(uint16_t address, uint16_t data_size, uint8_t* out_packet) {
    out_packet[0] = 0x7B;
    out_packet[1] = slave_id;
    out_packet[2] = LTBUS_READ_FC;

    out_packet[3] = address & 0xFF;
    out_packet[4] = (address >> 8) & 0xFF;

    out_packet[5] = data_size & 0xFF;
    out_packet[6] = (data_size >> 8) & 0xFF;

    uint16_t crc16 = ltbus_crc(out_packet, 7);
    out_packet[7] = crc16 & 0xFF;
    out_packet[8] = (crc16 >> 8) & 0xFF;
    out_packet[9] = 0x7D;

    return LTBUS_RC_OK;
}

LTBUS_RC ltbus_write_f32_request(uint16_t address, float value, uint8_t* out_packet) {
    out_packet[0] = 0x7B;
    out_packet[1] = slave_id;
    out_packet[2] = LTBUS_WRITE_FC;

    out_packet[3] = address & 0xFF;
    out_packet[4] = (address >> 8) & 0xFF;

    out_packet[5] = 0x04;
    out_packet[6] = 0x00;

    uint32_t value_u32 = *(uint32_t*)&value;
    out_packet[7] = (uint8_t)(value_u32 & 0xFF);
    out_packet[8] = (uint8_t)((value_u32 >> 8) & 0xFF);
    out_packet[9] = (uint8_t)((value_u32 >> 16) & 0xFF);
    out_packet[10] = (uint8_t)((value_u32 >> 24) & 0xFF);

    uint16_t crc16 = ltbus_crc(out_packet, 11);
    out_packet[11] = (uint8_t)(crc16 & 0xFF);
    out_packet[12] = (uint8_t)((crc16 >> 8) & 0xFF);
    out_packet[13] = 0x7D;

    return LTBUS_RC_OK;
}

LTBUS_RC ltbus_write_u16_request(uint16_t address, uint16_t value, uint8_t* out_packet) {
    out_packet[0] = 0x7B;
    out_packet[1] = slave_id;
    out_packet[2] = LTBUS_WRITE_FC;

    out_packet[3] = address & 0xFF;
    out_packet[4] = (address >> 8) & 0xFF;

    out_packet[5] = 0x02;
    out_packet[6] = 0x00;

    out_packet[7] = (uint8_t)(value & 0xFF);
    out_packet[8] = (uint8_t)((value >> 8) & 0xFF);

    uint16_t crc16 = ltbus_crc(out_packet, 9);
    out_packet[9] = (uint8_t)(crc16 & 0xFF);
    out_packet[10] = (uint8_t)((crc16 >> 8) & 0xFF);
    out_packet[11] = 0x7D;

    return LTBUS_RC_OK;
}

LTBUS_RC ltbus_write_i16_request(uint16_t address, int16_t value, uint8_t* out_packet) {
    out_packet[0] = 0x7B;
    out_packet[1] = slave_id;
    out_packet[2] = LTBUS_WRITE_FC;

    out_packet[3] = address & 0xFF;
    out_packet[4] = (address >> 8) & 0xFF;

    out_packet[5] = 0x02;
    out_packet[6] = 0x00;

    out_packet[7] = (uint8_t)(value & 0xFF);
    out_packet[8] = (uint8_t)((value >> 8) & 0xFF);

    uint16_t crc16 = ltbus_crc(out_packet, 9);
    out_packet[9] = (uint8_t)(crc16 & 0xFF);
    out_packet[10] = (uint8_t)((crc16 >> 8) & 0xFF);
    out_packet[11] = 0x7D;

    return LTBUS_RC_OK;
}

uint16_t ltbus_crc(const uint8_t* data, int len) {
    uint16_t res = 0xFFFF;
    for (uint8_t i = 0; i < len; i++)
        res = (res >> 8) ^ CRC16_POLYNOMIAL[(res ^ data[i]) & 0xFF];
    return ~res;
}

uint8_t* vm_buffer = 0;
void set_vm_buffer(uint8_t* _vm_buffer) {
    vm_buffer = _vm_buffer;
}
__attribute__((weak)) void ltbus_send(uint8_t* packet, uint8_t packet_size) {
    if (vm_buffer != 0)
        memcpy(vm_buffer, packet, packet_size);
}

LTBUS_RC ltbus_handle_read_request(const uint8_t* request_packet, uint8_t packet_size) {
    DeviceRegisterConfig dr_conf;
    uint8_t ddr_rc = decode_device_register_config(request_packet, packet_size, &dr_conf);
    if (ddr_rc != LTBUS_RC_OK)
        return ddr_rc;

    uint8_t read_resp_packet[LTBUS_MAX_TEMP_BUFFER];
    read_resp_packet[0] = '{';
    read_resp_packet[1] = slave_id;
    read_resp_packet[2] = LTBUS_READ_RESP_FC;
    read_resp_packet[3] = request_packet[3];
    read_resp_packet[4] = request_packet[4];
    read_resp_packet[5] = request_packet[5];
    read_resp_packet[6] = request_packet[6];

    for (uint8_t i = 0; i < dr_conf.register_size; i++)
        read_resp_packet[LTBUS_PACKET_HEADER_SIZE + i] = dr_conf.register_ptr[i];

    uint16_t crc16 = ltbus_crc(read_resp_packet, LTBUS_PACKET_HEADER_SIZE + dr_conf.register_size);
    uint8_t* crc16_ptr = (uint8_t*)&crc16;
    read_resp_packet[LTBUS_PACKET_HEADER_SIZE + dr_conf.register_size] = crc16_ptr[0];
    read_resp_packet[LTBUS_PACKET_HEADER_SIZE + dr_conf.register_size + 1] = crc16_ptr[1];
    read_resp_packet[LTBUS_PACKET_HEADER_SIZE + dr_conf.register_size + 2] = '}';
    ltbus_send(read_resp_packet, LTBUS_PACKET_HEADER_SIZE + LTBUS_PACKET_FOOTER_SIZE + dr_conf.register_size);
    return LTBUS_RC_OK;
}

LTBUS_RC ltbus_handle_write_request(const uint8_t* request_packet, uint8_t packet_size) {
    DeviceRegisterConfig dr_conf;
    uint8_t ddr_rc = decode_device_register_config(request_packet, packet_size, &dr_conf);
    if (ddr_rc != LTBUS_RC_OK)
        return ddr_rc;

    for (uint8_t i = 0; i < dr_conf.register_size; i++)
        dr_conf.register_ptr[i] = request_packet[LTBUS_PACKET_HEADER_SIZE + i];

    return LTBUS_RC_OK;
}

LTBUS_RC ltbus_handle_request(const uint8_t* request_packet, uint8_t packet_size) {
    if (packet_size < (LTBUS_PACKET_HEADER_SIZE + LTBUS_PACKET_FOOTER_SIZE))
        return LTBUS_RC_ERR_PKT_TOO_SMALL;

    // CRC-16 check
    uint16_t packet_crc16 = 0xFFFF;
    ((uint8_t*)&packet_crc16)[0] = request_packet[packet_size - 3];
    ((uint8_t*)&packet_crc16)[1] = request_packet[packet_size - 2];
    uint16_t target_crc16 = ltbus_crc(request_packet, packet_size - LTBUS_PACKET_FOOTER_SIZE);
    if (packet_crc16 != target_crc16)
        return LTBUS_RC_ERR_INV_CRC16;

    if (request_packet[1] != slave_id)
        return LTBUS_RC_ERR_SLV_ID_MISMATCH;

    uint8_t packet_fc = request_packet[2];
    if (packet_fc == LTBUS_READ_FC)
        return ltbus_handle_read_request(request_packet, packet_size);

    if (packet_fc == LTBUS_WRITE_FC)
        return ltbus_handle_write_request(request_packet, packet_size);

    return LTBUS_RC_ERR_UNK_FC;
}
