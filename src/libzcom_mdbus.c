#include "libzcom_mdbus.h"

// modbus-virtual-memory
uint8_t mdbus_slave_id = 0;
MDBUS_RC mdbus_set_slave_id(uint8_t _slave_id) {
    mdbus_slave_id = _slave_id;
    return MDBUS_RC_OK;
}
uint16_t* mdbus_page_table[16] = {0};
MDBUS_RC mdbus_set_page(uint8_t page_offset, uint16_t* page_ptr) {
    if (page_offset > 15)
        return MDBUS_RC_INV_PAGE_OFFSET;
    mdbus_page_table[page_offset] = page_ptr;
    return MDBUS_RC_OK;
}
MDBUS_RC mdbus_get_page(uint16_t address, uint16_t** out_page_ptr) {
    uint8_t page_offset = (address >> 12) & 0x000F;
    uint16_t* page_ptr = mdbus_page_table[page_offset];
    if (page_ptr == 0)
        return MDBUS_RC_PAGE_NOT_FOUND;

    *out_page_ptr = page_ptr;
    return MDBUS_RC_OK;
}

// mdbus-source-ops
MDBUS_RC mdbus_mv_word(uint16_t address, uint16_t word) {
    uint16_t* page_ptr;
    if (mdbus_get_page(address, &page_ptr) != MDBUS_RC_OK)
        return MDBUS_RC_PAGE_NOT_FOUND;

    uint16_t word_offset = address & 0x0FFF;
    page_ptr[word_offset] = word;
    return MDBUS_RC_OK;
}
MDBUS_RC mdbus_ld_word(uint16_t address, uint16_t* out_word) {
    uint16_t* page_ptr;
    if (mdbus_get_page(address, &page_ptr) != MDBUS_RC_OK)
        return MDBUS_RC_PAGE_NOT_FOUND;

    uint16_t word_offset = address & 0x0FFF;
    *out_word = page_ptr[word_offset];
    return MDBUS_RC_OK;
}

MDBUS_RC mdbus_mv_i16(uint16_t address, int16_t value) {
    uint16_t* page_ptr;
    if (mdbus_get_page(address, &page_ptr) != MDBUS_RC_OK)
        return MDBUS_RC_PAGE_NOT_FOUND;

    uint16_t word_offset = address & 0x0FFF;
    page_ptr[word_offset] = (uint16_t)value;
    return MDBUS_RC_OK;
}
MDBUS_RC mdbus_ld_i16(uint16_t address, int16_t* out_value) {
    uint16_t* page_ptr;
    if (mdbus_get_page(address, &page_ptr) != MDBUS_RC_OK)
        return MDBUS_RC_PAGE_NOT_FOUND;

    uint16_t word_offset = address & 0x0FFF;
    *out_value = (int16_t)page_ptr[word_offset];
    return MDBUS_RC_OK;
}

MDBUS_RC mdbus_mv_u32(uint16_t address, uint32_t value) {
    uint16_t* page_ptr;
    if (mdbus_get_page(address, &page_ptr) != MDBUS_RC_OK)
        return MDBUS_RC_PAGE_NOT_FOUND;

    uint16_t words[2];
    mdbus_u32_words(value, words);

    uint16_t word_offset = address & 0x0FFF;
    page_ptr[word_offset] = words[0];
    page_ptr[word_offset + 1] = words[1];
    return MDBUS_RC_OK;
}
MDBUS_RC mdbus_ld_u32(uint16_t address, uint32_t* out_value) {
    uint16_t* page_ptr;
    if (mdbus_get_page(address, &page_ptr) != MDBUS_RC_OK)
        return MDBUS_RC_PAGE_NOT_FOUND;

    uint16_t word_offset = address & 0x0FFF;
    uint16_t low_word = page_ptr[word_offset];
    uint16_t high_word = page_ptr[word_offset + 1];
    uint8_t* out_value_ptr = (uint8_t*)out_value;
    out_value_ptr[0] = low_word & 0xFF;
    out_value_ptr[1] = (low_word >> 8) & 0xFF;
    out_value_ptr[2] = high_word & 0xFF;
    out_value_ptr[3] = (high_word >> 8) & 0xFF;
    return MDBUS_RC_OK;
}

