#include <stdint.h>

uint16_t libzcom_set_bit(uint16_t x, uint8_t pos) {
    return x | (1 << pos);
}

uint16_t libzcom_clear_bit(uint16_t x, uint8_t pos) {
    return (x & ~(1 << pos));
}

uint16_t libzcom_toggle_bit(uint16_t x, uint8_t pos) {
    return x ^ (1 << pos);
}

uint16_t libzcom_check_bit(uint16_t x, uint8_t pos) {
    return (x & (1 << pos)) != 0;
}
