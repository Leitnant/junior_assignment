#include "udp_client.h"

UDPClient::UDPClient(lnl::net_address serverAdress) :   owner(nullptr),
                                                        serverAdress(serverAdress),
                                                        client(&listener) {
    listener.setOwner(this);
    printf("[UDP CLIENT]: Starting...\n");
    client.start();
    printf("[UDP CLIENT]: Connecting to %s:%d\n",(serverAdress.address()).c_str(), serverAdress.port());
    startCommunicationLoop();
}

void UDPClient::setOwner(Drone* owner) {
    this->owner = owner;
}

void UDPClient::sendBinary(const uint8_t* data, size_t length) {
    if (!peer) {
        printf("[UDP CLIENT]: Failed to send binary - No connected peer!\n");
        return;
    }
    writer.reset();
    writer.write(data, 0, length);
    peer->send(writer, lnl::DELIVERY_METHOD::RELIABLE_ORDERED);
    printf("[UDP CLIENT](TX): Sent %zu bytes\n", length);
}

void UDPClient::handleReceivedData(mavlink_message_t msg) {
    if (msg.msgid == MAVLINK_MSG_ID_COMMAND_LONG){
        mavlink_command_long_t command;
        mavlink_msg_command_long_decode(&msg, &command);
        if (command.command == MAV_CMD_DO_SET_MODE){
            if (command.param2 == 216){
                printf("[UDP CLIENT]: Received ARM!\n");
            } else if (command.param2 == 88) {
                printf("[UDP CLIENT]: Received DISARM!\n");
            } else {
                printf("[UDP CLIENT]: Received invalid mode: %d\n", (int)command.param2);
            }
        } else if (command.command == MAV_CMD_OVERRIDE_GOTO) {
            printf("[UDP CLIENT]: GOTO %f, %f, %f\n", command.param5, command.param6, command.param7);
        }
    }
}

void UDPClient::startCommunicationLoop() {
    running = true;
    client.connect(serverAdress, writer);
    commThread = thread(&UDPClient::communicationLoop, this);
}

void UDPClient::stopCommunicationLoop() {
    running = false;
    if (commThread.joinable()) {
        commThread.join();
    }
}

void UDPClient::communicationLoop() {
    while (running) {
    client.poll_events();
    this_thread::sleep_for(chrono::milliseconds(10));
    }
    client.disconnect_all();
    printf("[UDP CLIENT]: Communication stopped\n");
}
