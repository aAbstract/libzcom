#include "libzcom_ltbus.h"

// ltbus-virtual-memory
uint8_t ltbus_slave_id = 0;
LTBUS_RC ltbus_set_slave_id(uint8_t _slave_id) {
    ltbus_slave_id = _slave_id;
    return LTBUS_RC_OK;
}
uint8_t* ltbus_page_table[16] = {0};
LTBUS_RC ltbus_set_page(uint8_t page_offset, uint8_t* page_ptr) {
    if (page_offset > 15)
        return LTBUS_RC_INV_PAGE_OFFSET;
    ltbus_page_table[page_offset] = page_ptr;
    return LTBUS_RC_OK;
}
LTBUS_RC ltbus_get_page(uint16_t address, uint8_t** out_page_ptr) {
    uint8_t page_offset = (address >> 12) & 0x000F;
    uint8_t* page_ptr = ltbus_page_table[page_offset];
    if (page_ptr == 0)
        return LTBUS_RC_PAGE_NOT_FOUND;

    *out_page_ptr = page_ptr;
    return LTBUS_RC_OK;
}

// ltbus-codecs
LTBUS_RC ltbus_encode_read_regs(uint16_t address, uint16_t data_size, uint8_t* out_packet) {
    out_packet[0] = 0x7B;
    out_packet[1] = ltbus_slave_id;
    out_packet[2] = LTBUS_FC_READ;
    out_packet[3] = address & 0xFF;
    out_packet[4] = (address >> 8) & 0xFF;
    out_packet[5] = data_size & 0xFF;
    out_packet[6] = (data_size >> 8) & 0xFF;
    uint16_t crc16 = ltbus_crc(out_packet, LTBUS_PACKET_HEADER_SIZE);
    out_packet[7] = crc16 & 0xFF;
    out_packet[8] = (crc16 >> 8) & 0xFF;
    out_packet[9] = 0x7D;
    return LTBUS_RC_OK;
}

LTBUS_RC ltbus_encode_write_regs(uint16_t address, uint8_t* data_buffer, uint16_t data_size, uint8_t* out_packet) {
    out_packet[0] = 0x7B;
    out_packet[1] = ltbus_slave_id;
    out_packet[2] = LTBUS_FC_WRITE;
    out_packet[3] = address & 0xFF;
    out_packet[4] = (address >> 8) & 0xFF;
    out_packet[5] = data_size & 0xFF;
    out_packet[6] = (data_size >> 8) & 0xFF;

    for (uint16_t i = 0; i < data_size; i++) {
        uint8_t _byte = data_buffer[i];
        uint16_t idx = LTBUS_PACKET_HEADER_SIZE + i;
        out_packet[idx] = _byte;
    }

    uint16_t packet_data_size = LTBUS_PACKET_HEADER_SIZE + data_size;
    uint16_t crc16 = ltbus_crc(out_packet, packet_data_size);
    out_packet[packet_data_size] = crc16 & 0xFF;
    out_packet[packet_data_size + 1] = (crc16 >> 8) & 0xFF;
    out_packet[packet_data_size + 2] = 0x7D;
    return LTBUS_RC_OK;
}

