#ifndef LIBZCOM_MDBUS_H
#define LIBZCOM_MDBUS_H

#include <stdint.h>
#include <string.h>

#define MDBUS_FC_READ_HOLDING_REGS 0x03
#define MDBUS_FC_READ_INPUT_REGS 0x04
#define MDBUS_FC_WRITE_REGS 0x10
#define MDBUS_RC uint8_t
#define MDBUS_RC_OK 0x10
#define MDBUS_RC_ERR_PKT_TOO_SMALL 0x11
#define MDBUS_RC_ERR_INV_CRC16 0x12
#define MDBUS_RC_ERR_SLV_ID_MISMATCH 0x13
#define MDBUS_RC_ERR_UNK_FC 0x14
#define MDBUS_RC_INV_PAGE_OFFSET 0x15
#define MDBUS_RC_PAGE_NOT_FOUND 0x16

// modbus-virtual-memory
MDBUS_RC mdbus_set_slave_id(uint8_t _slave_id);
MDBUS_RC mdbus_set_page(uint8_t page_offset, uint16_t* page_ptr);
MDBUS_RC mdbus_get_page(uint16_t address, uint16_t** out_page_ptr);

// mdbus-source-ops
MDBUS_RC mdbus_mv_word(uint16_t address, uint16_t word);
MDBUS_RC mdbus_ld_word(uint16_t address, uint16_t* out_word);
MDBUS_RC mdbus_mv_i16(uint16_t address, int16_t value);
MDBUS_RC mdbus_ld_i16(uint16_t address, int16_t* out_value);
MDBUS_RC mdbus_mv_u32(uint16_t address, uint32_t value);
MDBUS_RC mdbus_ld_u32(uint16_t address, uint32_t* out_value);
MDBUS_RC mdbus_mv_i32(uint16_t address, int32_t value);
MDBUS_RC mdbus_ld_i32(uint16_t address, int32_t* out_value);
MDBUS_RC mdbus_mv_f32(uint16_t address, float value);
MDBUS_RC mdbus_ld_f32(uint16_t address, float* out_value);

// mdbus-codecs
MDBUS_RC mdbus_encode_read_holding_regs(uint16_t address, uint16_t word_cnt, uint8_t* out_packet);
MDBUS_RC mdbus_encode_read_input_regs(uint16_t address, uint16_t word_cnt, uint8_t* out_packet);
MDBUS_RC mdbus_encode_write_regs(uint16_t address, uint16_t* word_list, uint16_t word_cnt, uint8_t* out_packet);

// mdbus-utils
uint16_t mdbus_rtu_crc(const uint8_t* data, uint16_t len);
void mdbus_u32_words(uint32_t value, uint16_t* out_words);
void mdbus_i32_words(int32_t value, uint16_t* out_words);
void mdbus_f32_words(float value, uint16_t* out_words);
void mdbus_set_tx_buffer(uint8_t* tx_buffer);
void mdbus_transmit(uint8_t* packet, uint8_t packet_size);
uint16_t set_bit(uint16_t x, uint8_t pos);
uint16_t clear_bit(uint16_t x, uint8_t pos);
uint16_t toggle_bit(uint16_t x, uint8_t pos);
uint16_t check_bit(uint16_t x, uint8_t pos);

// mdbus-request-handler
MDBUS_RC mdbus_handle_request(const uint8_t* request_packet, uint16_t packet_size);

#endif
