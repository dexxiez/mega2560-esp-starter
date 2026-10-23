# Mega 2560 + ESP8266 template
# Run `just` with no arguments to list all recipes.

set shell := ["bash", "-euo", "pipefail", "-c"]

# Serial port. Override with: just port=/dev/ttyUSB1 mega-flash
port := env("PORT", "/dev/ttyUSB0")

# Every ESP recipe sources env.sh first (each just recipe is its own shell).
esp := "cd esp8266 && . ./env.sh && make"

# Show all recipes
_default:
    @just --list --unsorted

# ---------------------------------------------------------------
# Mega (DIP 3+4 ON)
# ---------------------------------------------------------------

# Build mega2560/build/firmware.hex and print flash/RAM usage
mega-build:
    make -C mega2560

# Flash the Mega via the stk500v2 bootloader
mega-flash:
    make -C mega2560 flash PORT={{port}}

# Serial monitor at 115200 (quit with Ctrl-a k)
mega-monitor:
    make -C mega2560 monitor PORT={{port}}

# Remove mega2560/build
mega-clean:
    make -C mega2560 clean

# ---------------------------------------------------------------
# ESP8266 (flash: DIP 5+6+7, then 7 OFF + reset; monitor: DIP 5+6)
# ---------------------------------------------------------------

# Build bootloader, partition table and app
esp-build:
    {{esp}}

# Build and flash. Afterwards turn DIP 7 OFF and press reset
esp-flash:
    {{esp}} flash PORT={{port}}

# Serial monitor with log colours (quit with Ctrl-])
esp-monitor:
    {{esp}} monitor PORT={{port}}

# SDK config UI (copy changes you want to keep into sdkconfig.defaults)
esp-menuconfig:
    {{esp}} menuconfig

# Memory usage summary
esp-size:
    {{esp}} size

# Full ESP clean
esp-clean:
    {{esp}} clean

# ---------------------------------------------------------------
# Both chips
# ---------------------------------------------------------------

# Build Mega and ESP
build: mega-build esp-build

# Clean Mega and ESP
clean: mega-clean esp-clean
