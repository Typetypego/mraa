enable_language(CXX)

find_package(fmt REQUIRED)
find_package(Threads REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/api)
include_directories(${PROJECT_SOURCE_DIR}/api/mraa)

include_directories(
    ${fmt_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/sntestCM4/share/zos/project/include
    ${PROJECT_SOURCE_DIR}/sntestCM4/share
    ${PROJECT_SOURCE_DIR}/sntestCM4/share/utils
    /home/sn/aarch64-toolchains/include
    /home/sn/aarch64-toolchains/fmt/include
)

set(LIBS
    ${LIBS}
    Threads::Threads
    fmt::fmt
    mraa
)

file(GLOB SHARED_SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/sntestCM4/share/udp/*.cpp
)

file(GLOB TEST_FILES
    ${PROJECT_SOURCE_DIR}/sntestCM4/*.cpp
)

# add_executable (aio_cpp aio.cpp)
# add_executable (gpio_advanced_cpp gpio_advanced.cpp)
# add_executable (gpio_cpp gpio.cpp)
# add_executable (pwm_cpp pwm.cpp)
add_executable (sntestCM4 ${TEST_FILES})
# add_executable (spi_cpp spi.cpp)
# add_executable (uart_cpp uart.cpp)
# add_executable (iio_cpp iio.cpp)
# add_executable (led_cpp led.cpp)


# target_link_libraries (aio_cpp mraa stdc++)
# target_link_libraries (gpio_advanced_cpp mraa stdc++)
# target_link_libraries (gpio_cpp mraa stdc++)
# target_link_libraries (pwm_cpp mraa stdc++)
target_link_libraries (sntestCM4 ${LIBS})
# target_link_libraries (spi_cpp mraa stdc++)
# target_link_libraries (uart_cpp mraa stdc++)
# target_link_libraries (iio_cpp mraa stdc++)
# target_link_libraries (led_cpp mraa stdc++)

# if (ONEWIRE)
#   add_executable (uart_ow_cpp uart_ow.cpp)
#   target_link_libraries (uart_ow_cpp mraa stdc++)
# endif ()
