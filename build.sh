#!/bin/bash

gcc -fPIC -Wall -O0 -g -shared src/*.c -I. -o libzcom.so
