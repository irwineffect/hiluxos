cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project (hiluxos C ASM)

set (CMAKE_SYSTEM_NAME Generic)
set (CMAKE_SYSTEM_PROCESSOR arm)
set (TOOLCHAIN_PREFIX arm-none-eabi-)
set (CMAKE_ASM_COMPILER ${TOOLCHAIN_PREFIX}as)
set (CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}gcc)
set (CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}gcc)

set (CPU "-mcpu=cortex-m4")
set (CMAKE_ASM_FLAGS "-mthumb ${CPU}")
set (CMAKE_C_FLAGS "-mthumb ${CPU} -nostdlib")
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")

set (LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/layout.ld")

set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${LINKER_SCRIPT}")

add_executable(kernel.elf layout.ld startup.s main.c )
set_target_properties(kernel.elf PROPERTIES
    LINK_DEPENDS ${LINKER_SCRIPT})

add_custom_target(upload
    DEPENDS kernel.elf
    COMMAND arm-none-eabi-objcopy -O ihex -R .eeprom kernel.elf kernel.hex
    #COMMAND echo "ready to upload, press reset on Teensy"
    COMMAND teensy_loader_cli -v -w --mcu=mk64fx512 kernel.hex

)
