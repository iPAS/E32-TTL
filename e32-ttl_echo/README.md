
# E32-TTL Echoer

Let Arduino-nano sends any byte received, back to an attached E32-TTL transceiver without any transformation,
    regarding the AUX pin.
Following links gave me an idea for the code:

* http://spacetinkerer.blogspot.com/2011/02/serial-port-echo-with-arduino.html
* https://github.com/tienfuc/E32-TTL

And, of course, the general hardware and software UART modules are required.
Please follow http://www.arduino.cc/en/Tutorial/SoftwareSerialExample for further information.

In addition, I use a circular queue -- adopted from https://github.com/sdesalas/Arduino-Queue.h --
    to buffer incoming bytes due to the intention that
    trying to send bytes terminated with '\n' or limited by maximum packet size.
