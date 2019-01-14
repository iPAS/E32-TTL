#!/usr/bin/env python3

import os
import sys

import asyncio
import datetime
import random


# --------------------------------------------------------------------------------------------------
@asyncio.coroutine
def task_display_stamped_message(f, loop):
    print(f)
    fo = open(f, 'r')
    try:
        #for line in iter(fo.readline, ''):
        while True:
            line = next(iter(fo.readline, ''))

            print(line, end='')
            yield None
    except:
        fo.close()


# --------------------------------------------------------------------------------------------------
if __name__ == '__main__':
    loop  = asyncio.get_event_loop()
    tasks = []
    for f in sys.argv[1:]:
        tasks.append( asyncio.ensure_future(
            task_display_stamped_message(f, loop)  # Because I'm the generator, right?
            ) )
    all_task = asyncio.gather(*tasks)

    try:
        loop.run_until_complete(all_task)
    except KeyboardInterrupt as e:
        # print('Caught keyboard interrupt. Canceling tasks...')
        all_task.cancel()
        # print('Canceled all tasks!')
        # loop.run_forever()
        # print('Wait the loop finished.')
        # all_task.exception()
        # print('Sent exception to all tasks!')
    finally:
        loop.close()
