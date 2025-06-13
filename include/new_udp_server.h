#include <udp/simple_udp.h>
#include <mavlink.h>
#include <chrono>
#include <thread>
#include <string>

using namespace std;
using time_source = chrono::system_clock;
using time_point = time_source::time_point;
using duration = time_source::duration;
using namespace chrono_literals;

class UDPServer {
    public:
        UDPServer(int serverPort, string clientIP, int clientPort);
        
        ~UDPServer();

        void sendArm();
        
        void sendDisarm();

        void sendGoTo(float x, float y, float alt);

        bool pollSocket(int timeout_ms);

        mavlink_message_t receiveMessage();

        mavlink_heartbeat_t decodeHeartbeat(mavlink_message_t message);

        mavlink_local_position_ned_t decodePosition(mavlink_message_t message);

        mavlink_command_ack_t decodeAck(mavlink_message_t message);

    private:
        const int SYSID = 255;
        const int COMPID = 190;

        string message;
        IpAddress clientAddress;

        UdpSocket connection;
};