#include "main.h"

uint8_t tx_freq, tx_mode, rx_freq, rx_mode;
uint8_t robot_num = 0x01;
uint8_t robot_set = 0x08;

int16_t Vx_package = 0, Vy_package = 0, Vr_package = 0;  //下发机器人速度
int16_t Vx_package_last = 0, Vy_package_last = 0, Vr_package_last = 0;  //上一帧下发机器人速度
double Vr_cal = 0.0;
double rot_factor = 0.0;

uint8_t acc_set = 150;  //加速度限制 16ms内合成加速度最大值，单位cm/s
uint16_t acc_r_set = 60;  //
uint8_t DEC_FRAME = 0;

int Vel_Motor1 = 0, Vel_Motor2 = 0, Vel_Motor3 = 0, Vel_Motor4 = 0;
int vel_pack[4] = {0,0,0,0};
const double Vel_k2 = 0.520573;

double sin_angle[4] = {0.7071067811865476, -0.7071067811865476,
                       -0.7071067811865476,
                       0.7071067811865476};  // double sin_angle[4] = {sin(58),
                                             // -sin(58), -sin(45), sin(45)};
double cos_angle[4] = {-0.7071067811865476, -0.7071067811865476,
                       0.7071067811865476,
                       0.7071067811865476};  // double cos_angle[4] = {-cos(58),
                                              // -cos(58), cos(45), cos(45)};

uint8_t RX_Packet[25];
uint8_t TX_Packet[25] = {
    0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5,  //[0-8]
    0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5,        //[9-16]
    0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5};       //[17-24]

uint16_t Received_packet = 0;
uint16_t transmitted_packet = 0;
uint8_t received_packet_flag = 0;
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



void Is_Infrared();
void Is_BatteryLow_BootCharged();
void Buzzer_Off();
void Buzzer_Ring();
void initSPI();
void unpack(uint8_t *Packet);
void motion_planner();

