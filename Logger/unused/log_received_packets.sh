#!/bin/bash

device0=$1
if [[ -z "$device0" ]]; then
    device0=$(readlink /tmp/ttyUSB3)
else
    # http://rdiez.shoutwiki.com/wiki/Serial_Port_Tips_for_Linux
    # http://www.computerhope.com/unix/ustty.htm
    # -parenb : parity bit is required
    # -clocal : ignore carier signal
    # -cstopb : stop bits per (-)
    # -echo   : echo input character
    stty -F $device0 cs8 -parenb -cstopb -clocal raw 9600
fi


device1=$2
if [[ -z "$device1" ]]; then
    device1=$(readlink /tmp/ttyUSB1)
else
    # http://rdiez.shoutwiki.com/wiki/Serial_Port_Tips_for_Linux
    # http://www.computerhope.com/unix/ustty.htm
    # -parenb : parity bit is required
    # -clocal : ignore carier signal
    # -cstopb : stop bits per (-)
    # -echo   : echo input character
    stty -F $device1 cs8 -parenb -cstopb -clocal raw 9600
fi


function show()
{

}
trap "show '<<< Finish >>>'; exit 0" SIGHUP SIGINT SIGTRAP SIGTERM

