#!/usr/bin/env python3

import os
import sys
from datetime import datetime
import asyncio
import serial


# http://stackoverflow.com/questions/33824359/read-file-line-by-line-with-asyncio
from concurrent.futures import ThreadPoolExecutor
io_pool_executor = ThreadPoolExecutor()


# --------------------------------------------------------------------------------------------------
@asyncio.coroutine
def task_display_stamped_message(f, future, loop):
    ser = serial.Serial(f)
    try:
        while True:
            # line = ser.readline()
            line = yield from loop.run_in_executor(io_pool_executor, ser.readline)
            # line = yield from loop.run_in_executor(None, ser.readline)
            # yield from asyncio.sleep(1)
            print('[{}] '.format(datetime.now().strftime('%F %T')) + line.decode('utf-8'),
                  end='')  # 'newline' might be sent within the message.
            sys.stdout.flush()
            # yield None
    except:
        pass
        # print('Exception!')
    finally:
        ser.close()
        print('Close', f)


# --------------------------------------------------------------------------------------------------
if __name__ == '__main__':
    loop   = asyncio.get_event_loop()
    future = asyncio.Future()

    tasks = []
    for f in sys.argv[1:]:
        tasks.append(
            asyncio.ensure_future(  # Because I'm the generator, right?
                task_display_stamped_message(f, future, loop) ))
    all_task = asyncio.gather(*tasks)

    try:
        loop.run_until_complete(all_task)

    except KeyboardInterrupt:
        print('Caught keyboard interrupt. Canceling tasks...')

        all_task.cancel()
        print('Cancel all tasks!')

        loop.run_forever()
        print('Wait the loop finished.')

        all_task.exception()
        print('Send exception to all tasks!')

    except BaseException as e:
        print('Exception >', e)

    finally:
        print('Close loop')
        loop.close()
