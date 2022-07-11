#include <iostream>
#include <thread>
#include <chrono>

#include "i2c_sntest.h"

int main() {
    static uint8_t motor_addr[1] = {0x31};
    // static uint8_t motor_addr[4] = {0x28,0x29,0x30,0x31};
    i2c_device motor(1, motor_addr);
    
    if (motor.detect())
    {
        std::cout << "device detected" << std::endl;
    }
    while ( 1 )
    {
        int Curr_Vel = motor.i2c_read();
        std::cout << "Current Velocity: " << Curr_Vel << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}