#!/usr/bin/env python3

import os
import sys
from time import sleep
from datetime import datetime
import serial


# --------------------------------------------------------------------------------------------------
if __name__ == '__main__':

    ser = serial.Serial(sys.argv[1])  # /dev/ttyUSB0, or ttyUSB2 in testing

    if len(sys.argv) > 2:
        suf = ' ' + sys.argv[2]
    else:
        suf = ''

    period = 1
    if len(sys.argv) > 3:
        period = int(sys.argv[3])

    single_packet_required = True
    if len(sys.argv) > 4:
        single_packet_required = True if sys.argv[4].lower() != 'false' else False


    seq = 0
    try:
        while True:
            msg = ''
            #msg += '{:>5} '.format(seq)
            msg += '{:05d} '.format(seq)
            msg += datetime.now().strftime('%F %T')
            msg += '{}\n'.format(suf)  # 'newline' must be sent, the receiver checks it!
            msg = msg.encode('utf-8')

            if single_packet_required:
                assert len(msg) <= 58, 'Message size is over than 58 bytes! Please reduce.'

            ser.flush()  # Wait for the old to be sent.
            ser.write(msg)

            print('Send {:>2} bytes: {}'.format(len(msg), msg))
            sys.stdout.flush()

            sleep(period)
            seq += 1
            seq %= 100000

    except KeyboardInterrupt as e:
        print(e)
    except Exception as e:
        print(e)
    finally:
        ser.close()
        print('...End...')
