#include "main.h"

uint8_t tx_freq, tx_mode, rx_freq, rx_mode;  //频点

int vel_pack[4] = {0,0,0,0};
int vel_encoder[4] = {0,0,0,0};

uint16_t Received_packet = 0;
uint16_t transmitted_packet = 0;
uint8_t received_packet_flag = 0;
uint32_t Total_Missed_Package_Num = 0, Period_2ms_Since_Last_Zero_Motion_Planner = 0;
uint8_t Comm_Down_Flag = 0;
uint16_t nopack_count = 0;


// uint8_t packet_flag = 0;
// uint8_t to_transmit_packet = 0;
// uint16_t miss_packet = 0;  
// uint8_t stop_flag = 0;
// uint16_t heart_toggle_flag = 0;
// uint8_t ADC_circle_i = 0;    

uint8_t txbuf[25] = "testtest";
uint8_t rxbuf[25] = {0x0};
// uint8_t rxbuf[25] = {0x48, 0x01, 0x14, 0x14, 0x50, 0x00, 0x00, 0x00,
//                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};


int main() {
    // Udp Init
    udp_init();
    
    // Robot Init
    robotz zjunlict;
    zjunlict.robot_num = 0x0f;

    // int count_test = 0;

    while (1)
    {   
        // Motor control
        zjunlict.motor.i2c_write(vel_pack);

        // TODO
        // Infrare
        // Dribble

        // Detect receive pack
        if (Received_packet) {
            // Callback of udp receive
            received_packet_flag = zjunlict.unpack(rxbuf);
            if (received_packet_flag) {
                // Correct package
                // zjunlict.motion_planner(vel_pack);   // Already set to regular()
                zjunlict.regular(vel_pack);

                Received_packet++;
                Total_Missed_Package_Num = 0;
                Comm_Down_Flag = 0;
            }
            Received_packet = 0;
        }

        if (Comm_Down_Flag == 1){
            // When the robot loss packs
            Period_2ms_Since_Last_Zero_Motion_Planner ++;
            if (Period_2ms_Since_Last_Zero_Motion_Planner>=8){
                nopack_count++;
                if ( nopack_count == 10 )
                {
                    // Restart udp
                    udp_init();
                    nopack_count = 0;
                    // Buzzer_Ring();
                }
                
                // Set to 0
                Period_2ms_Since_Last_Zero_Motion_Planner = 0;
                zjunlict.stand(vel_pack);
            }
        }else {
            if (received_packet_flag == 0) {
                Total_Missed_Package_Num ++;
                if (Total_Missed_Package_Num >= 50) { // 500ms Missing Package -> Every 16ms Do Motion Planner
                    Comm_Down_Flag = 1;

                    // Set to 0
                    Period_2ms_Since_Last_Zero_Motion_Planner = 0;
                    zjunlict.stand(vel_pack);
                }
            }
        }

        // TODO: Shoot and chip-need to determine time flag
        // if(chipshoot_timerdelay_flag < 1000)
        //     chipshoot_timerdelay_flag++;
        // Shoot_Chip();                    //平挑射

        // Test output
        // if ( count_test == 1000 ) {
        //     udp_sender(txbuf);
        //     std::cout << "send package" << std::endl;
        //     count_test = 0;
        // }else {
        //     count_test++;
        // }
        // motor.i2c_read(vel_encoder);

        // Encoder read
        std::string str_t;
        for(int i=0; i<zjunlict.test_motor_num; i++) {
            str_t += fmt::format("Motor {}  Vel {}    ", i, vel_encoder[i]);
        }
        std::cout << str_t << std::endl;
        // Send back
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
