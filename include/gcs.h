#pragma once
#include "udp_server.h"
#include <string>

using namespace std;

class GCS {
public:
    GCS(int port);

    void send_arm();

    void send_goto();

    void send_geofence();

    void update();

    void displayStatus();

private:
    UDPServer udpServer;
};
