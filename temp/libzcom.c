#include "libzcom.h"

#define UNIX_DEBUG

#ifdef UNIX_DEBUG

#include <signal.h>
#include <stdio.h>
#include <unistd.h>

#define GDB_TRIGGER                        \
    printf("GDB Trigger: %d\n", getpid()); \
    raise(SIGTRAP)

#endif

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

    uint16_t low_word = 0;
    uint16_t high_word = 0;
    ((uint8_t*)&low_word)[0] = (uint8_t)(value & 0xFF);
    ((uint8_t*)&low_word)[1] = (uint8_t)((value >> 8) & 0xFF);
    ((uint8_t*)&high_word)[0] = (uint8_t)((value >> 16) & 0xFF);
    ((uint8_t*)&high_word)[1] = (uint8_t)((value >> 24) & 0xFF);

    uint16_t word_offset = address & 0x0FFF;
    page_ptr[word_offset] = low_word;
    page_ptr[word_offset + 1] = high_word;
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

    uint16_t low_word = 0;
    uint16_t high_word = 0;
    uint32_t value_u32 = *(uint32_t*)&value;
    ((uint8_t*)&low_word)[0] = (uint8_t)(value_u32 & 0xFF);
    ((uint8_t*)&low_word)[1] = (uint8_t)((value_u32 >> 8) & 0xFF);
    ((uint8_t*)&high_word)[0] = (uint8_t)((value_u32 >> 16) & 0xFF);
    ((uint8_t*)&high_word)[1] = (uint8_t)((value_u32 >> 24) & 0xFF);

    uint16_t word_offset = address & 0x0FFF;
    page_ptr[word_offset] = low_word;
    page_ptr[word_offset + 1] = high_word;
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

    uint16_t low_word = 0;
    uint16_t high_word = 0;
    uint32_t value_u32 = *(uint32_t*)&value;
    ((uint8_t*)&low_word)[0] = (uint8_t)(value_u32 & 0xFF);
    ((uint8_t*)&low_word)[1] = (uint8_t)((value_u32 >> 8) & 0xFF);
    ((uint8_t*)&high_word)[0] = (uint8_t)((value_u32 >> 16) & 0xFF);
    ((uint8_t*)&high_word)[1] = (uint8_t)((value_u32 >> 24) & 0xFF);

    uint16_t word_offset = address & 0x0FFF;
    page_ptr[word_offset] = low_word;
    page_ptr[word_offset + 1] = high_word;
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

MDBUS_RC mdbus_read_holding_regs_request(uint16_t offset, uint16_t word_cnt, uint8_t* out_packet) {
    out_packet[0] = mdbus_slave_id;
    out_packet[1] = MDBUS_FC_READ_HOLDING_REGS;

    out_packet[2] = (offset >> 8) & 0xFF;
    out_packet[3] = offset & 0xFF;

    out_packet[4] = (word_cnt >> 8) & 0xFF;
    out_packet[5] = word_cnt & 0xFF;

    uint16_t crc16 = mdbus_rtu_crc(out_packet, 6);
    out_packet[6] = crc16 & 0xFF;
    out_packet[7] = (crc16 >> 8) & 0xFF;

    return MDBUS_RC_OK;
}

MDBUS_RC mdbus_read_input_regs_request(uint16_t offset, uint16_t word_cnt, uint8_t* out_packet) {
    out_packet[0] = mdbus_slave_id;
    out_packet[1] = MDBUS_FC_READ_INPUT_REGS;

    out_packet[2] = (offset >> 8) & 0xFF;
    out_packet[3] = offset & 0xFF;

    out_packet[4] = (word_cnt >> 8) & 0xFF;
    out_packet[5] = word_cnt & 0xFF;

    uint16_t crc16 = mdbus_rtu_crc(out_packet, 6);
    out_packet[6] = crc16 & 0xFF;
    out_packet[7] = (crc16 >> 8) & 0xFF;

    return MDBUS_RC_OK;
}

MDBUS_RC mdbus_write_holding_regs_request(uint16_t offset, uint16_t* word_list, uint16_t word_cnt, uint8_t* out_packet) {
    out_packet[0] = mdbus_slave_id;
    out_packet[1] = MDBUS_FC_WRITE_HOLDING_REGS;

    out_packet[2] = (offset >> 8) & 0xFF;
    out_packet[3] = offset & 0xFF;

    out_packet[4] = (word_cnt >> 8) & 0xFF;
    out_packet[5] = word_cnt & 0xFF;
    out_packet[6] = word_cnt * 2;

    for (uint8_t i = 0; i < word_cnt; i++) {
        uint16_t word = word_list[i];
        uint16_t idx = 7 + 2 * i;
        out_packet[idx] = (word >> 8) & 0xFF;
        out_packet[idx + 1] = word & 0xFF;
    }

    uint16_t packet_size = 6 + word_cnt * 2;
    uint16_t crc16 = mdbus_rtu_crc(out_packet, packet_size);
    out_packet[6] = crc16 & 0xFF;
    out_packet[7] = (crc16 >> 8) & 0xFF;

    return MDBUS_RC_OK;
}

