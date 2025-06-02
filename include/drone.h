#pragma once
#include "udp_client.h"
#include <lnl/lnl.h>

class Drone {

public:
    Drone(float x, float y, float alt, lnl::net_address serverAdress);

    void send_heartbeat();

    void send_ack();

    bool isConnected();

private:
    bool connected = false;
    bool armed = false;
    float x, y, alt;
    UDPClient udpClient;
};
