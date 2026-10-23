cmake_minimum_required(VERSION 3.5)

# Every subfolder of src/ is a component; src/main holds app_main().
set(EXTRA_COMPONENT_DIRS "${CMAKE_CURRENT_LIST_DIR}/src")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(esp8266-firmware)
