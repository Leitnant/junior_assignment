#pragma once
#include "server_listener.h"
#include <string>
#include <iostream>

using namespace std;

class GCS;

class UDPServer {
public:
    UDPServer() : port(14550) {}

    // For simulation: initialize on a given port.
    void init(unsigned short p) {
        port = p;
        cout << "UDPServer: Initialized on port " << port << "\n";
    }

    bool send(const std::string& message) {
        cout << "UDPServer: Sending message: " << message << "\n";
        return true;
    }

    void handleReceivedData(string message);


private:
    GCS* owner;
    server_listener listener;
    unsigned short port;
};
