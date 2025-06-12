#pragma once
#ifndef GCS_H

#include <mavlink.h>
#include <chrono>
#include <iostream>

#include "new_udp_server.h"

using namespace std;

struct DroneState{
    float x = 0;
    float y = 0;
    float alt = 0;
    int mode = 88;
    chrono::steady_clock::time_point lastHeartbeatTime;
};

class GCS {
public:
    GCS(int serverPort);
    ~GCS();

    void setDroneState(float x, float y, float alt, int mode);

    DroneState getDroneState();

    void updateDronePos(float x, float y, float alt);

    void updateHeartbeat(int mode);

    void startDisplayLoop();

    void stopDisplayLoop();

    void displayStatus();

    void sendArm();

    void sendDisarm();

    void sendGoTo(float x, float y, float alt);
    
    bool pollIncoming(int timeout_ms);

    void handleIncoming();
private:
    atomic<bool> dispRunning = false;
    thread dispThread;
    
    DroneState droneState;

    UDPServer server;
};
#endif