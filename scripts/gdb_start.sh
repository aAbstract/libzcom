#/!/bin/bash

gdb --args python -m pytest --capture=no test/mdbus_test.py::test_mdbus_handle_request
