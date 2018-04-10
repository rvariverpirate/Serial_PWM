#!/bin/bash

echo -e "Please enter the name of the .c file (exclude .c extention): "
read name

avr-gcc -Wall -g -Os -mmcu=atmega328 -o $name.bin $name.c
avr-size -C $name.bin
avr-objcopy -j .text -j .data -O ihex $name.bin $name.hex
sudo avrdude -p m328 -c usbasp -U flash:w:$name.hex
