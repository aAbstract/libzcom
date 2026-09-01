#!/bin/bash

uv run ctypesgen -l libzcom_$1.so ./inc/libzcom_$1.h -o test/libzcom_$1_ffi.py
