#include <string>

#include "listener_server.h"
#include "udp_server.h"

UDPServer::UDPServer(int port) :    owner(nullptr),
                                    server(&listener) {
    listener.setOwner(this);
    printf("[UDP SERVER]: Starting...\n");
    server.start(port);
    printf("[UDP SERVER]: Started on port %d\n", port);
    startCommunicationLoop();
}

void UDPServer::setOwner(GCS* owner) {
    this->owner = owner;
}

void UDPServer::sendBinary(const uint8_t* data, size_t length) {
    if (!peer) {
        printf("[UDP SERVER]: Failed to send binary - No connected peer!\n");
        return;
    }
    writer.reset();
    writer.write(data, 0, length);
    peer->send(writer, lnl::DELIVERY_METHOD::RELIABLE_ORDERED);
    printf("[UDP SERVER](TX): Sent %zu bytes\n", length);
}

void UDPServer::handleReceivedData(string message) {
    printf("[UDP SERVER](RX): %s\n", message.c_str());
}

void UDPServer::startCommunicationLoop() {
    running = true;
    commThread = thread(&UDPServer::communicationLoop, this);
}

void UDPServer::stopCommunicationLoop() {
    running = false;
    if (commThread.joinable()) {
        commThread.join();
    }
}

void UDPServer::communicationLoop() {
    while (running) {
        server.poll_events();
        this_thread::sleep_for(chrono::milliseconds(10));
    }
    server.disconnect_all();
    printf("[UDP SERVER]: Communication stopped\n");
}

void UDPServer::setPeer(shared_ptr<lnl::net_peer> peer){
    this->peer = peer;
}

void UDPServer::setConnectedState(bool state) {
    this->connected = state;
}

bool UDPServer::isConnected() {
    return this->connected;
}