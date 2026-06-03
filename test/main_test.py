import struct
import ctypes
import test.pyt_lib as pyt_lib


def test_bit_manipulation():
    libzcom = pyt_lib.load_libzcom_ffi()

    reg = 0b1101_1001
    reg = libzcom.set_bit(reg, 1)
    assert reg == 0b1101_1011

    reg = libzcom.clear_bit(reg, 4)
    assert reg == 0b1100_1011

    reg = libzcom.toggle_bit(reg, 3)
    assert reg == 0b1100_0011

    assert libzcom.check_bit(reg, 1) == 1
    assert libzcom.check_bit(reg, 2) == 0


def test_modbus_rtu_crc():
    libzcom = pyt_lib.load_libzcom_ffi()

    def test_sample(packet: bytes) -> bool:
        data = packet[:-2]
        target_crc = packet[-2:]
        data_ptr = (ctypes.c_uint8 * len(data)).from_buffer_copy(data)
        modbus_crc: int = libzcom.modbus_rtu_crc(data_ptr, len(data))
        crc_bytes = modbus_crc.to_bytes(2, byteorder='little')
        return crc_bytes == target_crc

    assert test_sample(bytes([0x03, 0x10, 0x00, 0x00, 0x00, 0x01, 0x02, 0x00, 0x28, 0xBF, 0x2E]))
    assert test_sample(bytes([0x03, 0x10, 0x00, 0x00, 0x00, 0x01, 0x00, 0x2B]))


def test_ltbus_read_request():
    libzcom = pyt_lib.load_libzcom_ffi()
    libzcom.ltbus_init_device(0x01, (ctypes.c_uint8 * 0xF)(), (ctypes.c_uint8 * 0xF)())
    out_packet = (ctypes.c_uint8 * 10)()
    target_packet = bytes([0x7B, 0x01, 0xAA, 0x04, 0xD0, 0x04, 0x00, 0x7A, 0xD3, 0x7D])
    libzcom.ltbus_read_request(0xD004, 4, out_packet)
    assert bytes(out_packet) == target_packet


def test_ltbus_decode_device_register_config():
    libzcom = pyt_lib.load_libzcom_ffi()

    # init ltbus buffers
    ltbus_config_buffer = (ctypes.c_uint8 * 0xFFF)()
    ltbus_data_buffer = (ctypes.c_uint8 * 0xFFF)()
    libzcom.ltbus_init_device(0x01, ltbus_config_buffer, ltbus_data_buffer)

    out_conf = pyt_lib.DeviceRegisterConfig()
    request_packet = bytes([0x7B, 0x01, 0xAA, 0x04, 0xD0, 0x04, 0x00, 0x7A, 0xD3, 0x7D])  # RFR 0xD004 F32
    req_packet_ptr = (ctypes.c_uint8 * len(request_packet)).from_buffer_copy(request_packet)
    ltbus_rc = libzcom.ltbus_decode_device_register_config(req_packet_ptr, ctypes.byref(out_conf))

    assert ltbus_rc == 0
    assert out_conf.register_address == 0xD004
    assert out_conf.register_size == 4
    assert ctypes.cast(out_conf.register_ptr, ctypes.c_void_p).value - ctypes.addressof(ltbus_data_buffer) == out_conf.register_size


def test_tustin_filter():
    libzcom = pyt_lib.load_libzcom_ffi()
    _P_Tustin_Filter_T_ = pyt_lib.P_Tustin_Filter_T_.in_dll(libzcom, 'Tustin_Filter_P')

    num_coef = list(_P_Tustin_Filter_T_.DiscreteTransferFcn_NumCoef)
    assert num_coef == [0.01547, 0.01547]

    den_coef = list(_P_Tustin_Filter_T_.DiscreteTransferFcn_DenCoef)
    assert den_coef == [1, -0.9691]


def test_ltbus_write_f32_request():
    libzcom = pyt_lib.load_libzcom_ffi()
    libzcom.ltbus_init_device(0x01, (ctypes.c_uint8 * 0xF)(), (ctypes.c_uint8 * 0xF)())
    out_packet = (ctypes.c_uint8 * 14)()
    target_packet = bytes([0x7B, 0x01, 0xEA, 0x00, 0xD0, 0x04, 0x00, 0xA4, 0x70, 0x45, 0x41, 0xB9, 0xD1, 0x7D])
    libzcom.ltbus_write_f32_request(0xD000, 12.34, out_packet)
    assert bytes(out_packet) == target_packet


def test_ltbus_handle_request():
    libzcom = pyt_lib.load_libzcom_ffi()

    # init ltbus buffers
    ltbus_config_buffer = (ctypes.c_uint8 * 0xFFF)()
    ltbus_data_buffer = (ctypes.c_uint8 * 0xFFF)()
    libzcom.ltbus_init_device(0x01, ltbus_config_buffer, ltbus_data_buffer)

    ltbr_wreq = (ctypes.c_uint8 * 14)()
    libzcom.ltbus_write_f32_request(0xD000, 12.34, ltbr_wreq)
    libzcom.ltbus_handle_request(ltbr_wreq, 14)

    echo_value: float = struct.unpack('<f', bytes(ltbus_data_buffer[:4]))[0]
    assert round(echo_value, 2) == 12.34

    ltbus_vm_buffer = (ctypes.c_uint8 * 0xFFF)()
    libzcom.set_vm_buffer(ltbus_vm_buffer)
    ltbr_rreq = (ctypes.c_uint8 * 10)()
    libzcom.ltbus_read_request(0xD000, 4, ltbr_rreq)
    libzcom.ltbus_handle_request(ltbr_rreq, 10)

    assert ltbus_vm_buffer[13] == 0x7D
    assert ltbus_vm_buffer[:11] == [0x7B, 0x01, 0xAB, 0x00, 0xD0, 0x04, 0x00, 0xA4, 0x70, 0x45, 0x41]
