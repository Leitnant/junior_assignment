
#include "listener_server.h"
#include "udp_server.h"
#include "gcs.h"

UDPServer::UDPServer(int port) :    owner(nullptr),
                                    server(&listener) {
    listener.setOwner(this);
    printf("[UDP SERVER]: Starting...\n");
    server.start(port);
    printf("[UDP SERVER]: Started on port %d\n", port);
    startCommunicationLoop();
}
UDPServer::~UDPServer(){
    stopCommunicationLoop();
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

void UDPServer::handleReceivedData(mavlink_message_t msg) {
    switch (msg.msgid){
        case MAVLINK_MSG_ID_HEARTBEAT:
            mavlink_heartbeat_t heartbeat;
            mavlink_msg_heartbeat_decode(&msg, &heartbeat);
            owner->updateHeartbeat(heartbeat.system_status);
            break;
        case MAVLINK_MSG_ID_LOCAL_POSITION_NED:
            mavlink_local_position_ned_t dronePos;
            mavlink_msg_local_position_ned_decode(&msg, &dronePos);
            owner->updateDronePos(dronePos.x, dronePos.y, -dronePos.z); //give data to gcs, but flip Z axis!
            break;
        case MAVLINK_MSG_ID_COMMAND_ACK:
            mavlink_command_ack_t ack;
            mavlink_msg_command_ack_decode(&msg, &ack);
            printf("(ACK): ");
            break;  
        default:
            printf("[UDP SERVER]: Received unexcpected message. %d\n", msg.msgid);
            break;
    }
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