MDBUS_RC mdbus_handle_read_request(const uint8_t* request_packet, uint8_t packet_size) {
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
    if (packet_size < (MDBUS_PACKET_HEADER_SIZE + MDBUS_PACKET_FOOTER_SIZE))
        return MDBUS_RC_ERR_PKT_TOO_SMALL;

    // CRC-16 check
    uint16_t packet_crc16 = 0xFFFF;
    ((uint8_t*)&packet_crc16)[0] = request_packet[packet_size - 2];
    ((uint8_t*)&packet_crc16)[1] = request_packet[packet_size - 1];
    uint16_t target_crc16 = mdbus_rtu_crc(request_packet, packet_size - MDBUS_PACKET_FOOTER_SIZE);
    if (packet_crc16 != target_crc16)
        return MDBUS_RC_ERR_INV_CRC16;

    if (request_packet[0] != mdbus_slave_id)
        return MDBUS_RC_ERR_SLV_ID_MISMATCH;

    uint8_t packet_fc = request_packet[1];
    if (packet_fc == MDBUS_FC_READ_INPUT_REGS || packet_fc == MDBUS_FC_READ_HOLDING_REGS)
        return mdbus_handle_read_request(request_packet, packet_size);

    if (packet_fc == MDBUS_FC_WRITE_HOLDING_REGS)
        return mdbus_handle_write_request(request_packet, packet_size);

    return MDBUS_RC_ERR_UNK_FC;
}

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

uint8_t* mdbus_vm_buffer = 0;
void set_mdbus_vm_buffer(uint8_t* vm_buffer) {
    mdbus_vm_buffer = vm_buffer;
}
__attribute__((weak)) void mdbus_send(uint8_t* packet, uint8_t packet_size) {
    if (mdbus_vm_buffer != 0)
        memcpy(mdbus_vm_buffer, packet, packet_size);
}

uint8_t ltbus_slave_id = 0;
uint8_t* ltbus_config_buffer = 0;
uint8_t* ltbus_data_buffer = 0;
LTBUS_RC ltbus_init_device(uint8_t _slave_id, uint8_t* _config_buffer, uint8_t* _data_buffer) {
    ltbus_slave_id = _slave_id;
    ltbus_config_buffer = _config_buffer;
    ltbus_data_buffer = _data_buffer;
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
        out_conf->register_ptr = &(ltbus_config_buffer[register_offset]);
    else if (buffer_address == 0xD000)
        out_conf->register_ptr = &(ltbus_data_buffer[register_offset]);
    else
        return LTBUS_RC_ERR_UNK_DEVICE_BUFFER;

    return LTBUS_RC_OK;
}

LTBUS_RC ltbus_read_request(uint16_t address, uint16_t data_size, uint8_t* out_packet) {
    out_packet[0] = 0x7B;
    out_packet[1] = ltbus_slave_id;
    out_packet[2] = LTBUS_FC_READ;

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
    out_packet[1] = ltbus_slave_id;
    out_packet[2] = LTBUS_FC_WRITE;

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
    out_packet[11] = crc16 & 0xFF;
    out_packet[12] = (crc16 >> 8) & 0xFF;
    out_packet[13] = 0x7D;

    return LTBUS_RC_OK;
}

LTBUS_RC ltbus_write_u16_request(uint16_t address, uint16_t value, uint8_t* out_packet) {
    out_packet[0] = 0x7B;
    out_packet[1] = ltbus_slave_id;
    out_packet[2] = LTBUS_FC_WRITE;

    out_packet[3] = address & 0xFF;
    out_packet[4] = (address >> 8) & 0xFF;

    out_packet[5] = 0x02;
    out_packet[6] = 0x00;

    out_packet[7] = (uint8_t)(value & 0xFF);
    out_packet[8] = (uint8_t)((value >> 8) & 0xFF);

    uint16_t crc16 = ltbus_crc(out_packet, 9);
    out_packet[9] = crc16 & 0xFF;
    out_packet[10] = (crc16 >> 8) & 0xFF;
    out_packet[11] = 0x7D;

    return LTBUS_RC_OK;
}

LTBUS_RC ltbus_write_i16_request(uint16_t address, int16_t value, uint8_t* out_packet) {
    out_packet[0] = 0x7B;
    out_packet[1] = ltbus_slave_id;
    out_packet[2] = LTBUS_FC_WRITE;

    out_packet[3] = address & 0xFF;
    out_packet[4] = (address >> 8) & 0xFF;

    out_packet[5] = 0x02;
    out_packet[6] = 0x00;

    out_packet[7] = (uint8_t)(value & 0xFF);
    out_packet[8] = (uint8_t)((value >> 8) & 0xFF);

    uint16_t crc16 = ltbus_crc(out_packet, 9);
    out_packet[9] = crc16 & 0xFF;
    out_packet[10] = (crc16 >> 8) & 0xFF;
    out_packet[11] = 0x7D;

    return LTBUS_RC_OK;
}

