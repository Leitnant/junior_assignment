#pragma once
#include "udp_server.h"
#include <string>

using namespace std;

class GCS {
public:
    GCS(int port);

    void send_arm();

    void send_disarm();

    void send_goto(float x, float y, float alt);

    void displayStatus();
    
    bool isConnected();

private:
    UDPServer udpServer;
};
