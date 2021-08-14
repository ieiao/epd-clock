SET(CMAKE_SYSTEM_NAME Generic)
CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

PROJECT(binloader C)

SET(CROSS_COMPILE "msp430-elf-")
SET(CMAKE_C_COMPILER ${CROSS_COMPILE}gcc)
SET(CMAKE_CXX_COMPILER ${CROSS_COMPILE}g++)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -MMD -MP")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mmcu=msp430g2553")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffunction-sections")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fdata-sections")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Xlinker --gc-sections")

set(EXECUTABLE ${PROJECT_NAME}.elf)

add_executable(${EXECUTABLE} 
    binloader.c
    hal/gpio.c
    hal/uart.c
    hal/spi.c
    )

target_include_directories(${EXECUTABLE} PRIVATE
    hal
    )

add_custom_target(flash
    COMMAND sudo mspdebug rf2500 "prog ${PROJECT_NAME}.elf"
    DEPENDS ${EXECUTABLE})

add_custom_target(size
    COMMAND ${CROSS_COMPILE}size ${EXECUTABLE}
    DEPENDS ${EXECUTABLE})
