#include "client_listener.h"
#include "udp_client.h"
#include <cstdio>
#include <chrono>
#include <thread>
#include <string>


void client_listener::setOwner(UDPClient* owner) {
    this->owner = owner;
}

void client_listener::on_peer_connected(shared_ptr<lnl::net_peer>& peer) {
    printf("[UDP CLIENT]: Reconnected!\n");
}

void client_listener::on_peer_disconnected(shared_ptr<lnl::net_peer>& peer,
                            lnl::disconnect_info& disconnectInfo) {
    printf("[UDP CLIENT]: Reconnecting...\n");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    owner->client.connect(owner->serverAdress, owner->writer);
}

void client_listener::on_network_error(const lnl::net_address& endpoint,
                        uint32_t socketErrorCode,
                        const string& message) {
    printf("[UDP CLIENT]: NETWORK ERROR with %s: [%d] %s\n", endpoint.address().c_str(),
                                            socketErrorCode, message.c_str());
}

void client_listener::on_network_receive(shared_ptr<lnl::net_peer>& peer,
                                           lnl::net_data_reader& reader,
                                           uint8_t channelNumber,
                                           lnl::DELIVERY_METHOD deliveryMethod) {
    string receivedMessage;
    if (reader.try_read(receivedMessage)) {
        if (owner) {
            // Now UDPClient is a complete type – the method call is valid.
            owner->handleReceivedData(receivedMessage);
        } else {
            printf("[UDP CLIENT]: No owner set for client_listener\n");
        }
    } else {
        printf("[UDP CLIENT]: Failed to read message from peer %d\n", peer->remote_id());
    }
}

void client_listener::on_connection_request(shared_ptr<lnl::net_connection_request>& request) {
    printf("[UDP CLIENT]: CONNECTION REQUESTED\n");
    request->accept();
}

void client_listener::on_message_delivered(shared_ptr<lnl::net_peer>& peer, void* userData) {
    printf("[UDP CLIENT]: MESSAGE DELIVERED TO %d\n", peer->remote_id());
}
