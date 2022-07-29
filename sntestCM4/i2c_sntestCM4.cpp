#include "i2c_sntestCM4.h"

i2c_device::i2c_device(int num, uint8_t *i2c_addr_t) {
    device_num = num;
    if (wiringPiSetup() != 0) {
        std::cout << "wiringPi error!" << std::endl;
    }
    for (int i=0; i<device_num; i++) {
        if (i2c_addr_t)   i2c_addr[i] = i2c_addr_t[i];
        device[i] = wiringPiI2CSetup(i2c_addr[i]);
        // std::cout << i << ": " << std::hex << i2c_addr[i] << " ";
    }
}

bool i2c_device::detect() {
    bool flag = false;
    for (int i=0; i<device_num; i++) {
        Rx_buf[i] = wiringPiI2CRead(device[i]); // TODO: read specific bits
        if (Rx_buf[i] != -1)    flag = true;
    }
    if (flag) {
        // Output
        std::cout << "huibao: ";
        for (int i=0; i<device_num; i++)    std::cout<< Rx_buf[i] << " ";
        std::cout << std::endl;
    }
    return flag;
}

void i2c_device::i2c_write(int* vel_pack) {
    for (int i=0; i<device_num; i++) {
        wiringPiI2CWrite(device[i], vel_pack[i]);
        delay(5);
    }
    // Output
    std::cout << "motor speed: ";
    for (int i=0; i<device_num; i++)    std::cout<< vel_pack[i] << " ";
    std::cout << std::endl;
}