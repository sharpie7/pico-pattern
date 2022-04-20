#
# Title:	        Pico-pattern Makefile
#

#
# See the official documentation https://www.raspberrypi.com/documentation/microcontrollers/c_sdk.html
# for guidance on setting up the Pico C/C++ SDK.
#
cmake_minimum_required(VERSION 3.13)

if (NOT PICO_SDK_PATH)
    message(FATAL_ERROR "PICO_SDK_PATH is not set")
endif()

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(pico-pattern)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()



enable_language( C CXX ASM )

add_executable(pico-pattern)

pico_generate_pio_header(pico-pattern ${CMAKE_CURRENT_LIST_DIR}/cvideo_sync.pio)
pico_generate_pio_header(pico-pattern ${CMAKE_CURRENT_LIST_DIR}/cvideo_data.pio)

target_sources(
        pico-pattern PRIVATE
        main.c
        cvideo.c
        graphics.c
        charset.c
        bitmap.c
        terminal.c
)

target_link_libraries(
        pico-pattern PRIVATE
        pico_stdlib
        pico_mem_ops
		hardware_gpio
        hardware_pio
        hardware_dma
        hardware_irq
)

pico_add_extra_outputs(pico-pattern)

# add url via pico_set_program_url
#
# example_auto_set_url(pico-pattern)

# generate .hex file and .pio.h file for the RP2040 datasheet (to make sure
# the datasheet always shows the output of the latest pioasm version)
#
add_custom_target(
        pio_cvideo_sync_datasheet DEPENDS
	Pioasm
	${CMAKE_CURRENT_LIST_DIR}/generated/cvideo_sync.hex
	${CMAKE_CURRENT_LIST_DIR}/generated/cvideo_sync.pio.h
)
add_custom_command(
        OUTPUT ${CMAKE_CURRENT_LIST_DIR}/generated/cvideo_sync.hex
        DEPENDS ${CMAKE_CURRENT_LIST_DIR}/cvideo_sync.pio
        COMMAND Pioasm -o hex ${CMAKE_CURRENT_LIST_DIR}/cvideo_sync.pio ${CMAKE_CURRENT_LIST_DIR}/generated/cvideo_sync.hex
)
add_custom_command(
        OUTPUT ${CMAKE_CURRENT_LIST_DIR}/generated/cvideo_sync.pio.h
        DEPENDS ${CMAKE_CURRENT_LIST_DIR}/cvideo_sync.pio
        COMMAND Pioasm ${CMAKE_CURRENT_LIST_DIR}/cvideo_sync.pio ${CMAKE_CURRENT_LIST_DIR}/generated/cvideo_sync.pio.h
)

add_custom_target(
        pio_cvideo_data_datasheet DEPENDS
	Pioasm
	${CMAKE_CURRENT_LIST_DIR}/generated/cvideo_data.hex
	${CMAKE_CURRENT_LIST_DIR}/generated/cvideo_data.pio.h
)
add_custom_command(
        OUTPUT ${CMAKE_CURRENT_LIST_DIR}/generated/cvideo_data.hex
        DEPENDS ${CMAKE_CURRENT_LIST_DIR}/cvideo_data.pio
        COMMAND Pioasm -o hex ${CMAKE_CURRENT_LIST_DIR}/cvideo_data.pio ${CMAKE_CURRENT_LIST_DIR}/generated/cvideo_data.hex
)
add_custom_command(
        OUTPUT ${CMAKE_CURRENT_LIST_DIR}/generated/cvideo_data.pio.h
        DEPENDS ${CMAKE_CURRENT_LIST_DIR}/cvideo_data.pio
        COMMAND Pioasm ${CMAKE_CURRENT_LIST_DIR}/cvideo_data.pio ${CMAKE_CURRENT_LIST_DIR}/generated/cvideo_data.pio.h
)


add_dependencies(pico-pattern pio_cvideo_sync_datasheet pio_cvideo_data_datasheet)