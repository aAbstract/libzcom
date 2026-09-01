import ctypes

from libzcom_mdbus_ffi import _libs


LIB_NAME = 'libzcom_mdbus.so'


def test_bit_manipulation():
    libzcom = _libs[LIB_NAME]

    reg = 0b1101_1001
    reg = libzcom.set_bit(reg, 1)
    assert reg == 0b1101_1011

    reg = libzcom.clear_bit(reg, 4)
    assert reg == 0b1100_1011

    reg = libzcom.toggle_bit(reg, 3)
    assert reg == 0b1100_0011

    assert libzcom.check_bit(reg, 1) == 1
    assert libzcom.check_bit(reg, 2) == 0


def test_mdbus_set_get_page():
    libzcom = _libs[LIB_NAME]
    MDBUS_RC_INV_PAGE_OFFSET = 0x15
    MDBUS_RC_PAGE_NOT_FOUND = 0x16
    MDBUS_RC_OK = 0x10

    mdbus_page_0x1 = (ctypes.c_uint16 * 0xFFF)()
    mdbus_rc = libzcom.mdbus_set_page(0xF + 1, mdbus_page_0x1)
    assert mdbus_rc == MDBUS_RC_INV_PAGE_OFFSET

    page_ptr = ctypes.POINTER(ctypes.c_uint16)()
    mdbus_rc = libzcom.mdbus_get_page(0x10A0, ctypes.byref(page_ptr))
    assert mdbus_rc == MDBUS_RC_PAGE_NOT_FOUND
    assert ctypes.cast(page_ptr, ctypes.c_void_p).value == None

    mdbus_rc = libzcom.mdbus_set_page(0x1, mdbus_page_0x1)
    assert mdbus_rc == MDBUS_RC_OK

    page_ptr = ctypes.POINTER(ctypes.c_uint16)()
    mdbus_rc = libzcom.mdbus_get_page(0x10A0, ctypes.byref(page_ptr))
    assert ctypes.cast(page_ptr, ctypes.c_void_p).value == ctypes.addressof(mdbus_page_0x1)


def test_mdbus_rtu_crc():
    libzcom = _libs[LIB_NAME]

    def test_sample(packet: bytes) -> bool:
        data = packet[:-2]
        target_crc = packet[-2:]
        data_ptr = (ctypes.c_uint8 * len(data)).from_buffer_copy(data)
        modbus_crc: int = libzcom.mdbus_rtu_crc(data_ptr, len(data))
        crc_bytes = modbus_crc.to_bytes(2, byteorder='little')
        return crc_bytes == target_crc

    assert test_sample(bytes([0x01, 0x03, 0x10, 0xC0, 0x00, 0x04, 0x40, 0xF5]))
    assert test_sample(bytes([0x01, 0x03, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x95, 0xD7]))
    assert test_sample(bytes([0x01, 0x04, 0x10, 0xA0, 0x00, 0x04, 0xF5, 0x2B]))
    assert test_sample(bytes([0x01, 0x04, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x0D]))
    assert test_sample(bytes([0x01, 0x10, 0x10, 0xC0, 0x00, 0x01, 0x02, 0xAA, 0xBB, 0x98, 0x42]))
    assert test_sample(bytes([0x01, 0x10, 0x10, 0xC5, 0x00, 0x02, 0x04, 0xAA, 0xBB, 0xCC, 0xDD, 0x36, 0xA4]))
    assert test_sample(bytes([0x01, 0x03, 0x10, 0xC0, 0x00, 0x02, 0xC0, 0xF7]))


def test_mdbus_mv_ld_word():
    libzcom = _libs[LIB_NAME]

    mdbus_page_0x1 = (ctypes.c_uint16 * 0xFFF)()
    libzcom.mdbus_set_page(0x1, mdbus_page_0x1)
    libzcom.mdbus_mv_word(0x10A0, 0xCCDD)

    word = ctypes.c_uint16(0)
    libzcom.mdbus_ld_word(0x10A0, ctypes.byref(word))
    assert word.value == 0xCCDD


def test_mdbus_mv_ld_i16():
    libzcom = _libs[LIB_NAME]

    mdbus_page_0x1 = (ctypes.c_uint16 * 0xFFF)()
    libzcom.mdbus_set_page(0x1, mdbus_page_0x1)
    libzcom.mdbus_mv_i16(0x10A0, -10)

    value_i16 = ctypes.c_int16(0)
    libzcom.mdbus_ld_i16(0x10A0, ctypes.byref(value_i16))
    value_u16 = ctypes.c_uint16(value_i16.value)
    assert value_i16.value == -10
    assert value_u16.value == 0xFFF6


def test_mdbus_mv_ld_u32():
    libzcom = _libs[LIB_NAME]

    mdbus_page_0x1 = (ctypes.c_uint16 * 0xFFF)()
    libzcom.mdbus_set_page(0x1, mdbus_page_0x1)
    libzcom.mdbus_mv_u32(0x10B0, 0xAABBCCDD)
    assert mdbus_page_0x1[0x0B0] == 0xCCDD
    assert mdbus_page_0x1[0x0B0 + 1] == 0xAABB

    value = ctypes.c_uint32(0)
    libzcom.mdbus_ld_u32(0x10B0, ctypes.byref(value))
    assert value.value == 0xAABBCCDD


