#include "server_listener.h"
#include "udp_server.h"
#include <cstdio>
#include <chrono>
#include <thread>
#include <string>


void server_listener::setOwner(UDPServer* owner) {
    this->owner = owner;
}

void server_listener::on_peer_connected(shared_ptr<lnl::net_peer>& peer) {
    printf("[UDP SERVER]: Reconnected!\n");
}

void server_listener::on_peer_disconnected(shared_ptr<lnl::net_peer>& peer,
                            lnl::disconnect_info& disconnectInfo) {
    printf("[UDP SERVER]: Peer disconnected.\n");
}

void server_listener::on_network_error(const lnl::net_address& endpoint,
                        uint32_t socketErrorCode,
                        const string& message) {
    printf("[UDP SERVER]: NETWORK ERROR with %s: [%d] %s\n", endpoint.address().c_str(),
                                            socketErrorCode, message.c_str());
}

void server_listener::on_network_receive(shared_ptr<lnl::net_peer>& peer,
                                           lnl::net_data_reader& reader,
                                           uint8_t channelNumber,
                                           lnl::DELIVERY_METHOD deliveryMethod) {
    string receivedMessage;
    if (reader.try_read(receivedMessage)) {
        if (owner) {
            // Now UDPClient is a complete type – the method call is valid.
            owner->handleReceivedData(receivedMessage);
        } else {
            printf("[UDP SERVER]: No owner set for client_listener\n");
        }
    } else {
        printf("[UDP SERVER]: Failed to read message from peer %d\n", peer->remote_id());
    }
}

void server_listener::on_connection_request(shared_ptr<lnl::net_connection_request>& request) {
    printf("[UDP SERVER]: CONNECTION REQUESTED\n");
    request->accept();
}

void server_listener::on_message_delivered(shared_ptr<lnl::net_peer>& peer, void* userData) {
    printf("[UDP SERVER]: MESSAGE DELIVERED TO %d\n", peer->remote_id());
}
