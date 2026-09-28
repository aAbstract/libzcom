# libzcom

```
libzcom

Author: Eslam Elsharkawy
Version: 1.11.0
Date: 2026-09-27
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
uint16_t libzcom_set_bit(uint16_t x, uint8_t pos);
uint16_t libzcom_clear_bit(uint16_t x, uint8_t pos);
uint16_t libzcom_toggle_bit(uint16_t x, uint8_t pos);
uint16_t libzcom_check_bit(uint16_t x, uint8_t pos);

// mdbus-request-handler
MDBUS_RC mdbus_handle_request(const uint8_t* request_packet, uint16_t packet_size);
```

## LTBus API Reference
```c
// ltbus-virtual-memory
LTBUS_RC ltbus_set_slave_id(uint8_t _slave_id);
LTBUS_RC ltbus_set_page(uint8_t page_offset, uint8_t* page_ptr);
LTBUS_RC ltbus_get_page(uint16_t address, uint8_t** out_page_ptr);

// ltbus-codecs
LTBUS_RC ltbus_encode_read_regs(uint16_t address, uint16_t size, uint8_t* out_packet);
LTBUS_RC ltbus_encode_write_regs(uint16_t address, uint8_t* data_buffer, uint16_t data_size, uint8_t* out_packet);

// ltbus-utils
uint16_t ltbus_crc(const uint8_t* data, uint16_t len);
void ltbus_set_tx_buffer(uint8_t* tx_buffer);
void ltbus_transmit(uint8_t* packet, uint8_t packet_size);
uint16_t libzcom_set_bit(uint16_t x, uint8_t pos);
uint16_t libzcom_clear_bit(uint16_t x, uint8_t pos);
uint16_t libzcom_toggle_bit(uint16_t x, uint8_t pos);
uint16_t libzcom_check_bit(uint16_t x, uint8_t pos);

// ltbus-request-handler
LTBUS_RC ltbus_handle_request(const uint8_t* request_packet, uint16_t packet_size);
```

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
- `libzcom_set_bit` - ✅
- `libzcom_clear_bit` - ✅
- `libzcom_toggle_bit` - ✅
- `libzcom_check_bit` - ✅

#### ModBus Request Handler
- `mdbus_handle_request` - ✅

---

#### LTBus Virtual Memory System
- `ltbus_set_slave_id` - ✅
- `ltbus_set_page` - ✅
- `ltbus_get_page` - ✅

#### LTBus Source Operations - Native C - ✅

#### LTBus Codecs
- `ltbus_encode_read_regs` - ✅
- `ltbus_encode_write_regs` - ✅

#### LTBus Utils
- `ltbus_crc` - ✅
- `ltbus_set_tx_buffer` - ✅
- `ltbus_transmit` - ✅
- `libzcom_set_bit` - ✅
- `libzcom_clear_bit` - ✅
- `libzcom_toggle_bit` - ✅
- `libzcom_check_bit` - ✅

#### LTBus Request Handler
- `ltbus_handle_request` - ✅

## Testing - Docs
`libzcom` uses a Python-driven testing methodology to verify its C implementation.  
Tests are written using `pytest` and interact with the compiled C libraries through Python `ctypes`, 
allowing C functions to be tested directly from Python.  
The test suite covers individual functions and protocol-specific operations.  
`GDB` is used alongside `pytest` when debugging C-level failures 
such as segmentation faults and memory-access errors.

#### Build Requirements
```bash
$ make all
gcc -fPIC -Wall -O0 -g -shared src/libzcom_mdbus.c src/libzcom_common.c -Iinc -o libzcom_mdbus.so
gcc -fPIC -Wall -O0 -g -shared src/libzcom_ltbus.c src/libzcom_common.c -Iinc -o libzcom_ltbus.so
gcc -fPIC -Wall -O0 -g -shared src/libzcom_mdbus.c src/libzcom_ltbus.c src/libzcom_common.c -Iinc -o libzcom.so

$ ls libzcom*.so
libzcom_ltbus.so  libzcom_mdbus.so  libzcom.so
```

