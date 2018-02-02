#!/usr/bin/python
import serial
import sys
import time

if __name__ == "__main__":
    s = serial.Serial(port="/dev/ttyUSB0", baudrate=115200)
    print "Waiting for reset...",
    sys.stdout.flush()
    while s.in_waiting == 0:
        time.sleep(0.1)
    s.write('012')
    s.flush()
    print "done."
    s.close()
