# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(aqualink C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(aqualink aqualink.c Conection/http_server.c Display/display.c Display/Display1306/ssd1306_i2c.c Joystick/joystick.c RGB/led.c Global/global.c
LedMatrix/matriz.c Buzzer/buzzer.c Temperature/temp.c Conection/mqtt_comm.c)


# Generate PIO header
pico_generate_pio_header(aqualink ${CMAKE_CURRENT_LIST_DIR}/LedMatrix/ws2818b.pio)

pico_set_program_name(aqualink "aqualink")
pico_set_program_version(aqualink "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(aqualink 1)
pico_enable_stdio_usb(aqualink 1)

# Add the standard library to the build
target_link_libraries(aqualink
        pico_stdlib
        hardware_adc
        hardware_pwm
        hardware_gpio
        hardware_i2c
        )

# Add the standard include files to the build
target_include_directories(aqualink PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(aqualink 
        pico_cyw43_arch_lwip_threadsafe_background
        hardware_i2c
        hardware_pio
        hardware_adc
        hardware_pwm
        hardware_gpio
        pico_multicore
        pico_lwip_mqtt
        )

pico_add_extra_outputs(aqualink)

