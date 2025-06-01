#pragma once
#include "udp_client.h"
#include <lnl/lnl.h>
#include <string>

enum class MAV_MODE {
    GUIDED_DISARMED,
    GUIDED_ARMED
};

class Drone {
public:
    Drone(float x, float y, float alt, lnl::net_address serverAdress);

    // Update simulation state
    void update();

    // Send telemetry
    void sendTelemetry();

    // Process incoming
    void processCommands();

    // Read the connection state
    bool isConnected();


private:
    // Drone variables:
    bool connected = false;
    float x, y, alt;
    MAV_MODE mode;
    UDPClient udpClient;
};
