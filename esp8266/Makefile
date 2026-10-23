# Thin wrapper over idf.py. Run `. ./env.sh` first.
# Flash/monitor need DIP 5+6+7 ON (USB <-> ESP, program mode); run mode: 7 OFF.

PORT ?= /dev/ttyUSB0
IDF  := idf.py -p $(PORT)

.PHONY: all flash monitor menuconfig size clean

all:
	$(IDF) build

flash:
	$(IDF) flash

monitor:
	$(IDF) monitor

menuconfig:
	$(IDF) menuconfig

size:
	$(IDF) size

clean:
	$(IDF) fullclean
