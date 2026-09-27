import struct
import ctypes

from libzcom_ltbus_ffi import _libs


LIB_NAME = 'libzcom_ltbus.so'


def test_bit_manipulation():
    libzcom = _libs[LIB_NAME]

    reg = 0b1101_1001
    reg = libzcom.libzcom_set_bit(reg, 1)
    assert reg == 0b1101_1011

    reg = libzcom.libzcom_clear_bit(reg, 4)
    assert reg == 0b1100_1011

    reg = libzcom.libzcom_toggle_bit(reg, 3)
    assert reg == 0b1100_0011

    assert libzcom.libzcom_check_bit(reg, 1) == 1
    assert libzcom.libzcom_check_bit(reg, 2) == 0


def test_ltbus_set_get_page():
    libzcom = _libs[LIB_NAME]
    LTBUS_RC_INV_PAGE_OFFSET = 0x07
    LTBUS_RC_PAGE_NOT_FOUND = 0x08
    LTBUS_RC_OK = 0x00

    ltbus_page_0xD = (ctypes.c_uint8 * 0xFFF)()
    ltbus_rc = libzcom.ltbus_set_page(0xF + 1, ltbus_page_0xD)
    assert ltbus_rc == LTBUS_RC_INV_PAGE_OFFSET

    page_ptr = ctypes.POINTER(ctypes.c_uint8)()
    ltbus_rc = libzcom.ltbus_get_page(0xD0A0, ctypes.byref(page_ptr))
    assert ltbus_rc == LTBUS_RC_PAGE_NOT_FOUND
    assert ctypes.cast(page_ptr, ctypes.c_void_p).value == None

    ltbus_rc = libzcom.ltbus_set_page(0xD, ltbus_page_0xD)
    assert ltbus_rc == LTBUS_RC_OK

    page_ptr = ctypes.POINTER(ctypes.c_uint8)()
    ltbus_rc = libzcom.ltbus_get_page(0xD0A0, ctypes.byref(page_ptr))
    assert ctypes.cast(page_ptr, ctypes.c_void_p).value == ctypes.addressof(ltbus_page_0xD)


def test_ltbus_crc():
    libzcom = _libs[LIB_NAME]

    def test_sample(packet: bytes) -> bool:
        data = packet[:-3]
        target_crc = packet[-3:-1]
        data_ptr = (ctypes.c_uint8 * len(data)).from_buffer_copy(data)
        ltbus_crc: int = libzcom.ltbus_crc(data_ptr, len(data))
        crc_bytes = ltbus_crc.to_bytes(2, byteorder='little')
        return crc_bytes == target_crc

    assert test_sample(bytes([0x7B, 0x01, 0xAA, 0x10, 0xD0, 0x04, 0x00, 0x37, 0x62, 0x7D]))
    assert test_sample(bytes([0x7B, 0x01, 0xEA, 0x7C, 0xD0, 0x04, 0x00, 0xD7, 0xA3, 0xB2, 0x41, 0x41, 0x4C, 0x7D]))
    assert test_sample(bytes([0x7B, 0x01, 0xAA, 0x7C, 0xD0, 0x04, 0x00, 0xE7, 0x6C, 0x7D]))
    assert test_sample(bytes([
        0x7B, 0x01, 0xEA,
        0x7C, 0xD0, 0x10, 0x00,
        0x8F, 0xC2, 0x31, 0x41,
        0x8F, 0xC2, 0xB1, 0x41,
        0xEC, 0x51, 0x05, 0x42,
        0x8F, 0xC2, 0x31, 0x42,
        0x91, 0x06, 0x7D,
    ]))
    assert test_sample(bytes([
        0x7B, 0x01, 0xAA,
        0x7C, 0xD0, 0x10, 0x00,
        0x16, 0x9E, 0x7D,
    ]))
    assert test_sample(bytes([
        0x7B, 0x01, 0xAB,
        0x7C, 0xD0, 0x10, 0x00,
        0x8F, 0xC2, 0x31, 0x41,
        0x8F, 0xC2, 0xB1, 0x41,
        0xEC, 0x51, 0x05, 0x42,
        0x8F, 0xC2, 0x31, 0x42,
        0x20, 0x82, 0x7D,
    ]))


def test_ltbus_encode_read_regs():
    libzcom = _libs[LIB_NAME]

    libzcom.ltbus_set_slave_id(0x01)
    out_packet = (ctypes.c_uint8 * 10)()
    libzcom.ltbus_encode_read_regs(0xD010, 4, out_packet)
    target_packet = bytes([0x7B, 0x01, 0xAA, 0x10, 0xD0, 0x04, 0x00, 0x37, 0x62, 0x7D])
    assert bytes(out_packet) == target_packet


def test_ltbus_encode_write_regs():
    libzcom = _libs[LIB_NAME]

    libzcom.ltbus_set_slave_id(0x01)
    out_packet = (ctypes.c_uint8 * 14)()
    data_buffer = (ctypes.c_uint8 * 4).from_buffer_copy(struct.pack('<f', 22.33))
    libzcom.ltbus_encode_write_regs(0xD07C, data_buffer, len(data_buffer), out_packet)
    target_packet = bytes([0x7B, 0x01, 0xEA, 0x7C, 0xD0, 0x04, 0x00, 0xD7, 0xA3, 0xB2, 0x41, 0x41, 0x4C, 0x7D])
    assert bytes(out_packet) == target_packet