def test_mdbus_mv_ld_i32():
    libzcom = _libs[LIB_NAME]

    mdbus_page_0x1 = (ctypes.c_uint16 * 0xFFF)()
    libzcom.mdbus_set_page(0x1, mdbus_page_0x1)
    libzcom.mdbus_mv_i32(0x10B0, -10)
    assert mdbus_page_0x1[0x0B0] == 0xFFF6
    assert mdbus_page_0x1[0x0B0 + 1] == 0xFFFF

    value = ctypes.c_int32(0)
    libzcom.mdbus_ld_i32(0x10B0, ctypes.byref(value))
    assert value.value == -10


def test_mdbus_mv_ld_f32():
    libzcom = _libs[LIB_NAME]

    mdbus_page_0x1 = (ctypes.c_uint16 * 0xFFF)()
    libzcom.mdbus_set_page(0x1, mdbus_page_0x1)
    libzcom.mdbus_mv_f32(0x10B0, 12.34)
    assert mdbus_page_0x1[0x0B0] == 0x70A4
    assert mdbus_page_0x1[0x0B0 + 1] == 0x4145

    value = ctypes.c_float(0)
    libzcom.mdbus_ld_f32(0x10B0, ctypes.byref(value))
    assert round(value.value, 2) == 12.34


def test_mdbus_encode_read_holding_regs():
    libzcom = _libs[LIB_NAME]

    libzcom.mdbus_set_slave_id(0x01)
    out_packet = (ctypes.c_uint8 * 8)()
    target_packet = bytes([0x01, 0x03, 0x10, 0xC0, 0x00, 0x02, 0xC0, 0xF7])
    libzcom.mdbus_encode_read_holding_regs(0x10C0, 2, out_packet)
    assert bytes(out_packet) == target_packet


def test_mdbus_encode_read_input_regs():
    libzcom = _libs[LIB_NAME]

    libzcom.mdbus_set_slave_id(0x01)
    out_packet = (ctypes.c_uint8 * 8)()
    target_packet = bytes([0x01, 0x04, 0x10, 0xC6, 0x00, 0x04, 0x15, 0x34])
    libzcom.mdbus_encode_read_input_regs(0x10C6, 4, out_packet)
    assert bytes(out_packet) == target_packet


def test_mdbus_encode_write_regs():
    libzcom = _libs[LIB_NAME]

    libzcom.mdbus_set_slave_id(0x01)
    out_packet = (ctypes.c_uint8 * 13)()
    target_packet = bytes([
        0x01, 0x10, 0x10, 0xC0,
        0x00, 0x02, 0x04,
        0x70, 0xA4, 0x41, 0x45,
        0x98, 0xBF,
    ])

    word_list = (ctypes.c_uint16 * 2)(0x70A4, 0x4145)
    libzcom.mdbus_encode_write_regs(0x10C0, word_list, len(word_list), out_packet)
    assert bytes(out_packet) == target_packet


def test_mdbus_u32_words():
    libzcom = _libs[LIB_NAME]

    out_words = (ctypes.c_uint16 * 2)(0x0000, 0x0000)
    libzcom.mdbus_u32_words(0xAABBCCDD, out_words)
    assert out_words[0] == 0xCCDD
    assert out_words[1] == 0xAABB


def test_mdbus_i32_words():
    libzcom = _libs[LIB_NAME]

    out_words = (ctypes.c_uint16 * 2)(0x0000, 0x0000)
    libzcom.mdbus_i32_words(-10, out_words)
    assert out_words[0] == 0xFFF6
    assert out_words[1] == 0xFFFF


def test_mdbus_f32_words():
    libzcom = _libs[LIB_NAME]

    out_words = (ctypes.c_uint16 * 2)(0x0000, 0x0000)
    libzcom.mdbus_f32_words(12.34, out_words)
    assert out_words[0] == 0x70A4
    assert out_words[1] == 0x4145


def test_mdbus_handle_write_request():
    libzcom = _libs[LIB_NAME]

    libzcom.mdbus_set_slave_id(0x01)
    mdbus_page_0x1 = (ctypes.c_uint16 * 0xFFF)()
    libzcom.mdbus_set_page(0x1, mdbus_page_0x1)

    mdbus_wreq = bytes([0x01, 0x10, 0x10, 0xC0, 0x00, 0x01, 0x02, 0xAA, 0xBB, 0x98, 0x42])
    mdbus_wreq_ptr = (ctypes.c_uint8 * len(mdbus_wreq)).from_buffer_copy(mdbus_wreq)
    libzcom.mdbus_handle_request(mdbus_wreq_ptr, len(mdbus_wreq))
    assert mdbus_page_0x1[0x10C0 & 0x0FFF] == 0xAABB

    mdbus_wreq = bytes([0x01, 0x10, 0x10, 0xC5, 0x00, 0x02, 0x04, 0xAA, 0xBB, 0xCC, 0xDD, 0x36, 0xA4])
    mdbus_wreq_ptr = (ctypes.c_uint8 * len(mdbus_wreq)).from_buffer_copy(mdbus_wreq)
    libzcom.mdbus_handle_request(mdbus_wreq_ptr, len(mdbus_wreq))
    assert mdbus_page_0x1[0x10C5 & 0x0FFF] == 0xAABB
    assert mdbus_page_0x1[(0x10C5 & 0x0FFF) + 1] == 0xCCDD


