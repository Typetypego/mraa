#include "i2c_sntest.h"

i2c_device::i2c_device(int num, uint8_t *i2c_addr_t, int i2c_bus) : devices(4, {0}) {
    device_num = num;
    // if (wiringPiSetup() != 0) {
    //     std::cout << "wiringPi error!" << std::endl;
    // }
    for (int i=0; i<device_num; i++) {
        if (i2c_addr_t)   i2c_addr[i] = i2c_addr_t[i];
        device.address(i2c_addr[i]);

        devices[i] = device;
    }
}

bool i2c_device::detect() {
    bool flag = false;
    for (int i=0; i<device_num; i++) {
        Rx_buf[i] = devices[i].readByte();
        // Rx_buf[i] = wiringPiI2CRead(device[i]);
        if (Rx_buf[i] != -1)    flag = true;
    }
    if (flag) {
        // Output
        // std::cout << "huibao: ";
        // for (int i=0; i<device_num; i++)    std::cout<< Rx_buf[i] << " ";
        // std::cout << std::endl;
    }
    return flag;
}

void i2c_device::i2c_write(int* vel_pack) {
    for (int i=0; i<device_num; i++) {
        devices[i].writeByte(vel_pack[i]);
        // wiringPiI2CWrite(device[i], vel_pack[i]);
        // delay(5);    // FIX
    }
    // Output
    // std::cout << "motor speed: ";
    // for (int i=0; i<device_num; i++)    std::cout<< vel_pack[i] << " ";
    // std::cout << std::endl;
}

int i2c_device::i2c_read() {
    int Curr_Vel = -1;
    for (int i=0; i<device_num; i++) {
        Curr_Vel = devices[i].readByte();
        // wiringPiI2CWrite(device[i], vel_pack[i]);
        // delay(5);    // FIX
    }
    return Curr_Vel;
    // Output
    // std::cout << "motor speed: ";
    // for (int i=0; i<device_num; i++)    std::cout<< vel_pack[i] << " ";
    // std::cout << std::endl;
}
