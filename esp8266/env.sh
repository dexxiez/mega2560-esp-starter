# Source before building: `. ./env.sh`
# ESP8266_RTOS_SDK, toolchain and Python venv live outside the repo in ~/esp.
export IDF_PATH="$HOME/esp/ESP8266_RTOS_SDK"
export PATH="$HOME/esp/xtensa-lx106-elf/bin:$IDF_PATH/tools:$PATH"
. "$HOME/esp/venv/bin/activate"
# SDK's bundled CMakeLists predate CMake 4's dropped <3.5 compat.
export CMAKE_POLICY_VERSION_MINIMUM=3.5
