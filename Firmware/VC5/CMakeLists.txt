# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NEVER EDIT THE NEXT LINES for Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 1.5.1)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

# set fully de-optimized debug build, no optimization
set(PICO_DEOPTIMIZED_DEBUG 1)

#--------------------
# Pico SDK
#--------------------
set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
set(PICO_SDK_PATH ${CMAKE_CURRENT_LIST_DIR}/pico-sdk)
include(pico_sdk_import.cmake)
#--------------------


#--------------------
# FreeRTOS
#--------------------
set(FREERTOS_KERNEL_PATH  ${CMAKE_CURRENT_LIST_DIR}/FreeRTOS)

# Select the heap port.  values between 1-4 will pick a heap.
set(FREERTOS_HEAP "3" CACHE STRING "" FORCE)

# Select the native compile PORT, see FreeRTOS/freertos_kernel-src/portable/CMakLists.cmake
set(FREERTOS_PORT "GCC_ARM_CM0" CACHE STRING "" FORCE)
#--------------------


#--------------------
# Waveshare
#--------------------
# include Paint lib by Waveshare
include(${CMAKE_CURRENT_LIST_DIR}/paint/paint.cmake)
#--------------------


#--------------------
# Project setup
#--------------------
# project sets the ${PROJECT_NAME} variable as well
project(VC5 C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add main executable
add_executable(${PROJECT_NAME})

#  Default name is the project name, version 1.0
pico_set_program_name(${PROJECT_NAME} "VC5")
pico_set_program_version(${PROJECT_NAME} "1.0")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(${PROJECT_NAME} 1)
pico_enable_stdio_usb(${PROJECT_NAME} 0)

# use FreeRTOS as OS
target_compile_definitions(${PROJECT_NAME} PRIVATE
    CFG_TUSB_OS=OPT_OS_FREERTOS
)

# needed for using a PIO
pico_generate_pio_header(
    ${PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}/led_WS2812.pio
    OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}
    )

# Add the standard include files to the build
target_include_directories(${PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
)

# Add the standard libraries to the build
target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    cmsis_core
    )
#--------------------


#--------------------
# Sources and SDK libraries
#--------------------
# Add custom source files to the build
target_sources(${PROJECT_NAME} PRIVATE
    main.cpp
    FreeRTOS_support.cpp
    VC5_RP2040.cpp
    MCP23017.cpp
    rotary_bank.cpp
    rotary.cpp
    usb_descriptors.cpp
    usb_device.cpp
    Mutex.cpp
    usb_cmd_processor.cpp
    led_WS2812.cpp
    SH1106_disp_driver.cpp
    VC5_display_row.cpp
    )

# Add any user requested libraries
target_link_libraries(${PROJECT_NAME}
    hardware_i2c
    pico_unique_id
    tinyusb_device
    tinyusb_board
    hardware_pio
    hardware_clocks
    hardware_timer
    hardware_spi
    paint_lib
    )
#--------------------


#--------------------
# FreeRTOS
#--------------------
# Add the freertos_config for FreeRTOS-Kernel
add_library(freertos_config INTERFACE)

target_include_directories(freertos_config INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
    )

# add FreeRTOS to the main build
add_subdirectory(${FREERTOS_KERNEL_PATH} FreeRTOS-Kernel)

target_link_libraries(${PROJECT_NAME}
    freertos_kernel
    freertos_config
    )
#--------------------


pico_add_extra_outputs(${PROJECT_NAME})
