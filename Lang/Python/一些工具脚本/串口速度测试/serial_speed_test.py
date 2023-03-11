import time
from datetime import datetime

import serial


def main():
    ser = serial.Serial()
    ser.baudrate = 115200
    ser.timeout = 0.5
    ser.port = "COM21"

    try:
        ser.open()
    except:
        print("open ser failed")
        exit()

    while True:
        start = datetime.now()

        recv = ser.read(4096000)
        end = datetime.now()
        timeused = end - start

        # recv = str(recv,'utf-8')
        # print(recv)

        # print(timeused)

        readsize = len(recv)
        
        if readsize > 0:
            speed = readsize * 1000 * 1000 / timeused.microseconds / 1024     # KB/s
            print("readsize %d, timeused %d, speed = %.2f KB/s" % (readsize, timeused.microseconds, speed))

        else:
            time.sleep(1)

if __name__ == '__main__':
    main()
