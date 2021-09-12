SET(CMAKE_SYSTEM_NAME Generic)
CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

PROJECT(clock C)

SET(CROSS_COMPILE "msp430-elf-")
SET(CMAKE_C_COMPILER ${CROSS_COMPILE}gcc)
SET(CMAKE_CXX_COMPILER ${CROSS_COMPILE}g++)

set(EXECUTABLE ${PROJECT_NAME}.elf)

add_executable(${EXECUTABLE} 
    clock.c
    sys/core.c
    sys/ui.c
    sys/font.c
    drivers/platform.c
    drivers/pm.c
    drivers/gpio.c
    drivers/uart.c
    drivers/spi.c
    drivers/i2c.c
    drivers/epd-2in9.c
    drivers/timer.c
    lib/pcf8563.c
    lib/spi-nor.c
    lib/time.c
    lib/search.c
    lib/utf8-cov.c
    lib/button.c
)

target_include_directories(${EXECUTABLE} PRIVATE
    drivers
    lib
    sys
)

target_compile_options(${EXECUTABLE} PRIVATE
    -O2
    -mmcu=msp430g2553
    -ffunction-sections
    -fdata-sections
    -Wall
)

target_link_options(${EXECUTABLE} PRIVATE
    -O2
    -mmcu=msp430g2553
    -ffunction-sections
    -fdata-sections
    -static
    -Xlinker --gc-sections
)

add_custom_target(flash
    COMMAND sudo mspdebug rf2500 "prog ${PROJECT_NAME}.elf"
    DEPENDS ${EXECUTABLE})

add_custom_target(size
    COMMAND ${CROSS_COMPILE}size ${EXECUTABLE}
    DEPENDS ${EXECUTABLE})
