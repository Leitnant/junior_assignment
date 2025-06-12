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

class UDPClient {
    public:
        UDPClient(int clientPort, string serverAddress, int serverPort);

        void sendHeartbeat(bool armed);

        void sendPosition(float x, float y, float alt);

        void sendAck(mavlink_command_long_t command, int senderSysID, int senderCompID);

        bool pollSocket(int timeout_ms);

        mavlink_command_long_t receiveCommand();

    private:
        const int SYSID = 1;
        const int COMPID = 1;

        string message;
        IpAddress serverAddress; 

        time_point lastMessageTime;
        duration messageInterval;

        UdpSocket connection;
        
};