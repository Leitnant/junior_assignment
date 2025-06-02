#include "udp_client.h"
#include "drone.h"

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
    if (msg.msgid == MAVLINK_MSG_ID_COMMAND_LONG) {
        mavlink_command_long_t command;
        mavlink_msg_command_long_decode(&msg, &command);

        switch (command.command) {
            case MAV_CMD_DO_SET_MODE:
                switch (static_cast<int>(command.param2)) {
                    case 216:
                        printf("[UDP CLIENT]: Received ARM!\n");
                        owner->setArmedState(true);
                        break;
                    case 88:
                        printf("[UDP CLIENT]: Received DISARM!\n");
                        owner->setArmedState(false);
                        break;
                    default:
                        printf("[UDP CLIENT]: Received invalid mode: %d\n", (int)command.param2);
                        break;
                }
                owner->send_ack(command.param2);
                break;

            case MAV_CMD_OVERRIDE_GOTO:
                printf("[UDP CLIENT]: GOTO %f, %f, %f\n", command.param5, command.param6, -command.param7); //param7 (Z) needs to be flipped in NED point up
                owner->setTargetPos(command.param5, command.param6, -command.param7); //param7 (Z) needs to be flipped in NED to point up
                owner->send_ack(command.param2);
                break;

            default:
                printf("[UDP CLIENT]: Received unknown command: %d\n", (int)command.command);
                break;
        }
    }
}

void UDPClient::startCommunicationLoop() {
    running = true;
    peer = client.connect(serverAdress, writer);
    commThread = thread(&UDPClient::communicationLoop, this);
}

void UDPClient::stopCommunicationLoop() {
    running = false;
    peer = nullptr;
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
void UDPClient::setConnectionState(bool state){
    connected = state;
}

bool UDPClient::isConnected(){
    return connected;
}