MDBUS_RC mdbus_mv_i32(uint16_t address, int32_t value) {
    uint16_t* page_ptr;
    if (mdbus_get_page(address, &page_ptr) != MDBUS_RC_OK)
        return MDBUS_RC_PAGE_NOT_FOUND;

    uint16_t words[2];
    mdbus_i32_words(value, words);

    uint16_t word_offset = address & 0x0FFF;
    page_ptr[word_offset] = words[0];
    page_ptr[word_offset + 1] = words[1];
    return MDBUS_RC_OK;
}
MDBUS_RC mdbus_ld_i32(uint16_t address, int32_t* out_value) {
    uint16_t* page_ptr;
    if (mdbus_get_page(address, &page_ptr) != MDBUS_RC_OK)
        return MDBUS_RC_PAGE_NOT_FOUND;

    uint16_t word_offset = address & 0x0FFF;
    uint16_t low_word = page_ptr[word_offset];
    uint16_t high_word = page_ptr[word_offset + 1];
    uint8_t* out_value_ptr = (uint8_t*)out_value;
    out_value_ptr[0] = low_word & 0xFF;
    out_value_ptr[1] = (low_word >> 8) & 0xFF;
    out_value_ptr[2] = high_word & 0xFF;
    out_value_ptr[3] = (high_word >> 8) & 0xFF;
    return MDBUS_RC_OK;
}

MDBUS_RC mdbus_mv_f32(uint16_t address, float value) {
    uint16_t* page_ptr;
    if (mdbus_get_page(address, &page_ptr) != MDBUS_RC_OK)
        return MDBUS_RC_PAGE_NOT_FOUND;

    uint16_t words[2];
    mdbus_f32_words(value, words);

    uint16_t word_offset = address & 0x0FFF;
    page_ptr[word_offset] = words[0];
    page_ptr[word_offset + 1] = words[1];
    return MDBUS_RC_OK;
}
MDBUS_RC mdbus_ld_f32(uint16_t address, float* out_value) {
    uint16_t* page_ptr;
    if (mdbus_get_page(address, &page_ptr) != MDBUS_RC_OK)
        return MDBUS_RC_PAGE_NOT_FOUND;

    uint16_t word_offset = address & 0x0FFF;
    uint16_t low_word = page_ptr[word_offset];
    uint16_t high_word = page_ptr[word_offset + 1];
    uint8_t* out_value_ptr = (uint8_t*)out_value;
    out_value_ptr[0] = low_word & 0xFF;
    out_value_ptr[1] = (low_word >> 8) & 0xFF;
    out_value_ptr[2] = high_word & 0xFF;
    out_value_ptr[3] = (high_word >> 8) & 0xFF;
    return MDBUS_RC_OK;
}

// mdbus-codecs
MDBUS_RC mdbus_encode_read_holding_regs(uint16_t address, uint16_t word_cnt, uint8_t* out_packet) {
    out_packet[0] = mdbus_slave_id;
    out_packet[1] = MDBUS_FC_READ_HOLDING_REGS;
    out_packet[2] = (address >> 8) & 0xFF;
    out_packet[3] = address & 0xFF;
    out_packet[4] = (word_cnt >> 8) & 0xFF;
    out_packet[5] = word_cnt & 0xFF;
    uint16_t crc16 = mdbus_rtu_crc(out_packet, 6);
    out_packet[6] = crc16 & 0xFF;
    out_packet[7] = (crc16 >> 8) & 0xFF;
    return MDBUS_RC_OK;
}

MDBUS_RC mdbus_encode_read_input_regs(uint16_t address, uint16_t word_cnt, uint8_t* out_packet) {
    out_packet[0] = mdbus_slave_id;
    out_packet[1] = MDBUS_FC_READ_INPUT_REGS;
    out_packet[2] = (address >> 8) & 0xFF;
    out_packet[3] = address & 0xFF;
    out_packet[4] = (word_cnt >> 8) & 0xFF;
    out_packet[5] = word_cnt & 0xFF;
    uint16_t crc16 = mdbus_rtu_crc(out_packet, 6);
    out_packet[6] = crc16 & 0xFF;
    out_packet[7] = (crc16 >> 8) & 0xFF;
    return MDBUS_RC_OK;
}

