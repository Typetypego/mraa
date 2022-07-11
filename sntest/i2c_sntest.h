#ifndef I2C_H
#define I2C_H

// #include "main.h"
#include <iostream>

/* mraa headers */
#include "mraa/common.hpp"
#include "mraa/i2c.hpp"

#define MAX_MOTOR 4

class i2c_device{
public:
    i2c_device(int num = MAX_MOTOR, uint8_t *i2c_addr_t = NULL, int i2c_bus=0);
    bool detect();
    void i2c_write(int* vel_pack);
    int i2c_read();

private:
    int device_num = 0;
    uint8_t i2c_addr[MAX_MOTOR] = {0x28,0x29,0x30,0x31};
    mraa::I2c device = mraa::I2c(0);
    int Rx_buf[MAX_MOTOR];
};

#endif