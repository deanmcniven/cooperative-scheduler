#baud=19200
#programmerDev=/dev/ttyUSB003
#programmerType=arduino

SOURCES=main.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=coop-sched

CC=avr-gcc
CFLAGS=-Wall -Os -Werror -Wextra
AVRTYPE=atmega168p


all: $(SOURCES) $(EXECUTABLE) hex

clean:
	rm -f *.o *.elf *.lst *.hex

.c.o:
	$(CC) $(CFLAGS) -mmcu=$(AVRTYPE) -Wa,-ahlmns=$(EXECUTABLE).lst -c $< -o $@

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -mmcu=$(AVRTYPE) $(OBJECTS) -o $@.elf
	chmod a-x $(EXECUTABLE).elf 2>&1

hex: $(EXECUTABLE)
	avr-objcopy -j .text -j .data -O ihex $(EXECUTABLE).elf $(EXECUTABLE).flash.hex
	avr-objcopy -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 -O ihex $(EXECUTABLE).elf $(EXECUTABLE).eeprom.hex

#program: flash eeprom fuses
#
#flash: hex
#	avrdude -p$(avrType) -c$(programmerType) -P$(programmerDev) -b$(baud) -v -U flash:w:$(src).flash.hex
#	date