def test_ltbus_native_source_operations():
    libzcom = _libs[LIB_NAME]

    ltbus_page_0xD = (ctypes.c_uint8 * 0xFFF)()
    libzcom.ltbus_set_page(0xD, ltbus_page_0xD)

    float_offset = 0x014
    float_ptr = ctypes.cast(
        ctypes.byref(ltbus_page_0xD, float_offset),
        ctypes.POINTER(ctypes.c_float)
    )
    float_ptr[0] = 12.34
    assert bytes(ltbus_page_0xD[float_offset:float_offset + 4]) == struct.pack('<f', 12.34)

    i16_offset = 0x004
    i16_ptr = ctypes.cast(
        ctypes.byref(ltbus_page_0xD, i16_offset),
        ctypes.POINTER(ctypes.c_int16)
    )
    i16_ptr[0] = -100
    assert bytes(ltbus_page_0xD[i16_offset:i16_offset + 2]) == struct.pack('<h', -100)


def test_ltbus_handle_write_request():
    libzcom = _libs[LIB_NAME]

    libzcom.ltbus_set_slave_id(0x01)
    ltbus_page_0xD = (ctypes.c_uint8 * 0xFFF)()
    libzcom.ltbus_set_page(0xD, ltbus_page_0xD)

    ltbus_wreq = bytes([0x7B, 0x01, 0xEA, 0x7C, 0xD0, 0x04, 0x00, 0xD7, 0xA3, 0xB2, 0x41, 0x41, 0x4C, 0x7D])
    ltbus_wreq_ptr = (ctypes.c_uint8 * len(ltbus_wreq)).from_buffer_copy(ltbus_wreq)
    libzcom.ltbus_handle_request(ltbus_wreq_ptr, len(ltbus_wreq))
    register_offset = 0xD07C & 0x0FFF
    assert ltbus_page_0xD[register_offset:register_offset + 4] == [0xD7, 0xA3, 0xB2, 0x41]
    assert bytes(ltbus_page_0xD[register_offset:register_offset + 4]) == struct.pack('<f', 22.33)


def test_ltbus_handle_request():
    libzcom = _libs[LIB_NAME]

    libzcom.ltbus_set_slave_id(0x01)
    ltbus_page_0xD = (ctypes.c_uint8 * 0xFFF)()
    libzcom.ltbus_set_page(0xD, ltbus_page_0xD)

    f32_buffer = b''.join([struct.pack('<f', x) for x in [11.11, 22.22, 33.33, 44.44]])
    data_buffer = (ctypes.c_uint8 * len(f32_buffer)).from_buffer_copy(f32_buffer)
    ltbus_wreq = (ctypes.c_uint8 * 26)()
    libzcom.ltbus_encode_write_regs(0xD07C, data_buffer, len(data_buffer), ltbus_wreq)

    target_packet = bytes([
        0x7B, 0x01, 0xEA,
        0x7C, 0xD0, 0x10, 0x00,
    ])
    target_packet += f32_buffer
    target_packet += bytes([0x91, 0x06, 0x7D])
    assert bytes(ltbus_wreq) == target_packet

    ltbus_wreq_ptr = (ctypes.c_uint8 * len(ltbus_wreq)).from_buffer_copy(ltbus_wreq)
    libzcom.ltbus_handle_request(ltbus_wreq_ptr, len(ltbus_wreq))
    register_offset = 0xD07C & 0x0FFF
    assert bytes(ltbus_page_0xD[register_offset:register_offset + 16]) == f32_buffer

    ltbus_tx_buffer = (ctypes.c_uint8 * 0xFF)()
    libzcom.ltbus_set_tx_buffer(ltbus_tx_buffer)
    ltbus_rreq = (ctypes.c_uint8 * 10)()
    libzcom.ltbus_encode_read_regs(0xD07C, 16, ltbus_rreq)
    target_packet = bytes([
        0x7B, 0x01, 0xAA,
        0x7C, 0xD0, 0x10, 0x00,
        0x16, 0x9E, 0x7D,
    ])
    assert bytes(ltbus_rreq) == target_packet

    ltbus_rreq_ptr = (ctypes.c_uint8 * len(ltbus_rreq)).from_buffer_copy(ltbus_rreq)
    tx_packet_size = 26
    assert ltbus_tx_buffer[:tx_packet_size] == [0] * tx_packet_size
    libzcom.ltbus_handle_request(ltbus_rreq_ptr, len(ltbus_rreq))

    target_resp_packet = bytes([
        0x7B, 0x01, 0xAB,
        0x7C, 0xD0, 0x10, 0x00,
    ])
    target_resp_packet += f32_buffer
    target_resp_packet += bytes([0x20, 0x82, 0x7D])
    assert bytes(ltbus_tx_buffer[:tx_packet_size]) == target_resp_packet
