#!/bin/bash

device=$1
if [[ -z "$device" ]]; then
    echo 'Please set the target device.'
    exit 255
fi

echo $device
# stty -F $device speed 9600 cs8 -parenb -cstopb raw  #-ignpar -clocal -crtscts -echo
# stty -F $device -speed 9600 cs8 -parenb -cstopb  #-ignpar -clocal -crtscts -echo
# stty -F $device 9600 cs8 -parodd -parenb -cstopb -icanon -iexten -ixon -ixoff -crtscts cread clocal echo -echoe echok -echoctl
stty -F $device speed 9600 cs8 -cstopb -parity
stty -F $device -a

function show()
{
    # echo -n -e "[$(date +%F\ %T)] $1\r\n"
    echo "[$(date +%F\ %T)] $1"
}
trap "show '<<< Finish >>>' > $device; exit 0" SIGHUP SIGINT SIGTRAP SIGTERM


show 'Get started!' > $device
sequence=0
while true; do
    mesg=$(show $sequence)
    echo $mesg | tee $device
    # show $sequence > $device
    sequence=$((sequence + 1))
    sleep 1
done
