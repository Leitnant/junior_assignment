#pragma once

#include <mavlink.h>
#include <iostream>
#include <string>
#include <thread>

#include "listener_server.h"

using namespace std;

class GCS;

class UDPServer {
public:
    UDPServer(int port);

    void setOwner(GCS* owner);

    void sendBinary(const uint8_t* data, size_t length);

    void handleReceivedData(mavlink_message_t msg);

    void startCommunicationLoop();

    void stopCommunicationLoop();

    void communicationLoop();

    void setPeer(shared_ptr<lnl::net_peer> peer);

    void setConnectedState(bool state);

    bool isConnected();

private:
    atomic<bool> connected = false;
    atomic<bool> running = false;
    thread commThread;

    GCS* owner;
    server_listener listener;
    lnl::net_address serverAdress;
    lnl::net_data_writer writer;
    lnl::net_manager server;
    shared_ptr<lnl::net_peer> peer;

    friend class server_listener;
};
