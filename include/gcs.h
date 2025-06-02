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
    //Messed up, virtual for gtesting
    virtual void updateDronePos(float x, float y, float alt);

    virtual void updateHeartbeat(int mode);

    void displayStatus();
    
    bool isConnected();

protected:
    UDPServer* getServerPointer(); // Messed up, need this for gtest

private:
    UDPServer udpServer;
    DroneState droneState;
    chrono::steady_clock::time_point lastHeartbeatTime;
};
