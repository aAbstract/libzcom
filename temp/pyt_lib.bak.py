import os
import ctypes


def test_tustin_filter():
    libzcom = _libs['libzcom.so']  # type: ignore
    _P_Tustin_Filter_T_ = pyt_lib.P_Tustin_Filter_T_.in_dll(libzcom, 'Tustin_Filter_P')  # type: ignore

    num_coef = list(_P_Tustin_Filter_T_.DiscreteTransferFcn_NumCoef)
    assert num_coef == [0.01547, 0.01547]

    den_coef = list(_P_Tustin_Filter_T_.DiscreteTransferFcn_DenCoef)
    assert den_coef == [1, -0.9691]


class DeviceRegisterConfig(ctypes.Structure):
    _fields_ = [
        ('register_address', ctypes.c_uint16),
        ('register_size', ctypes.c_uint16),
        ('register_ptr', ctypes.POINTER(ctypes.c_uint8)),
    ]


class P_Tustin_Filter_T_(ctypes.Structure):
    _fields_ = [
        ('DiscreteTransferFcn_NumCoef', ctypes.c_double * 2),
        ('DiscreteTransferFcn_DenCoef', ctypes.c_double * 2),
    ]


def init_utils_ffi(libzcom: ctypes.CDLL):
    libzcom.set_bit.argtypes = [
        ctypes.c_uint16,
        ctypes.c_uint8,
    ]
    libzcom.set_bit.restype = ctypes.c_uint16

    libzcom.clear_bit.argtypes = [
        ctypes.c_uint16,
        ctypes.c_uint8,
    ]
    libzcom.clear_bit.restype = ctypes.c_uint16

    libzcom.toggle_bit.argtypes = [
        ctypes.c_uint16,
        ctypes.c_uint8,
    ]
    libzcom.toggle_bit.restype = ctypes.c_uint16

    libzcom.check_bit.argtypes = [
        ctypes.c_uint16,
        ctypes.c_uint8,
    ]
    libzcom.check_bit.restype = ctypes.c_uint16

    libzcom.set_vm_buffer.argtypes = [
        ctypes.POINTER(ctypes.c_uint8),
    ]
    libzcom.set_vm_buffer.restype = None


def init_mdbus_ffi(libzcom: ctypes.CDLL):
    # MDBUS_RC mdbus_set_slave_id(uint8_t _slave_id);
    libzcom.mdbus_set_slave_id.argtypes = [
        ctypes.c_uint8,                  # uint8_t _slave_id
    ]
    libzcom.mdbus_set_slave_id.restype = ctypes.c_uint8

    # MDBUS_RC mdbus_handle_request(const uint8_t* request_packet, uint16_t packet_size);
    libzcom.mdbus_handle_request.argtypes = [
        ctypes.POINTER(ctypes.c_uint8),  # const uint8_t* request_packet
        ctypes.c_uint16,                 # uint16_t packet_size
    ]
    libzcom.mdbus_handle_request.restype = ctypes.c_uint8

    # uint16_t mdbus_rtu_crc(const uint8_t* data, int len);
    libzcom.mdbus_rtu_crc.argtypes = [
        ctypes.POINTER(ctypes.c_uint8),  # const uint8_t* data
        ctypes.c_int,                    # int len
    ]
    libzcom.mdbus_rtu_crc.restype = ctypes.c_uint16


def init_ltbus_ffi(libzcom: ctypes.CDLL):
    # LTBUS_RC ltbus_init_device(uint8_t _slave_id, uint8_t* _config_buffer, uint8_t* _data_buffer);
    libzcom.ltbus_init_device.argtypes = [
        ctypes.c_uint8,                    # uint8_t _slave_id
        ctypes.POINTER(ctypes.c_uint8),    # uint8_t* _config_buffer
        ctypes.POINTER(ctypes.c_uint8),    # uint8_t* _data_buffer
    ]
    libzcom.ltbus_init_device.restype = ctypes.c_uint8

    # LTBUS_RC ltbus_decode_device_register_config(const uint8_t* request_packet, DeviceRegisterConfig* out_conf);
    libzcom.ltbus_decode_device_register_config.argtypes = [
        ctypes.POINTER(ctypes.c_uint8),         # const uint8_t* request_packet
        ctypes.POINTER(DeviceRegisterConfig),   # DeviceRegisterConfig* out_conf
    ]
    libzcom.ltbus_decode_device_register_config.restype = ctypes.c_uint8

    # LTBUS_RC ltbus_read_request(uint16_t address, uint16_t size, uint8_t* out_packet);
    libzcom.ltbus_read_request.argtypes = [
        ctypes.c_uint16,                    # uint16_t address
        ctypes.c_uint16,                    # uint16_t size
        ctypes.POINTER(ctypes.c_uint8),     # uint8_t* out_packet
    ]
    libzcom.ltbus_read_request.restype = ctypes.c_uint8

    # LTBUS_RC ltbus_write_f32_request(uint16_t address, float value, uint8_t* out_packet);
    libzcom.ltbus_write_f32_request.argtypes = [
        ctypes.c_uint16,                    # uint16_t address
        ctypes.c_float,                     # float value
        ctypes.POINTER(ctypes.c_uint8),     # uint8_t* out_packet
    ]
    libzcom.ltbus_write_f32_request.restype = ctypes.c_uint8

    # LTBUS_RC ltbus_write_u16_request(uint16_t address, uint16_t value, uint8_t* out_packet);
    libzcom.ltbus_write_u16_request.argtypes = [
        ctypes.c_uint16,                    # uint16_t address
        ctypes.c_uint16,                    # uint16 value
        ctypes.POINTER(ctypes.c_uint8),     # uint8_t* out_packet
    ]
    libzcom.ltbus_write_u16_request.restype = ctypes.c_uint8

    # LTBUS_RC ltbus_write_i16_request(uint16_t address, int16_t value, uint8_t* out_packet);
    libzcom.ltbus_write_i16_request.argtypes = [
        ctypes.c_uint16,                    # uint16_t address
        ctypes.c_int16,                     # int16_t value
        ctypes.POINTER(ctypes.c_uint8),     # uint8_t* out_packet
    ]
    libzcom.ltbus_write_i16_request.restype = ctypes.c_uint8

    # uint16_t ltbus_crc(const uint8_t* data, int len);
    libzcom.ltbus_crc.argtypes = [
        ctypes.POINTER(ctypes.c_uint8),  # const uint8_t* data
        ctypes.c_int,                    # int len
    ]
    libzcom.ltbus_crc.restype = ctypes.c_uint16


def load_libzcom_ffi() -> ctypes.CDLL:
    libzcom = ctypes.CDLL(os.path.join(os.getcwd(), 'libzcom.so'))
    init_mdbus_ffi(libzcom)
    init_ltbus_ffi(libzcom)
    return libzcom
