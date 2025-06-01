#pragma once
#include "client_listener.h"
#include <string>
#include <iostream>
#include <thread>
#include <lnl/net_address.h>


using namespace std;

class Drone;

class UDPClient {
public:
    UDPClient(lnl::net_address serverAdress) :  owner(nullptr),
                                                serverAdress(serverAdress),
                                                client(&listener) {
        listener.setOwner(this);
        printf("[UDP CLIENT]: Starting...\n");
        client.start();
        printf("[UDP CLIENT]: Connecting to %s:%d\n",(serverAdress.address()).c_str(), serverAdress.port());
        client.connect(serverAdress, writer);
        startCommunicationLoop();
    }

    void setOwner(Drone* owner) {
        this->owner = owner;
    }

    void send(const string& message) {
        writer.reset();
        writer.write(message);
        
        peer->send(writer, lnl::DELIVERY_METHOD::RELIABLE_ORDERED);

        printf("[UDP CLIENT](TX): %s", message.c_str());
    }

    void handleReceivedData(string message) {
        printf("[UDP CLIENT](RX): %s\n", message.c_str());
    }

    void startCommunicationLoop() {
        running = true;
        commThread = std::thread(&UDPClient::communicationLoop, this);
    }
    
    void stopCommunicationLoop() {
        running = false;
        if (commThread.joinable()) {
            commThread.join();
        }
    }

    void communicationLoop() {
        while (running) {
        client.poll_events();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        printf("[UDP CLIENT]: Communication stopped\n");
    }

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
