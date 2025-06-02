#pragma once
#include "listener_client.h"
#include <string>
#include <iostream>
#include <thread>
#include <lnl/net_address.h>
#include <mavlink.h>


using namespace std;

class Drone;

class UDPClient {
public:
    UDPClient(lnl::net_address serverAdress);

    void setOwner(Drone* owner);

    void sendBinary(const uint8_t* data, size_t length);

    void handleReceivedData(mavlink_message_t msg);

    void startCommunicationLoop();
    
    void stopCommunicationLoop();

    void communicationLoop();

private:
    atomic<bool> running = false;
    thread commThread;

    Drone* owner;
    client_listener listener;
    lnl::net_address serverAdress;
    lnl::net_data_writer writer;
    lnl::net_manager client;
    shared_ptr<lnl::net_peer> peer;
    friend class client_listener;
};
