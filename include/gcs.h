#pragma once

#include <chrono>
#include "udp_server.h"

using namespace std;

struct DroneState{
    float x = 0;
    float y = 0;
    float alt = 0;
    int mode = 88;
};

class GCS {
public:
    GCS(int port);

    void send_arm();

    void send_disarm();

    void send_goto(float x, float y, float alt);

    void updateDronePos(float x, float y, float alt);

    void updateHeartbeat(int mode);

    void displayStatus();
    
    bool isConnected();


private:
    UDPServer udpServer;
    DroneState droneState;
    chrono::steady_clock::time_point lastHeartbeatTime;
};
