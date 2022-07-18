#include "wifi_sntestCM4.h"

void _cb(const void* p,size_t lens){
    std::string s(static_cast<const char*>(p),lens);
    // 更新最新数据包
    Received_packet = 1;
    std::copy(std::begin(s), std::end(s), std::begin(rxbuf));
}

void udp_init()
{
    std::thread receiveThread(udp_receiver);
    receiveThread.detach();
}

// Receiver
void udp_receiver()
{
    int port = 30001;
    asio::ip::udp::endpoint listen_ep(asio::ip::address::from_string("0.0.0.0"),30001);
    zos::udp::socket socket(listen_ep,_cb);
    zos::__io::GetInstance().run();
}

void udp_sender(uint8_t *txbuf)
{
    int port = 30002;
    zos::udp::socket socket;
    asio::ip::udp::endpoint receiver_endpoint(asio::ip::address::from_string("127.0.0.1"),port);
    socket.send_to(fmt::format("count : {}",txbuf),receiver_endpoint);   // TODO: protobuf
    // std::cout << "send 1 message" <<std::endl;
}














// void UdpTest::checkstatus()
// {
//     // Search
//     receiveThread = new std::thread([=] {receive();});
//     receiveThread->detach();
//     std::cout << "Start searching devices ..." << std::endl;
//     std::this_thread::sleep_for(std::chrono::seconds(1));
// //    stopreceive();
// }


// void UdpTest::refreship(){
//     // Store Data
// }


// int UdpTest::receive() {
//     asio::ip::udp::endpoint listen_ep(asio::ip::address::from_string("0.0.0.0"), port);
//     zos::udp::socket socket(listen_ep,_cb);
// //    std::scoped_lock lock(_mutex);
//     zos::__io::GetInstance().run();
// }


// void UdpTest::stopreceive() {
//     zos::__io::GetInstance().stop();
//     delete receiveThread;
// }