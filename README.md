# STK2UPDI

This is a firmware, that when uploaded on an atmega328p, or a similar AVR MCU running @16Mhz, enables it to interface with Atmel Studio as if it were an STK500 programmer. In particular, you can use an Arduino to host this firmware.
It provides a bridge to program the new attiny817 family of MCUs, that use the UPDI interface, from Atmel Studio, using the STK500v2 protocol:

Atmel Studio -> HW Serial interface -> Programmer MCU (e.g. Mega328P) -> SW Serial on PD6 -> Target MCU (e.g. tiny817)

Currently, I have not tested this software with a level shifter, so the target MCU must run at the same voltage as the programmer.

<pre>
                                              Vcc                     Vcc
                                              +-+                     +-+
                                               |                       |
 +----------+          +---------------------+ |                       | +--------------------+
 | Atmel    |          | Programmer          +-+                       +-+  Target            |
 | Studio   |          |                     |      +----------+         |                    |
 |       TX +----------+ RX              PD6 +------+   4k7    +---------+ UPDI               |
 |          |          |                     |      +----------+         |                    |
 |       RX +----------+ TX                  |                           |                    |
 |          |          |                     |                           |                    |
 |          |          |                     |                           |                    |
 |          |          |                     +--+                     +--+                    |
 +----------+          +---------------------+  |                     |  +--------------------+
              STK500v2                         +-+     UPDI          +-+
              Protocol                         GND     Protocol      GND

</pre>
Adapted from: https://github.com/mraardvark/pyupdi


To build, run the make.bat file, after editing it with the path of AVR-GCC on your system. Since this program is meant to talk to Atmel Studio, I provide a makefile suitable for a MS-Windows environment.

There are also pre-built files on the "build" directory. They were built using avr-gcc 8.0 compiled for MinGW by sprintersb:
https://sourceforge.net/projects/mobilechessboar/files/avr-gcc%20snapshots%20%28Win32%29/

More information at: http://www.avrfreaks.net/forum/stk500-updi-working-was-enabling-xtiny-updi