MDBUS_RC mdbus_encode_write_regs(uint16_t address, uint16_t* word_list, uint16_t word_cnt, uint8_t* out_packet) {
    out_packet[0] = mdbus_slave_id;
    out_packet[1] = MDBUS_FC_WRITE_REGS;
    out_packet[2] = (address >> 8) & 0xFF;
    out_packet[3] = address & 0xFF;
    out_packet[4] = (word_cnt >> 8) & 0xFF;
    out_packet[5] = word_cnt & 0xFF;
    out_packet[6] = word_cnt * 2;

    for (uint8_t i = 0; i < word_cnt; i++) {
        uint16_t word = word_list[i];
        uint16_t idx = 7 + 2 * i;
        out_packet[idx] = (word >> 8) & 0xFF;
        out_packet[idx + 1] = word & 0xFF;
    }

    uint16_t data_size = 7 + word_cnt * 2;
    uint16_t crc16 = mdbus_rtu_crc(out_packet, data_size);
    out_packet[data_size] = crc16 & 0xFF;
    out_packet[data_size + 1] = (crc16 >> 8) & 0xFF;
    return MDBUS_RC_OK;
}

// mdbus-request-handler
MDBUS_RC mdbus_handle_read_request(const uint8_t* request_packet, uint8_t packet_size) {
    uint8_t offset_high_nib = request_packet[2] >> 4;
    uint16_t* page_ptr = mdbus_page_table[offset_high_nib];
    if (page_ptr == 0)
        return MDBUS_RC_PAGE_NOT_FOUND;

    uint16_t mdbus_offset = 0xFFFF;
    ((uint8_t*)&mdbus_offset)[1] = request_packet[2];
    ((uint8_t*)&mdbus_offset)[0] = request_packet[3];
    uint16_t page_offset = mdbus_offset & 0x0FFF;

    uint16_t word_cnt = 0xFFFF;
    ((uint8_t*)&word_cnt)[1] = request_packet[4];
    ((uint8_t*)&word_cnt)[0] = request_packet[5];
    uint8_t byte_count = word_cnt * 2;

    uint8_t response_packet[256];
    response_packet[0] = mdbus_slave_id;
    response_packet[1] = request_packet[1];  // MDBUS_FC
    response_packet[2] = byte_count;

    for (uint8_t i = 0; i < word_cnt; i++) {
        uint16_t word = page_ptr[page_offset + i];
        uint16_t idx = 3 + 2 * i;
        response_packet[idx] = ((uint8_t*)&word)[1];
        response_packet[idx + 1] = ((uint8_t*)&word)[0];
    }

    uint16_t data_size = 3 + word_cnt * 2;
    uint16_t crc16 = mdbus_rtu_crc(response_packet, data_size);
    response_packet[data_size] = crc16 & 0xFF;
    response_packet[data_size + 1] = (crc16 >> 8) & 0xFF;

    mdbus_transmit(response_packet, data_size + 2);
    return MDBUS_RC_OK;
}

MDBUS_RC mdbus_handle_write_request(const uint8_t* request_packet, uint8_t packet_size) {
    uint8_t offset_high_nib = request_packet[2] >> 4;
    uint16_t* page_ptr = mdbus_page_table[offset_high_nib];
    if (page_ptr == 0)
        return MDBUS_RC_PAGE_NOT_FOUND;

    uint16_t mdbus_offset = 0xFFFF;
    ((uint8_t*)&mdbus_offset)[1] = request_packet[2];
    ((uint8_t*)&mdbus_offset)[0] = request_packet[3];
    uint16_t page_offset = mdbus_offset & 0x0FFF;

    uint16_t word_cnt = 0xFFFF;
    ((uint8_t*)&word_cnt)[1] = request_packet[4];
    ((uint8_t*)&word_cnt)[0] = request_packet[5];

    for (uint8_t i = 0; i < word_cnt; i++) {
        uint16_t word = 0xFFFF;
        uint16_t idx = 7 + 2 * i;
        ((uint8_t*)&word)[1] = request_packet[idx];
        ((uint8_t*)&word)[0] = request_packet[idx + 1];
        page_ptr[page_offset + i] = word;
    }

    return MDBUS_RC_OK;
}