// ltbus-request-handler
LTBUS_RC ltbus_handle_read_request(const uint8_t* request_packet, uint8_t packet_size) {
    uint16_t register_address = 0;
    ((uint8_t*)&register_address)[0] = request_packet[3];
    ((uint8_t*)&register_address)[1] = request_packet[4];

    uint16_t register_size = 0;
    ((uint8_t*)&register_size)[0] = request_packet[5];
    ((uint8_t*)&register_size)[1] = request_packet[6];

    uint8_t* page_ptr = 0;
    if (ltbus_get_page(register_address, &page_ptr) != LTBUS_RC_OK)
        return LTBUS_RC_PAGE_NOT_FOUND;

    uint8_t read_resp_packet[256];
    read_resp_packet[0] = 0x7B;
    read_resp_packet[1] = ltbus_slave_id;
    read_resp_packet[2] = LTBUS_FC_READ_RESP;
    read_resp_packet[3] = request_packet[3];
    read_resp_packet[4] = request_packet[4];
    read_resp_packet[5] = request_packet[5];
    read_resp_packet[6] = request_packet[6];

    uint16_t register_offset = register_address & 0x0FFF;
    for (uint8_t i = 0; i < register_size; i++)
        read_resp_packet[LTBUS_PACKET_HEADER_SIZE + i] = page_ptr[register_offset + i];

    uint16_t crc16 = ltbus_crc(read_resp_packet, LTBUS_PACKET_HEADER_SIZE + register_size);
    read_resp_packet[LTBUS_PACKET_HEADER_SIZE + register_size] = crc16 & 0xFF;
    read_resp_packet[LTBUS_PACKET_HEADER_SIZE + register_size + 1] = (crc16 >> 8) & 0xFF;
    read_resp_packet[LTBUS_PACKET_HEADER_SIZE + register_size + 2] = 0x7D;
    ltbus_transmit(read_resp_packet, LTBUS_PACKET_HEADER_SIZE + LTBUS_PACKET_FOOTER_SIZE + register_size);
    return LTBUS_RC_OK;
}

LTBUS_RC ltbus_handle_write_request(const uint8_t* request_packet, uint8_t packet_size) {
    uint16_t register_address = 0;
    ((uint8_t*)&register_address)[0] = request_packet[3];
    ((uint8_t*)&register_address)[1] = request_packet[4];

    uint16_t register_size = 0;
    ((uint8_t*)&register_size)[0] = request_packet[5];
    ((uint8_t*)&register_size)[1] = request_packet[6];

    uint8_t* page_ptr = 0;
    if (ltbus_get_page(register_address, &page_ptr) != LTBUS_RC_OK)
        return LTBUS_RC_PAGE_NOT_FOUND;

    uint16_t register_offset = register_address & 0x0FFF;
    for (uint8_t i = 0; i < register_size; i++)
        page_ptr[register_offset + i] = request_packet[LTBUS_PACKET_HEADER_SIZE + i];

    return LTBUS_RC_OK;
}

LTBUS_RC ltbus_handle_request(const uint8_t* request_packet, uint16_t packet_size) {
    if (packet_size < LTBUS_PACKET_HEADER_SIZE + LTBUS_PACKET_FOOTER_SIZE)
        return LTBUS_RC_ERR_PKT_TOO_SMALL;

    // CRC-16 check
    uint16_t packet_crc16 = 0xFFFF;
    ((uint8_t*)&packet_crc16)[0] = request_packet[packet_size - LTBUS_PACKET_FOOTER_SIZE];
    ((uint8_t*)&packet_crc16)[1] = request_packet[packet_size - LTBUS_PACKET_FOOTER_SIZE + 1];
    uint16_t target_crc16 = ltbus_crc(request_packet, packet_size - LTBUS_PACKET_FOOTER_SIZE);
    if (packet_crc16 != target_crc16)
        return LTBUS_RC_ERR_INV_CRC16;

    if (request_packet[1] != ltbus_slave_id)
        return LTBUS_RC_ERR_SLV_ID_MISMATCH;

    uint8_t packet_fc = request_packet[2];
    if (packet_fc == LTBUS_FC_READ)
        return ltbus_handle_read_request(request_packet, packet_size);

    if (packet_fc == LTBUS_FC_WRITE)
        return ltbus_handle_write_request(request_packet, packet_size);

    return LTBUS_RC_ERR_UNK_FC;
}

// ltbus-utils
uint16_t ltbus_crc(const uint8_t* data, uint16_t len) {
    uint16_t res = 0xFFFF;
    for (uint8_t i = 0; i < len; i++)
        res = (res >> 8) ^ CRC16_POLYNOMIAL[(res ^ data[i]) & 0xFF];
    return ~res;
}

uint8_t* ltbus_tx_buffer = 0;
void ltbus_set_tx_buffer(uint8_t* tx_buffer) {
    ltbus_tx_buffer = tx_buffer;
}

__attribute__((weak)) void ltbus_transmit(uint8_t* packet, uint8_t packet_size) {
    if (ltbus_tx_buffer != 0)
        memcpy(ltbus_tx_buffer, packet, packet_size);
}