uint16_t ltbus_crc(const uint8_t* data, uint16_t len) {
    uint16_t res = 0xFFFF;
    for (uint8_t i = 0; i < len; i++)
        res = (res >> 8) ^ CRC16_POLYNOMIAL[(res ^ data[i]) & 0xFF];
    return ~res;
}

uint8_t* ltbus_vm_buffer = 0;
void set_ltbus_vm_buffer(uint8_t* vm_buffer) {
    ltbus_vm_buffer = vm_buffer;
}
__attribute__((weak)) void ltbus_send(uint8_t* packet, uint8_t packet_size) {
    if (ltbus_vm_buffer != 0)
        memcpy(ltbus_vm_buffer, packet, packet_size);
}

LTBUS_RC ltbus_handle_read_request(const uint8_t* request_packet, uint8_t packet_size) {
    DeviceRegisterConfig dr_conf;
    uint8_t ddr_rc = ltbus_decode_device_register_config(request_packet, &dr_conf);
    if (ddr_rc != LTBUS_RC_OK)
        return ddr_rc;

    uint8_t read_resp_packet[LTBUS_MAX_TEMP_BUFFER];
    read_resp_packet[0] = '{';
    read_resp_packet[1] = ltbus_slave_id;
    read_resp_packet[2] = LTBUS_FC_READ_RESP;
    read_resp_packet[3] = request_packet[3];
    read_resp_packet[4] = request_packet[4];
    read_resp_packet[5] = request_packet[5];
    read_resp_packet[6] = request_packet[6];

    for (uint8_t i = 0; i < dr_conf.register_size; i++)
        read_resp_packet[LTBUS_PACKET_HEADER_SIZE + i] = dr_conf.register_ptr[i];

    uint16_t crc16 = ltbus_crc(read_resp_packet, LTBUS_PACKET_HEADER_SIZE + dr_conf.register_size);
    read_resp_packet[LTBUS_PACKET_HEADER_SIZE + dr_conf.register_size] = crc16 & 0xFF;
    read_resp_packet[LTBUS_PACKET_HEADER_SIZE + dr_conf.register_size + 1] = (crc16 >> 8) & 0xFF;
    read_resp_packet[LTBUS_PACKET_HEADER_SIZE + dr_conf.register_size + 2] = '}';
    ltbus_send(read_resp_packet, LTBUS_PACKET_HEADER_SIZE + LTBUS_PACKET_FOOTER_SIZE + dr_conf.register_size);
    return LTBUS_RC_OK;
}

LTBUS_RC ltbus_handle_write_request(const uint8_t* request_packet, uint8_t packet_size) {
    DeviceRegisterConfig dr_conf;
    uint8_t ddr_rc = ltbus_decode_device_register_config(request_packet, &dr_conf);
    if (ddr_rc != LTBUS_RC_OK)
        return ddr_rc;

    for (uint8_t i = 0; i < dr_conf.register_size; i++)
        dr_conf.register_ptr[i] = request_packet[LTBUS_PACKET_HEADER_SIZE + i];

    return LTBUS_RC_OK;
}

LTBUS_RC ltbus_handle_request(const uint8_t* request_packet, uint16_t packet_size) {
    if (packet_size < (LTBUS_PACKET_HEADER_SIZE + LTBUS_PACKET_FOOTER_SIZE))
        return LTBUS_RC_ERR_PKT_TOO_SMALL;

    // CRC-16 check
    uint16_t packet_crc16 = 0xFFFF;
    ((uint8_t*)&packet_crc16)[0] = request_packet[packet_size - 3];
    ((uint8_t*)&packet_crc16)[1] = request_packet[packet_size - 2];
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

LTBUS_RC ltbus_send_mmap(uint16_t mmap_size) {
    uint8_t out_packet[LTBUS_MMAP_MAX];

    out_packet[0] = 0x7B;
    out_packet[1] = ltbus_slave_id;
    out_packet[2] = LTBUS_FC_READ_RESP;

    out_packet[3] = 0x00;
    out_packet[4] = 0xD0;

    out_packet[5] = mmap_size & 0xFF;
    out_packet[6] = (mmap_size >> 8) & 0xFF;

    memcpy(out_packet + LTBUS_PACKET_HEADER_SIZE, ltbus_data_buffer, mmap_size);

    uint16_t crc16 = ltbus_crc(out_packet, LTBUS_PACKET_HEADER_SIZE + mmap_size);
    out_packet[LTBUS_PACKET_HEADER_SIZE + mmap_size] = crc16 & 0xFF;
    out_packet[LTBUS_PACKET_HEADER_SIZE + mmap_size + 1] = (crc16 >> 8) & 0xFF;
    out_packet[LTBUS_PACKET_HEADER_SIZE + mmap_size + 2] = 0x7D;

    ltbus_send(out_packet, LTBUS_PACKET_HEADER_SIZE + LTBUS_PACKET_FOOTER_SIZE + mmap_size);
    return LTBUS_RC_OK;
}