#### Setup Test Environment
- Install python project manager [uv](https://docs.astral.sh/uv/)
- Install test environment dependencies listed in file [pyproject.toml](./pyproject.toml)
```bash
$ uv sync
Resolved 11 packages in 10ms
Checked 9 packages in 1ms
```

#### Running the Tests
- Run all Tests
```bash
$ uv run pytest
test session starts
platform linux -- Python 3.10.20, pytest-9.0.3, pluggy-1.6.0
rootdir: /home/eslam/work/LabTronic/libzcom
configfile: pyproject.toml
collected 24 items 

test/ltbus_test.py ........              [ 33%]
test/mdbus_test.py ................      [100%]

24 passed in 0.13s
```

- Run One Test File
```bash
$ uv run pytest test/mdbus_test.py
test session starts
platform linux -- Python 3.10.20, pytest-9.0.3, pluggy-1.6.0
rootdir: /home/eslam/work/LabTronic/libzcom
configfile: pyproject.toml
collected 16 items

test/mdbus_test.py ................     [100%]

16 passed in 0.07s
```

- Run One Testcase
```bash
$ uv run pytest test/mdbus_test.py::test_mdbus_handle_request
test session starts
platform linux -- Python 3.10.20, pytest-9.0.3, pluggy-1.6.0
rootdir: /home/eslam/work/LabTronic/libzcom
configfile: pyproject.toml
collected 1 item

test/mdbus_test.py .

1 passed in 0.07s
```

## Debugging Failed Tests

#### Debugging Pytest Tests
Pytest unit tests can be debugged using either the **VS Code Debugger** or Python's built-in **PDB Debugger**.

##### VS Code PDB Integration
VS Code can run pytest tests directly through its Python testing integration.  
Tests can be started in debug mode by placing breakpoints in the test code 
and selecting **Debug Test** from the test explorer or the inline debug option next to the test.  
This allows you to pause execution, inspect variables, step through the Python test code, and investigate failures interactively.

##### PDB CLI
For command-line debugging, Python's built-in **PDB Debugger** can be used by adding a breakpoint to the test:

```py
# ...
breakpoint()
# ...
```

When pytest reaches this statement, execution pauses and an interactive **PDB** session is opened. Common commands include:

```bash
n       # execute the next line
s       # step into a function
c       # continue execution
p var   # print a variable
pp var  # pretty-print a variable
q       # quit the debugger
```

Alternatively, `pytest` can be started with **PDB** enabled:

```bash
$ uv run pytest --pdb
```

With this option, `pytest` automatically enters the debugger when a test fails, allowing the failure to be inspected interactively.

#### Debugging C Code
The Python debugger only debugs the Python side of the test. It cannot step through or inspect the compiled C code called through `ctypes`.  
When a test needs to be debugged at the C level, **GDB** should be used instead. 
The libraries are compiled with debug information `-g`, allowing **GDB** to set breakpoints in the C source, 
inspect variables, step through C functions, and investigate errors such as segmentation faults.  

##### VS Code GDB Integration
VS Code can also use its built-in **GDB** integration for **C/C++ Debugging**. 
A debug configuration can launch the pytest process and attach **GDB** to it, allowing Python and the native C library to be debugged together.  
Sample debug configuration can be found here [launch.json](.vscode/launch.json)

```js
"args": [
    "-m",
    "pytest",
    "--capture=no",
    "test/ltbus_test.py::test_ltbus_handle_write_request" // edit this to change target test to debug using GDB
],
```

##### GDB CLI
For command-line debugging, GDB can also be launched directly:
```bash
$ gdb --args python -m pytest --capture=no test/mdbus_test.py::test_mdbus_handle_request

# start the test from the GDB prompt
(gdb) run

# useful commands include:
(gdb) break function_name
(gdb) next
(gdb) step
(gdb) print variable
(gdb) backtrace
(gdb) continue
```

For a breakpoint mechanism similar to Python `breakpoint()`, the following macro can be used in C code:
```c
#define GDB_TRIGGER                        \
    printf("GDB Trigger: %d\n", getpid()); \
    raise(SIGTRAP)
```

Place `GDB_TRIGGER;` at the point where execution should pause in the C source.  
When the test is running under **GDB**, `SIGTRAP` causes the debugger to stop at that location, 
allowing the C code to be inspected and stepped through.
