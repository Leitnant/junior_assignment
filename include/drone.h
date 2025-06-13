#pragma once
#ifndef DRONE_H

#include <algorithm>
#include <thread>
#include <chrono>
#include <cmath>

#include "new_udp_client.h"

struct Position{
    float x = 0;
    float y = 0;
    float alt = 0;
};

class Drone {

public:

    Drone(float speed, float geofence, int clientPort);

    ~Drone();

    void updatePosition();

    void setCurrentPosition(float x, float y, float alt);

    void setArmedState(bool state);

    bool getArmedState();

    void setTargetPos(float x, float y, float alt);

    Position getTargetPos();

    void sendHeartbeat();

    void sendPosition();

    bool pollIncoming(int timeout_ms);

    void handleIncoming();

private:
    bool armed = false;
    float speed;
    float geofence; // geofence is a square with sides of 2*geofence, centered on 0,0,0
    
    Position currentPos;
    Position targetPos;

    thread clientThread;
    UDPClient client;
};
#endif