@echo off

set PORT="COM5"
set CHIP="atmega328p"

set CFILE="main.c"

"C:\Program Files (x86)\Atmel\AVR Tools\AVR Toolchain\bin\avr-gcc.exe" %CFILE% -c -o flash.o -Os -g -mmcu=atmega328p -I /usr/avr/sys-root/include
"C:\Program Files (x86)\Atmel\AVR Tools\AVR Toolchain\bin\avr-gcc.exe" flash.o -o flash.elf -mmcu=atmega328p
"C:\Program Files (x86)\Atmel\AVR Tools\AVR Toolchain\bin\avr-objcopy.exe" -j .text -j .data -O ihex flash.elf flash.hex

del "flash.O"
rem del "flash.elf"

"C:\Program Files (x86)\Atmel\AVR Tools\AVR Toolchain\bin\avrdude.exe" -C"C:\Program Files (x86)\Atmel\AVR Tools\AVR Toolchain\bin\avrdude.conf" -p%CHIP% -carduino -P%PORT% -b57600 -D -Uflash:w:flash.hex:i
pause