int main() {
    static uint8_t motor_addr[4] = {0x28,0x29,0x30,0x31};
    i2c_device motor(4, motor_addr);
    while ( !motor.detect() ) {
        std::cout << "no motor detected" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    udp_init();
    
    int count = 0;
    while(1){
        // std::chrono::system_clock::now();

        motor.i2c_write(vel_pack);
        if (Received_packet) {
            unpack(rxbuf);
            if (received_packet_flag) {
                motion_planner();
            }
            Received_packet = 0;
        }
        // 
        if ( count == 1000 ){
            udp_sender(txbuf);
            std::cout << "send package" << std::endl;
            count = 0;
        }
        else{
            count++;
        }
    }
    return 0;
}

void Buzzer_Ring(){
    
}

void Buzzer_Off(){
    
}

//解包，到每个轮子的速度
void unpack(uint8_t *Packet) {
    if ((Packet[0] & 0xf0) == 0x40) {
        if ((robot_num == (Packet[1] & 0x0f)) && (Packet[0] & 0x08)) {
            received_packet_flag = 1;
            Vx_package = (Packet[2] & 0x7f) + ((Packet[17] & 0xc0) << 1);
            Vx_package = (Packet[2] & 0x80) ? ( -Vx_package ) : Vx_package;
            Vy_package = (Packet[3] & 0x7f) + ((Packet[17] & 0x30) << 3);
            Vy_package = (Packet[3] & 0x80) ? ( -Vy_package ) : Vy_package;
            Vr_package = (Packet[4] & 0x7f) + ((Packet[17] & 0x0f) << 7);
            Vr_package = (Packet[4] & 0x80) ? ( -Vr_package ) : Vr_package;
            // Robot_Is_Report = Packet[1] >> 7;
            // Robot_drib = (Packet[1] >> 4) & 0x03;
            // Robot_Chip_Or_Shoot = ( Packet[1] >> 6 ) & 0x01;
            // Robot_Boot_Power = Packet[21] & 0x7f;
            // use_dir = Packet[21] & 0x80;
        }
        // else if((robot_num == (Packet[5] & 0x0f)) && (Packet[0] & 0x04)){
        //     received_packet_flag = 1;
        //     Vx_package = (Packet[6] & 0x7f) + ((Packet[18] & 0xc0) << 1);
        //     Vx_package = (Packet[6] & 0x80) ? ( -Vx_package ) : Vx_package;
        //     Vy_package = (Packet[7] & 0x7f) + ((Packet[18] & 0x30) << 3);
        //     Vy_package = (Packet[7] & 0x80) ? ( -Vy_package ) : Vy_package;
        //     Vr_package = (Packet[8] & 0x7f) + ((Packet[18] & 0x0f) << 7);
        //     Vr_package = (Packet[8] & 0x80) ? ( -Vr_package ) : Vr_package;
        //     Robot_Is_Report = Packet[5] >> 7;
        //     Robot_drib = (Packet[5] >> 4) & 0x03;
        //     Robot_Chip_Or_Shoot = ( Packet[5] >> 6 ) & 0x01;
        //     Robot_Boot_Power = Packet[22] & 0x7f;
        //     use_dir = Packet[22] & 0x80;
        // }
        // else if((robot_num == (Packet[9] & 0x0f)) && (Packet[0] & 0x02)){
        //     received_packet_flag = 1;
        //     Vx_package = (Packet[10] & 0x7f) + ((Packet[19] & 0xc0) << 1);
        //     Vx_package = (Packet[10] & 0x80) ? ( -Vx_package ) : Vx_package;
        //     Vy_package = (Packet[11] & 0x7f) + ((Packet[19] & 0x30) << 3);
        //     Vy_package = (Packet[11] & 0x80) ? ( -Vy_package ) : Vy_package;
        //     Vr_package = (Packet[12] & 0x7f) + ((Packet[19] & 0x0f) << 7);
        //     Vr_package = (Packet[12] & 0x80) ? ( -Vr_package ) : Vr_package;
        //     Robot_Is_Report = Packet[9] >> 7;
        //     Robot_drib = (Packet[9] >> 4) & 0x03;
        //     Robot_Chip_Or_Shoot = ( Packet[9] >> 6 ) & 0x01;
        //     Robot_Boot_Power = Packet[23] & 0x7f;
        //     use_dir = Packet[23] & 0x80;
        // }
        // else if((robot_num == (Packet[13] & 0x0f)) && (Packet[0] & 0x01)){
        //     received_packet_flag = 1;
        //     Vx_package = (Packet[14] & 0x7f) + ((Packet[20] & 0xc0) << 1);
        //     Vx_package = (Packet[14] & 0x80) ? ( -Vx_package ) : Vx_package;
        //     Vy_package = (Packet[15] & 0x7f) + ((Packet[20] & 0x30) << 3);
        //     Vy_package = (Packet[15] & 0x80) ? ( -Vy_package ) : Vy_package;
        //     Vr_package = (Packet[16] & 0x7f) + ((Packet[20] & 0x0f) << 7);
        //     Vr_package = (Packet[16] & 0x80) ? ( -Vr_package ) : Vr_package;
        //     Robot_Is_Report = Packet[13] >> 7;
        //     Robot_drib = (Packet[13] >> 4) & 0x03;                                      
        //     Robot_Chip_Or_Shoot = ( Packet[13] >> 6 ) & 0x01;
        //     Robot_Boot_Power = Packet[24] & 0x7f;
        //     use_dir = Packet[24] & 0x80;
        // }
        else 
            received_packet_flag = 0;
    }
    else
        received_packet_flag = 0;
}

void motion_planner() {
    int16_t acc_x = 0;
    int16_t acc_y = 0;
    double acc_whole = 0;
    double sin_x = 0;
    double sin_y = 0;
    if(sqrt(Vx_package_last * Vx_package_last + Vy_package_last * Vy_package_last) > 325.0) {
        acc_set = 25.0;	// 4.18 test
        DEC_FRAME++;
    }
    else {
        DEC_FRAME = 0;
        acc_set = 20.0;
    }
    acc_x = Vx_package - Vx_package_last;
    acc_y = Vy_package - Vy_package_last; 
    acc_whole = acc_x * acc_x + acc_y * acc_y ;
    acc_whole = sqrt(acc_whole);
    sin_x = acc_x / acc_whole;
    sin_y = acc_y / acc_whole;

    if (acc_whole > acc_set) {
        acc_whole = acc_set;
        acc_x = acc_whole * sin_x;
        acc_y = acc_whole * sin_y;
        Vx_package = Vx_package_last + acc_x;
        Vy_package = Vy_package_last + acc_y; 
    }
    
    //if((Vr_package - Vr_package_last)  >  acc_r_set) Vr_package = Vr_package_last + acc_r_set;
    //if((Vr_package - Vr_package_last)  < -acc_r_set) Vr_package = Vr_package_last - acc_r_set;
     //Vr_fuck = Vr_package;
    //Vr_package = Vr_package / 160.0;
    // Vr_fucked = Vr_package;
    
    // double type
    Vr_cal = Vr_package / 160.0;
    if(!DEC_FRAME) {
        Vx_package_last = Vx_package;
        Vy_package_last = Vy_package;
    }
    else {
        Vx_package_last += acc_x;
        Vy_package_last += acc_y;
    }
    
    // no use of v_r limit
    // Vr_package_last = Vr_package;
    
    vel_pack[0] = ((sin_angle[0]) * Vx_package + (cos_angle[0]) * Vy_package - 8.2 * Vr_cal) * Vel_k2;
    vel_pack[1] = ((sin_angle[1]) * Vx_package + (cos_angle[1]) * Vy_package - 8.2 * Vr_cal) * Vel_k2;
    vel_pack[2] = ((sin_angle[2]) * Vx_package + (cos_angle[2]) * Vy_package - 8.2 * Vr_cal) * Vel_k2;
    vel_pack[3] = ((sin_angle[3]) * Vx_package + (cos_angle[3]) * Vy_package - 8.2 * Vr_cal) * Vel_k2;
}