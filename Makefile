all:
	avr-gcc -mmcu=atmega168 -Wall -Os -DF_CPU=16000000UL -o src.elf main.c  -Wno-unused-function -Wno-parentheses
	avr-objcopy -j .text -j .data -O ihex src.elf main.hex
	avr-size -d -t src.elf
