#!/bin/bash

gcc -fPIC -Wall -O0 -g -shared src/*.c Iinc -o libzcom.so
