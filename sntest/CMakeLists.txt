enable_language(CXX)

include_directories(${PROJECT_SOURCE_DIR}/api)
include_directories(${PROJECT_SOURCE_DIR}/api/mraa)

file(GLOB TEST_FILES
    *.cpp
)

# add_executable (aio_cpp aio.cpp)
# add_executable (gpio_advanced_cpp gpio_advanced.cpp)
# add_executable (gpio_cpp gpio.cpp)
# add_executable (pwm_cpp pwm.cpp)
add_executable (sntest ${TEST_FILES})
# add_executable (spi_cpp spi.cpp)
# add_executable (uart_cpp uart.cpp)
# add_executable (iio_cpp iio.cpp)
# add_executable (led_cpp led.cpp)


# target_link_libraries (aio_cpp mraa stdc++)
# target_link_libraries (gpio_advanced_cpp mraa stdc++)
# target_link_libraries (gpio_cpp mraa stdc++)
# target_link_libraries (pwm_cpp mraa stdc++)
target_link_libraries (sntest mraa stdc++)
# target_link_libraries (spi_cpp mraa stdc++)
# target_link_libraries (uart_cpp mraa stdc++)
# target_link_libraries (iio_cpp mraa stdc++)
# target_link_libraries (led_cpp mraa stdc++)

# if (ONEWIRE)
#   add_executable (uart_ow_cpp uart_ow.cpp)
#   target_link_libraries (uart_ow_cpp mraa stdc++)
# endif ()
