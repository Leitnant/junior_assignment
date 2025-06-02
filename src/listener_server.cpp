#include <cstdio>
#include <string>

#include "listener_server.h"
#include "udp_server.h"


void server_listener::setOwner(UDPServer* owner) {
    this->owner = owner;
}

void server_listener::on_peer_connected(shared_ptr<lnl::net_peer>& peer) {
    printf("[UDP SERVER]: Connected to client!\n");
    owner->peer = peer;
    owner->setConnectedState(true);
}

void server_listener::on_peer_disconnected( shared_ptr<lnl::net_peer>& peer,
                                            lnl::disconnect_info& disconnectInfo) {
    printf("\n[UDP SERVER]: Client disconnected.\n");
    owner->peer = nullptr;
    owner->setConnectedState(false);
}

void server_listener::on_network_error( const lnl::net_address& endpoint,
                                        uint32_t socketErrorCode,
                                        const string& message) {
    printf("[UDP SERVER]: NETWORK ERROR with %s: [%d] %s\n", endpoint.to_string().c_str(),
                                            socketErrorCode, message.c_str());
}

void server_listener::on_network_receive(   shared_ptr<lnl::net_peer>& peer,
                                            lnl::net_data_reader& reader,
                                            uint8_t channelNumber,
                                            lnl::DELIVERY_METHOD deliveryMethod) {
    string receivedMessage;
    if (reader.try_read(receivedMessage)) {
        if (owner) {
            owner->handleReceivedData(receivedMessage);
        } else {
            printf("[UDP SERVER]: No owner set for server_listener\n");
        }
    } else {
        printf("[UDP SERVER]: Failed to read message from peer %d\n", peer->remote_id());
    }
}

void server_listener::on_network_receive_unconnected(   const lnl::net_address& endpoint, 
                                                        lnl::net_data_reader& reader,
                                                        lnl::UNCONNECTED_MESSAGE_TYPE messageType){
    string message;
    printf("[UDP SERVER]: Received UNCONNECTED communications from %s\n", endpoint.to_string().c_str());
    if (reader.try_read(message)){
        printf("[UDP SERVER](RX): %s\n", message.c_str());
    } else {
        printf("[UDP SERVER](RX): Failed to read message.\n");
    }
}

void server_listener::on_network_latency_update(shared_ptr<lnl::net_peer>& peer, int latency){
    //Disabled due to no use
    //printf("[UDP SERVER]: Network latency of peer %d: %d ms\n", peer->connect_number(), latency);
}


void server_listener::on_connection_request(shared_ptr<lnl::net_connection_request>& request) {
    printf("[UDP SERVER]: Connection requested - accepting.\n");
    request->accept();
}

void server_listener::on_message_delivered(shared_ptr<lnl::net_peer>& peer, void* userData) {
    printf("[UDP SERVER]: Message delivered to peer %d\n", peer->remote_id());
}
