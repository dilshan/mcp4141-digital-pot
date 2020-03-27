MCU = attiny13
CLOCK = 9600000

CC = avr-gcc
OBJCPY = avr-objcopy
AVRDUDE = avrdude

digital-pot: main.c main.h
	$(CC) -O1 -mmcu=$(MCU) -DF_CPU=$(CLOCK) -o digital-pot main.c
	$(OBJCPY) -j .text -j .data -O ihex digital-pot digital-pot.hex

flash:
	avrdude -p $(MCU) -c usbasp -U flash:w:digital-pot.hex

fuse:
	avrdude -p $(MCU) -c usbasp -U lfuse:w:0x7a:m -U hfuse:w:0xff:m

clean:
	rm -f digital-pot digital-pot.hex
