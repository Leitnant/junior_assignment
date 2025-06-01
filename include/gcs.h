#pragma once
#include "udp_server.h"
#include <string>

using namespace std;

class GCS {
public:
    GCS(int port);

    // Poll for the latest telemetry from the drone.
    void update();

    // Display the received drone status.
    void displayStatus();

private:
    // Embedded UDP-server to accept drone telemetry.
    UDPServer udpServer;

    // Last received telemetry message
    string message;
};
