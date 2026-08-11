import ctypes

from libzcom_ffi import _libs


def test_bit_manipulation():
    libzcom = _libs['libzcom.so']

    reg = 0b1101_1001
    reg = libzcom.set_bit(reg, 1)
    assert reg == 0b1101_1011

    reg = libzcom.clear_bit(reg, 4)
    assert reg == 0b1100_1011

    reg = libzcom.toggle_bit(reg, 3)
    assert reg == 0b1100_0011

    assert libzcom.check_bit(reg, 1) == 1
    assert libzcom.check_bit(reg, 2) == 0


def test_mdbus_rtu_crc():
    libzcom = _libs['libzcom.so']

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


def test_mdbus_handle_request():
    libzcom = _libs['libzcom.so']

    libzcom.mdbus_set_slave_id(0x01)
    mdbus_wreq = bytes([0x01, 0x10, 0x10, 0xC0, 0x00, 0x01, 0x02, 0xAA, 0xBB, 0x98, 0x42])
    mdbus_wreq_ptr = (ctypes.c_uint8 * len(mdbus_wreq)).from_buffer_copy(mdbus_wreq)

    libzcom.mdbus_handle_request(mdbus_wreq_ptr, len(mdbus_wreq))
    print("TODO")
