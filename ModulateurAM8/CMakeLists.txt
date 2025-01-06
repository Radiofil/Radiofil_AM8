cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(am8F01)

pico_sdk_init()

add_executable(am8F01
        src/main.c
        src/usb_descriptors.c
        src/get_serial.c
        )

if (DEFINED ENV{AM8F01_LED})
        message("AM8F01_LED is defined as " $ENV{AM8F01_LED})
        target_compile_definitions(am8F01 PRIVATE AM8F01_LED=$ENV{AM8F01_LED})
endif()

set(DBG_PIN_COUNT=4)

pico_generate_pio_header(am8F01 ${CMAKE_CURRENT_LIST_DIR}/src/carr.pio)

target_include_directories(am8F01 PRIVATE src)

target_link_libraries(am8F01 PRIVATE pico_stdlib pico_unique_id 
     tinyusb_device 
     tinyusb_board hardware_pio 
     hardware_pwm)

pico_add_extra_outputs(am8F01)
