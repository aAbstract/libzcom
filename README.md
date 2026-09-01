# libzcom

```
libzcom

Author: Eslam Elsharkawy
Version: 1.1.0
Date: 2026-09-01
```

## ModBus API Reference
```c
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
```

## LTBus API Reference - TODO

## Testing - Coverage

#### ModBus Virtual Memory System
- `mdbus_set_slave_id` - ✅
- `mdbus_set_page` - ✅
- `mdbus_get_page` - ✅

#### ModBus Source Operations
- `mdbus_mv_word` - ✅
- `mdbus_ld_word` - ✅
- `mdbus_mv_i16` - ✅
- `mdbus_ld_i16` - ✅
- `mdbus_mv_u32` - ✅
- `mdbus_ld_u32` - ✅
- `mdbus_mv_i32` - ✅
- `mdbus_ld_i32` - ✅
- `mdbus_mv_f32` - ✅
- `mdbus_ld_f32` - ✅

#### ModBus Codecs
- `mdbus_encode_read_holding_regs` - ✅
- `mdbus_encode_read_input_regs` - ✅
- `mdbus_encode_write_regs` - ✅

#### ModBus Utils
- `mdbus_rtu_crc` - ✅
- `mdbus_u32_words` - ✅
- `mdbus_i32_words` - ✅
- `mdbus_f32_words` - ✅
- `mdbus_set_tx_buffer` - ✅
- `mdbus_transmit` - ✅
- `set_bit` - ✅
- `clear_bit` - ✅
- `toggle_bit` - ✅
- `check_bit` - ✅

#### ModBus Request Handler
- `mdbus_handle_request` - ✅

## Testing - Docs - TODO
