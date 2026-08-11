import code
import struct
import readline
from rlcompleter import Completer


def _bytes_str(_bytes: bytes | bytearray) -> str:
    return ' '.join([f"0x{b:02X}" for b in _bytes])


def mdbus_f32(val: float):
    ieee_bytes = struct.pack('>f', val)
    print(f"IEEE-754: {_bytes_str(ieee_bytes)}")

    modbus_bytes = bytearray(4)
    modbus_bytes[0] = ieee_bytes[2]
    modbus_bytes[1] = ieee_bytes[3]
    modbus_bytes[2] = ieee_bytes[0]
    modbus_bytes[3] = ieee_bytes[1]
    print(f"ModBus: {_bytes_str(modbus_bytes)}")


def mdbus_u16(val: float):
    c_bytes = struct.pack('>H', val)
    print(f"C99: {_bytes_str(c_bytes)}")

    modbus_bytes = bytearray(2)
    modbus_bytes[0] = c_bytes[0]
    modbus_bytes[1] = c_bytes[1]
    print(f"ModBus: {_bytes_str(modbus_bytes)}")


def mdbus_i16(val: float):
    c_bytes = struct.pack('>h', val)
    print(f"C99: {_bytes_str(c_bytes)}")

    modbus_bytes = bytearray(2)
    modbus_bytes[0] = c_bytes[0]
    modbus_bytes[1] = c_bytes[1]
    print(f"ModBus: {_bytes_str(modbus_bytes)}")


if __name__ == '__main__':
    readline.set_completer(Completer().complete)
    readline.parse_and_bind("tab: complete")
    code.interact(local=locals())
