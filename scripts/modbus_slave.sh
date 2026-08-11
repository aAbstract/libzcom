#!/bin/bash

diagslave -m enc -p 8502 -a 1
sudo socat -dd pty,raw,echo=0,link=/dev/ttyS90,mode=777 TCP:127.0.0.1:8502
sudo socat -dd pty,raw,echo=0,link=/dev/ttyS91,mode=777 TCP:127.0.0.1:8502
