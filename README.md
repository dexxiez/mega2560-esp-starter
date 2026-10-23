# Mega 2560 + ESP8266 Template

Bare C template for the Keyestudio Mega 2560 + ESP8266 WiFi board (RobotDyn-style layout). No Arduino IDE, no Arduino core.

| Chip | Directory | Toolchain | Runtime |
|---|---|---|---|
| ATmega2560 | `mega2560/` | avr-gcc + avr-libc, avrdude | Bare metal |
| ESP8266 | `esp8266/` | xtensa-lx106 gcc 8.4, ESP8266_RTOS_SDK v3.4 | FreeRTOS |

Out of the box:

- **Mega:** blinks the LED on pin 13 and bridges USB (USART0) ↔ ESP8266 (USART3), so ESP output shows up in the Mega's serial monitor.
- **ESP8266:** logs a heartbeat (uptime + free heap) every second.

## Layout

```
mega2560/
├── Makefile                 build / flash / monitor
└── src/
    ├── main.c               app entry: LED blink + USB <-> ESP bridge
    ├── board.h              pins + baud rates
    └── drivers/
        ├── uart.*           USART0 (USB) + USART3 (ESP), interrupt-driven RX ring buffers
        ├── uart_stdio.*     printf -> USB UART
        └── systick.*        1 ms tick on Timer0, systick_millis()

esp8266/
├── env.sh                   source before building (SDK, toolchain, Python venv)
├── Makefile                 thin wrapper over idf.py
├── CMakeLists.txt           every folder in src/ is a component
├── sdkconfig.defaults       committed config overrides (sdkconfig itself is generated)
└── src/
    ├── main/                app_main()
    └── heartbeat/           example component (include/ holds its public header)
```

Any `.c` file under `mega2560/src/` is picked up automatically. On the ESP side, add a folder under `esp8266/src/` with its own `CMakeLists.txt` to create a new component.

## DIP switches

| Switches ON | Mode | Use |
|---|---|---|
| 3, 4 | USB ↔ Mega | Flash the Mega, Mega `printf` output |
| 5, 6, 7 | USB ↔ ESP, **program mode** | Flash the ESP |
| 5, 6 | USB ↔ ESP, run mode | ESP serial log directly |
| 1, 2 | Mega ↔ ESP (Serial3) | Chips talk to each other, USB unused |
| 1, 2, 3, 4 | Mega ↔ ESP + USB ↔ Mega | Normal running: ESP log reaches USB via the Mega bridge |

All others OFF. Switch 8 is not connected. After changing switches, press reset.

The first four rows are the vendor's official modes. 1+2+3+4 isn't in the vendor table, but it works on this board.

## Setup (one time)

Tested on Fedora. The board's USB chip is a CH340 and shows up as `/dev/ttyUSB0`. Your user needs to be in the `dialout` group.

### Mega toolchain

```sh
sudo dnf install avr-gcc avr-libc avr-binutils avrdude
```

### ESP8266 SDK

The SDK, toolchain and Python venv live in `~/esp`, outside the repo. The SDK's Python tooling doesn't work on Python 3.12+, so the venv uses 3.11 (via [uv](https://github.com/astral-sh/uv)).

```sh
mkdir -p ~/esp && cd ~/esp

git clone -b release/v3.4 --recursive https://github.com/espressif/ESP8266_RTOS_SDK.git

curl -fsSL https://dl.espressif.com/dl/xtensa-lx106-elf-gcc8_4_0-esp-2020r3-linux-amd64.tar.gz | tar xz

uv venv --python 3.11 ~/esp/venv
VIRTUAL_ENV=~/esp/venv uv pip install "setuptools<70" -r ~/esp/ESP8266_RTOS_SDK/requirements.txt
```

If you put things somewhere else, edit `esp8266/env.sh`.

## Commands

Run everything from the repo root with [just](https://github.com/casey/just) (`sudo dnf install just`). Run `just` on its own to list all recipes. ESP recipes source `esp8266/env.sh` for you.

The serial port defaults to `/dev/ttyUSB0`. Override it with `just port=/dev/ttyUSB1 mega-flash` or `PORT=/dev/ttyUSB1 just mega-flash`.

### Mega

| Command | DIP | Does |
|---|---|---|
| `just mega-build` | any | Build `mega2560/build/firmware.hex` and print flash/RAM usage |
| `just mega-flash` | 3, 4 | Upload via the on-board stk500v2 bootloader |
| `just mega-monitor` | 3, 4 | Serial monitor at 115200 (`screen`, quit with `Ctrl-a k`) |
| `just mega-clean` | any | Remove `mega2560/build/` |

### ESP8266

| Command | DIP | Does |
|---|---|---|
| `just esp-build` | any | Build bootloader, partition table and app |
| `just esp-flash` | 5, 6, 7 | Build and flash. Afterwards turn 7 OFF and press reset |
| `just esp-monitor` | 5, 6 | Serial monitor with log colours (quit with `Ctrl-]`) |
| `just esp-menuconfig` | any | SDK config UI. Copy changes you want to keep into `sdkconfig.defaults` |
| `just esp-size` | any | Memory usage summary |
| `just esp-clean` | any | Full clean |

### Both

| Command | Does |
|---|---|
| `just build` | Build Mega and ESP |
| `just clean` | Clean Mega and ESP |

### Without just

Each project's `Makefile` has the same targets: `make`, `flash`, `monitor`, `clean`, plus `menuconfig` and `size` on the ESP. For example, `make -C mega2560 flash PORT=/dev/ttyUSB1`. For the ESP, run `. ./env.sh` in `esp8266/` once per terminal first.

`esp8266/build/compile_commands.json` is generated on build for clangd and other editors.

## Typical workflow

1. **Mega:** DIP 3+4 → `just mega-flash`
2. **ESP:** DIP 5+6+7 → `just esp-flash` → turn 7 OFF, reset
3. **Run both:** DIP 1+2+3+4 → reset → `just mega-monitor` shows Mega and ESP output

## Notes and gotchas

- **ESP flashing erases the factory AT firmware.** If you want AT commands back, flash Espressif's AT firmware yourself.
- **Baud rates:** Mega↔USB and Mega↔ESP both run at 115200 (`mega2560/src/board.h`). The ESP console baud is set in `esp8266/sdkconfig.defaults`, so keep the two in sync.
- **Boot garbage:** the ESP ROM prints its first boot lines at 74880 baud, which shows as junk at 115200. This is harmless.
- **Flash size:** the ESP is configured for 2MB, which is safe. Most of these boards have 4MB; raise it in `make menuconfig` if you need the space.
- **CMake 4:** the SDK's old CMakeLists need `CMAKE_POLICY_VERSION_MINIMUM=3.5`. `env.sh` sets it.
- **`src/main/CMakeLists.txt` has no `REQUIRES` on purpose.** Adding one disables the SDK's "main depends on everything" default and breaks linking (missing `pthread_*`). Declare dependencies in your other components instead.
- **Opening the serial port resets the board** (DTR auto-reset), so the first lines of ESP output can be missed while the Mega boots.
