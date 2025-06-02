#pragma once
#include "udp_server.h"
#include <string>

using namespace std;

struct DroneState{
    float x = 0;
    float y = 0;
    float alt = 0;
    bool armed = false;
};

class GCS {
public:
    GCS(int port);

    void send_arm();

    void send_disarm();

    void send_goto(float x, float y, float alt);

    void updateDroneState(float x, float y, float alt, bool armed);

    void updateHeartbeatTime();

    void displayStatus();
    
    bool isConnected();


private:
    UDPServer udpServer;
    DroneState droneState;
};
