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

void client_listener::on_peer_disconnected( shared_ptr<lnl::net_peer>& peer,
                                            lnl::disconnect_info& disconnectInfo) {
    printf("[UDP CLIENT]: Reconnecting...\n");
    // wair for 1 sec and attemt reconnection
    std::this_thread::sleep_for(std::chrono::seconds(1));
    owner->client.connect(owner->serverAdress, owner->writer);
}

void client_listener::on_network_error( const lnl::net_address& endpoint,
                                        uint32_t socketErrorCode,
                                        const string& message) {
    printf("[UDP CLIENT]: NETWORK ERROR with %s: [%d] %s\n", endpoint.address().c_str(), socketErrorCode, message.c_str());
}

void client_listener::on_network_receive(   shared_ptr<lnl::net_peer>& peer,
                                            lnl::net_data_reader& reader,
                                            uint8_t channelNumber,
                                            lnl::DELIVERY_METHOD deliveryMethod) {
    string receivedMessage;
    if (reader.try_read(receivedMessage)) {
        if (owner) {
            owner->handleReceivedData(receivedMessage);
        } else {
            printf("[UDP CLIENT]: No owner set for client_listener\n");
        }
    } else {
        printf("[UDP CLIENT]: Failed to read message from peer: %d\n", peer->remote_id());
    }
}

void client_listener::on_network_receive_unconnected(   const lnl::net_address& endpoint, 
                                                        lnl::net_data_reader& reader,
                                                        lnl::UNCONNECTED_MESSAGE_TYPE messageType){
    string message;
    printf("[UDP CLIENT]: Received UNCONNECTED communications from %s\n", endpoint.to_string().c_str());
    if (reader.try_read(message)){
        printf("[UDP CLIENT](RX): %s\n", message.c_str());
    } else {
        printf("[UDP CLIENT](RX): Failed to read message.\n");
    }
}

void client_listener::on_network_latency_update(shared_ptr<lnl::net_peer>& peer, int latency){
    printf("[UDP CLIENT]: Network latency of peer %d: %d ms\n", peer->connect_number(), latency);
}


void client_listener::on_connection_request(shared_ptr<lnl::net_connection_request>& request) {
    printf("[UDP CLIENT]: Connection requested, discarding\n");
    request->reject();
}

void client_listener::on_message_delivered(shared_ptr<lnl::net_peer>& peer, void* userData) {
    printf("[UDP CLIENT]: Message delivered to peer: %d\n", peer->remote_id());
}
