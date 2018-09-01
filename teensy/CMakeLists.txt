cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project (hiluxos CXX ASM)

set (CMAKE_SYSTEM_NAME Generic)
set (CMAKE_SYSTEM_PROCESSOR arm)
set (TOOLCHAIN_PREFIX arm-none-eabi-)
set (CMAKE_ASM_COMPILER ${TOOLCHAIN_PREFIX}as)
set (CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}gcc)
set (CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}g++)

set (CPU "-mcpu=cortex-m4")
set (CMAKE_ASM_FLAGS "-mthumb ${CPU}")
#set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -mthumb ${CPU} -nostdlib")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror -mthumb ${CPU} -fno-exceptions -fno-unwind-tables -fno-rtti -nostdlib")

# These lines suppress CMake from adding the -rdynamic flag
set (CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set (CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")

set (LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/layout.ld")

set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${LINKER_SCRIPT}")

set (PERIPHERIAL_FILES
    debug.cpp
    pin_control.cpp
    uart.cpp
    printf.cpp
    spi.cpp
    clocks.cpp
    rtc.cpp
    interrupt_timer.cpp
)

add_executable(kernel.elf layout.ld startup.s handlers.s main.cpp ${PERIPHERIAL_FILES})
target_link_libraries(kernel.elf c gcc)
set_target_properties(kernel.elf PROPERTIES
    LINK_DEPENDS ${LINKER_SCRIPT})

add_custom_command(OUTPUT kernel.hex
    DEPENDS kernel.elf
    COMMAND arm-none-eabi-objcopy -O ihex -R .eeprom kernel.elf kernel.hex
)

add_custom_target(upload
    DEPENDS kernel.hex
    COMMAND python ${PROJECT_SOURCE_DIR}/../reset/reset.py
    COMMAND teensy_loader_cli -v -w --mcu=mk64fx512 kernel.hex
)

add_custom_target(remote-upload
    DEPENDS kernel.hex
    COMMAND rsync kernel.hex oryx:Repos/hiluxos/teensy/build
    COMMAND ssh oryx /home/james/bin/remote-teensy-upload.sh
)

add_custom_target(dump
    DEPENDS kernel.elf
    COMMAND arm-none-eabi-objdump -S kernel.elf | c++filt
)
