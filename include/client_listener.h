#pragma once
#include <lnl/lnl.h>
#include <memory>
#include <string>

class UDPClient;

using namespace std;

class client_listener : public lnl::net_event_listener {
public:
    client_listener() : owner(nullptr){};

    void setOwner(UDPClient* owner);

    void on_peer_connected(shared_ptr<lnl::net_peer>& peer) override;

    void on_peer_disconnected(shared_ptr<lnl::net_peer>& peer,
                              lnl::disconnect_info& disconnectInfo) override;

    void on_network_error(const lnl::net_address& endpoint,
                          uint32_t socketErrorCode,
                          const string& message) override;
        
    void on_network_receive(shared_ptr<lnl::net_peer>& peer,
                            lnl::net_data_reader& reader,
                            uint8_t channelNumber,
                            lnl::DELIVERY_METHOD deliveryMethod) override;

    void on_connection_request(shared_ptr<lnl::net_connection_request>& request) override;

    void on_message_delivered(shared_ptr<lnl::net_peer>& peer, void* userData) override;

private:
    UDPClient* owner;
};
