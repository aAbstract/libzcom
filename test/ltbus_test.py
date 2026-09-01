# import struct
# import ctypes

# from libzcom_ffi import _libs, DeviceRegisterConfig


# def test_bit_manipulation():
#     libzcom = _libs['libzcom.so']

#     reg = 0b1101_1001
#     reg = libzcom.set_bit(reg, 1)
#     assert reg == 0b1101_1011

#     reg = libzcom.clear_bit(reg, 4)
#     assert reg == 0b1100_1011

#     reg = libzcom.toggle_bit(reg, 3)
#     assert reg == 0b1100_0011

#     assert libzcom.check_bit(reg, 1) == 1
#     assert libzcom.check_bit(reg, 2) == 0


# def test_ltbus_read_request():
#     libzcom = _libs['libzcom.so']
#     libzcom.ltbus_init_device(0x01, (ctypes.c_uint8 * 0xF)(), (ctypes.c_uint8 * 0xF)())
#     out_packet = (ctypes.c_uint8 * 10)()
#     target_packet = bytes([0x7B, 0x01, 0xAA, 0x04, 0xD0, 0x04, 0x00, 0x7A, 0xD3, 0x7D])
#     libzcom.ltbus_read_request(0xD004, 4, out_packet)
#     assert bytes(out_packet) == target_packet


# def test_ltbus_decode_device_register_config():
#     libzcom = _libs['libzcom.so']

#     # init ltbus buffers
#     ltbus_config_buffer = (ctypes.c_uint8 * 0xFFF)()
#     ltbus_data_buffer = (ctypes.c_uint8 * 0xFFF)()
#     libzcom.ltbus_init_device(0x01, ltbus_config_buffer, ltbus_data_buffer)

#     out_conf = DeviceRegisterConfig()
#     request_packet = bytes([0x7B, 0x01, 0xAA, 0x04, 0xD0, 0x04, 0x00, 0x7A, 0xD3, 0x7D])  # RFR 0xD004 F32
#     req_packet_ptr = (ctypes.c_uint8 * len(request_packet)).from_buffer_copy(request_packet)
#     ltbus_rc = libzcom.ltbus_decode_device_register_config(req_packet_ptr, ctypes.byref(out_conf))

#     assert ltbus_rc == 0
#     assert out_conf.register_address == 0xD004
#     assert out_conf.register_size == 4
#     assert ctypes.cast(out_conf.register_ptr, ctypes.c_void_p).value - ctypes.addressof(ltbus_data_buffer) == out_conf.register_size


# def test_ltbus_write_f32_request():
#     libzcom = _libs['libzcom.so']
#     libzcom.ltbus_init_device(0x01, (ctypes.c_uint8 * 0xF)(), (ctypes.c_uint8 * 0xF)())
#     out_packet = (ctypes.c_uint8 * 14)()
#     target_packet = bytes([0x7B, 0x01, 0xEA, 0x00, 0xD0, 0x04, 0x00, 0xA4, 0x70, 0x45, 0x41, 0xB9, 0xD1, 0x7D])
#     libzcom.ltbus_write_f32_request(0xD000, 12.34, out_packet)
#     assert bytes(out_packet) == target_packet


# def test_ltbus_handle_request():
#     libzcom = _libs['libzcom.so']

#     # init ltbus buffers
#     ltbus_config_buffer = (ctypes.c_uint8 * 0xFFF)()
#     ltbus_data_buffer = (ctypes.c_uint8 * 0xFFF)()
#     libzcom.ltbus_init_device(0x01, ltbus_config_buffer, ltbus_data_buffer)

#     ltbr_wreq = (ctypes.c_uint8 * 14)()
#     libzcom.ltbus_write_f32_request(0xD000, 12.34, ltbr_wreq)
#     libzcom.ltbus_handle_request(ltbr_wreq, 14)

#     echo_value: float = struct.unpack('<f', bytes(ltbus_data_buffer[:4]))[0]
#     assert round(echo_value, 2) == 12.34

#     ltbus_vm_buffer = (ctypes.c_uint8 * 0xFFF)()
#     libzcom.set_vm_buffer(ltbus_vm_buffer)
#     ltbr_rreq = (ctypes.c_uint8 * 10)()
#     libzcom.ltbus_read_request(0xD000, 4, ltbr_rreq)
#     libzcom.ltbus_handle_request(ltbr_rreq, 10)

#     assert ltbus_vm_buffer[13] == 0x7D
#     assert ltbus_vm_buffer[:11] == [0x7B, 0x01, 0xAB, 0x00, 0xD0, 0x04, 0x00, 0xA4, 0x70, 0x45, 0x41]


# def test_ltbus_send_mmap():
#     libzcom = _libs['libzcom.so']

#     # init ltbus buffers
#     ltbus_config_buffer = (ctypes.c_uint8 * 0xFFF)()
#     ltbus_data_buffer = (ctypes.c_uint8 * 0xFFF)()
#     libzcom.ltbus_init_device(0x01, ltbus_config_buffer, ltbus_data_buffer)

#     ltbr_wreq_1 = (ctypes.c_uint8 * 14)()
#     ltbr_wreq_2 = (ctypes.c_uint8 * 14)()
#     ltbr_wreq_3 = (ctypes.c_uint8 * 12)()
#     libzcom.ltbus_write_f32_request(0xD000, 12.34, ltbr_wreq_1)
#     libzcom.ltbus_write_f32_request(0xD004, 34.56, ltbr_wreq_2)
#     libzcom.ltbus_write_u16_request(0xD008, 0b1010_0000_1010_1111, ltbr_wreq_3)
#     libzcom.ltbus_handle_request(ltbr_wreq_1, 14)
#     libzcom.ltbus_handle_request(ltbr_wreq_2, 14)
#     libzcom.ltbus_handle_request(ltbr_wreq_3, 12)

#     echo_value_1: float = struct.unpack('<f', bytes(ltbus_data_buffer[:4]))[0]
#     echo_value_2: float = struct.unpack('<f', bytes(ltbus_data_buffer[4:8]))[0]
#     echo_value_3_bytes: bytes = bytes(ltbus_data_buffer[8:12])
#     assert round(echo_value_1, 2) == 12.34
#     assert round(echo_value_2, 2) == 34.56
#     assert echo_value_3_bytes == bytes([0xAF, 0xA0, 0x00, 0x00])

#     ltbus_vm_buffer = (ctypes.c_uint8 * 0xFFF)()
#     libzcom.set_vm_buffer(ltbus_vm_buffer)

#     data_size = 0x008 + 2
#     libzcom.ltbus_send_mmap(data_size)

#     assert ltbus_vm_buffer[0] == 0x7B
#     assert ltbus_vm_buffer[1] == 0x01
#     assert ltbus_vm_buffer[2] == 0xAB
#     assert ltbus_vm_buffer[3] == 0x00
#     assert ltbus_vm_buffer[4] == 0xD0

#     assert ltbus_vm_buffer[5] == data_size
#     assert ltbus_vm_buffer[6] == 0

#     echo_value_1: float = struct.unpack('<f', bytes(ltbus_vm_buffer[7:11]))[0]
#     echo_value_2: float = struct.unpack('<f', bytes(ltbus_vm_buffer[11:15]))[0]
#     echo_value_3_bytes: bytes = bytes(ltbus_vm_buffer[15:17])
#     assert round(echo_value_1, 2) == 12.34
#     assert round(echo_value_2, 2) == 34.56
#     assert echo_value_3_bytes == bytes([0xAF, 0xA0])
