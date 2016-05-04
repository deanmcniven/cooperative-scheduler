#baud=19200
#programmerDev=/dev/ttyUSB003
#programmerType=arduino

srcDir=src
dstDir=target
src=main
avrType=atmega168p

cflags=-Wall -Os -Werror -Wextra

memoryTypes=calibration eeprom efuse flash fuse hfuse lfuse lock signature application apptable boot prodsig usersig

.PHONY: backup clean disassemble dumpelf edit eeprom elf flash fuses help hex makefile object program

help:
	@echo 'clean        Delete automatically created files.'
	@echo 'elf      Create $(src).elf'
#	@echo 'flash        Program $(src).hex to controller flash memory.'
	@echo 'help     Show this text.'
	@echo 'hex      Create all hex files for flash, eeprom and fuses.'
	@echo 'object       Create $(src).o'
#	@echo 'program      Do all programming to controller.'

#all: object elf hex

clean:
	rm -f $(dstDir)/*

object:
	avr-gcc $(cflags) -mmcu=$(avrType) -Wa,-ahlmns=$(dstDir)/$(src).lst -c -o $(dstDir)/$(src).o $(srcDir)/$(src).c

elf: object
	avr-gcc $(cflags) -mmcu=$(avrType) -o $(dstDir)/$(src).elf $(dstDir)/$(src).o
	chmod a-x $(dstDir)/$(src).elf 2>&1

hex:    elf
	avr-objcopy -j .text -j .data -O ihex $(dstDir)/$(src).elf $(dstDir)/$(src).flash.hex
	avr-objcopy -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 -O ihex $(dstDir)/$(src).elf $(dstDir)/$(src).eeprom.hex

#program: flash eeprom fuses
#
#flash: hex
#	avrdude -p$(avrType) -c$(programmerType) -P$(programmerDev) -b$(baud) -v -U flash:w:$(src).flash.hex
#	date
