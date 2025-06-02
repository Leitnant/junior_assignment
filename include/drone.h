#pragma once

#include "udp_client.h"
#include <lnl/lnl.h>

struct Position{
    float x = 0;
    float y = 0;
    float alt = 0;
};

class Drone {

public:
    Drone(float speed, float geofence, lnl::net_address serverAdress);

    void updatePosition();

    void setArmedState(bool state);

    void setTargetPos(float x, float y, float alt);

    void send_heartbeat();

    void send_position();

    void send_ack(int command);

    bool isConnected();

    bool isArmed();

private:
    bool armed = false;
    Position currentPos;
    Position targetPos;
    float geofence; // geofence is a square with sides of 2*geofence, centered on 0,0,0
    float speed;
    UDPClient udpClient;
};