def test_mdbus_handle_request():
    libzcom = _libs[LIB_NAME]

    libzcom.mdbus_set_slave_id(0x01)
    mdbus_page_0x1 = (ctypes.c_uint16 * 0xFFF)()
    libzcom.mdbus_set_page(0x1, mdbus_page_0x1)

    f32_words_1 = (ctypes.c_uint16 * 2)(0x0000, 0x0000)
    f32_words_2 = (ctypes.c_uint16 * 2)(0x0000, 0x0000)
    f32_words_3 = (ctypes.c_uint16 * 2)(0x0000, 0x0000)
    f32_words_4 = (ctypes.c_uint16 * 2)(0x0000, 0x0000)
    libzcom.mdbus_f32_words(11.11, f32_words_1)
    libzcom.mdbus_f32_words(22.22, f32_words_2)
    libzcom.mdbus_f32_words(33.33, f32_words_3)
    libzcom.mdbus_f32_words(44.44, f32_words_4)
    assert f32_words_1[0] == 0xC28F
    assert f32_words_1[1] == 0x4131
    assert f32_words_2[0] == 0xC28F
    assert f32_words_2[1] == 0x41B1
    assert f32_words_3[0] == 0x51EC
    assert f32_words_3[1] == 0x4205
    assert f32_words_4[0] == 0xC28F
    assert f32_words_4[1] == 0x4231

    f32_word_list = (ctypes.c_uint16 * 8)(
        *(list(f32_words_1) +
          list(f32_words_2) +
          list(f32_words_3) +
          list(f32_words_4))
    )
    mdbus_wreq = (ctypes.c_uint8 * 25)()
    libzcom.mdbus_encode_write_regs(0x10A0, f32_word_list, len(f32_word_list), mdbus_wreq)
    target_packet = bytes([
        0x01, 0x10, 0x10, 0xA0,
        0x00, 0x08, 0x10,
        0xC2, 0x8F, 0x41, 0x31,
        0xC2, 0x8F, 0x41, 0xB1,
        0x51, 0xEC, 0x42, 0x05,
        0xC2, 0x8F, 0x42, 0x31,
        0x50, 0xAD,
    ])
    assert bytes(mdbus_wreq) == target_packet

    mdbus_wreq_ptr = (ctypes.c_uint8 * len(mdbus_wreq)).from_buffer_copy(mdbus_wreq)
    libzcom.mdbus_handle_request(mdbus_wreq_ptr, len(mdbus_wreq))

    f32_1 = ctypes.c_float(0)
    f32_2 = ctypes.c_float(0)
    f32_3 = ctypes.c_float(0)
    f32_4 = ctypes.c_float(0)
    libzcom.mdbus_ld_f32(0x10A0, ctypes.byref(f32_1))
    libzcom.mdbus_ld_f32(0x10A2, ctypes.byref(f32_2))
    libzcom.mdbus_ld_f32(0x10A4, ctypes.byref(f32_3))
    libzcom.mdbus_ld_f32(0x10A6, ctypes.byref(f32_4))
    assert round(f32_1.value, 2) == 11.11
    assert round(f32_2.value, 2) == 22.22
    assert round(f32_3.value, 2) == 33.33
    assert round(f32_4.value, 2) == 44.44

    mdbus_tx_buffer = (ctypes.c_uint8 * 0xFF)()
    libzcom.mdbus_set_tx_buffer(mdbus_tx_buffer)
    mdbus_rreq = (ctypes.c_uint8 * 8)()
    libzcom.mdbus_encode_read_holding_regs(0x10A0, 8, mdbus_rreq)
    target_packet = bytes([0x01, 0x03, 0x10, 0xA0, 0x00, 0x08, 0x40, 0xEE])
    assert bytes(mdbus_rreq) == target_packet
    mdbus_rreq_ptr = (ctypes.c_uint8 * len(mdbus_rreq)).from_buffer_copy(mdbus_rreq)

    assert mdbus_tx_buffer[:21] == [0] * 21
    libzcom.mdbus_handle_request(mdbus_rreq_ptr, len(mdbus_rreq))

    target_resp_packet = [
        0x01, 0x03, 0x10,
        0xC2, 0x8F, 0x41, 0x31,
        0xC2, 0x8F, 0x41, 0xB1,
        0x51, 0xEC, 0x42, 0x05,
        0xC2, 0x8F, 0x42, 0x31,
        0x27, 0x11,
    ]
    assert mdbus_tx_buffer[:21] == target_resp_packet
