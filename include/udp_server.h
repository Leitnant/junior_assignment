#pragma once
#include "server_listener.h"
#include <string>
#include <iostream>

using namespace std;

class GCS;

class UDPServer {
public:
    UDPServer(int port) :   owner(nullptr),
                            server(&listener) {
        listener.setOwner(this);
        printf("[UDP SERVER]: Starting...\n");
        server.start(port);
        printf("[UDP SERVER]: Started on port %d", port);
        startCommunicationLoop();
    }
    
    void setOwner(GCS* owner) {
        this->owner = owner;
    }

    void send(const string& message) {
        writer.reset();
        writer.write(message);
        peer->send(writer, lnl::DELIVERY_METHOD::RELIABLE_ORDERED);

        printf("[UDP SERVER](TX): %s\n", message.c_str());
    }

    void handleReceivedData(string message) {
        printf("[UDP SERVER](RX): %s\n", message.c_str());
    }
    
    void startCommunicationLoop() {
        running = true;
        commThread = thread(&UDPServer::communicationLoop, this);
    }
    
    void stopCommunicationLoop() {
        running = false;
        if (commThread.joinable()) {
            commThread.join();
        }
    }

    void communicationLoop() {
        while (running) {
            server.poll_events();
            this_thread::sleep_for(chrono::milliseconds(10));
        }
        server.disconnect_all();
        printf("[UDP SERVER]: Communication stopped\n");
    }


private:
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