MDBUS_RC mdbus_handle_request(const uint8_t* request_packet, uint16_t packet_size) {
    if (packet_size < 8)
        return MDBUS_RC_ERR_PKT_TOO_SMALL;

    // CRC-16 check
    uint16_t packet_crc16 = 0xFFFF;
    ((uint8_t*)&packet_crc16)[0] = request_packet[packet_size - 2];
    ((uint8_t*)&packet_crc16)[1] = request_packet[packet_size - 1];
    uint16_t target_crc16 = mdbus_rtu_crc(request_packet, packet_size - 2);
    if (packet_crc16 != target_crc16)
        return MDBUS_RC_ERR_INV_CRC16;

    if (request_packet[0] != mdbus_slave_id)
        return MDBUS_RC_ERR_SLV_ID_MISMATCH;

    uint8_t packet_fc = request_packet[1];
    if (packet_fc == MDBUS_FC_READ_INPUT_REGS || packet_fc == MDBUS_FC_READ_HOLDING_REGS)
        return mdbus_handle_read_request(request_packet, packet_size);

    if (packet_fc == MDBUS_FC_WRITE_REGS)
        return mdbus_handle_write_request(request_packet, packet_size);

    return MDBUS_RC_ERR_UNK_FC;
}

// mdbus-utils
uint16_t mdbus_rtu_crc(const uint8_t* data, uint16_t len) {
    uint16_t crc = 0xFFFF;
    for (uint16_t i = 0; i < len; i++) {
        crc ^= (uint16_t)data[i];
        for (uint8_t j = 0; j < 8; j++) {
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

void mdbus_u32_words(uint32_t value, uint16_t* out_words) {
    uint16_t low_word = 0;
    uint16_t high_word = 0;
    ((uint8_t*)&low_word)[0] = (uint8_t)(value & 0xFF);
    ((uint8_t*)&low_word)[1] = (uint8_t)((value >> 8) & 0xFF);
    ((uint8_t*)&high_word)[0] = (uint8_t)((value >> 16) & 0xFF);
    ((uint8_t*)&high_word)[1] = (uint8_t)((value >> 24) & 0xFF);
    out_words[0] = low_word;
    out_words[1] = high_word;
}

void mdbus_i32_words(int32_t value, uint16_t* out_words) {
    uint16_t low_word = 0;
    uint16_t high_word = 0;
    uint32_t value_u32 = *(uint32_t*)&value;
    ((uint8_t*)&low_word)[0] = (uint8_t)(value_u32 & 0xFF);
    ((uint8_t*)&low_word)[1] = (uint8_t)((value_u32 >> 8) & 0xFF);
    ((uint8_t*)&high_word)[0] = (uint8_t)((value_u32 >> 16) & 0xFF);
    ((uint8_t*)&high_word)[1] = (uint8_t)((value_u32 >> 24) & 0xFF);
    out_words[0] = low_word;
    out_words[1] = high_word;
}

void mdbus_f32_words(float value, uint16_t* out_words) {
    uint16_t low_word = 0;
    uint16_t high_word = 0;
    uint32_t value_u32 = *(uint32_t*)&value;
    ((uint8_t*)&low_word)[0] = (uint8_t)(value_u32 & 0xFF);
    ((uint8_t*)&low_word)[1] = (uint8_t)((value_u32 >> 8) & 0xFF);
    ((uint8_t*)&high_word)[0] = (uint8_t)((value_u32 >> 16) & 0xFF);
    ((uint8_t*)&high_word)[1] = (uint8_t)((value_u32 >> 24) & 0xFF);
    out_words[0] = low_word;
    out_words[1] = high_word;
}

uint8_t* mdbus_tx_buffer = 0;
void mdbus_set_tx_buffer(uint8_t* tx_buffer) {
    mdbus_tx_buffer = tx_buffer;
}

__attribute__((weak)) void mdbus_transmit(uint8_t* packet, uint8_t packet_size) {
    if (mdbus_tx_buffer != 0)
        memcpy(mdbus_tx_buffer, packet, packet_size);
}

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