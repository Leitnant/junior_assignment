#include "new_udp_client.h"

UDPClient::UDPClient(int clientPort, string serverIP, int serverPort) {
    if(!connection.create(clientPort, false)){
        printf("[UDP SERVER]: Failed to create socket.");
        while (1) {}
    }
    serverAddress = IpAddress(serverIP.c_str(), serverPort);
}   

void UDPClient::sendHeartbeat(bool armed) {
    int modeFlags = MAV_MODE_FLAG_GUIDED_ENABLED;
    if (armed) {
        modeFlags |= MAV_MODE_FLAG_SAFETY_ARMED;
    }
    mavlink_message_t heartbeat;
    mavlink_msg_heartbeat_pack(
        SYSID,
        COMPID,
        &heartbeat,
        MAV_TYPE_QUADROTOR,
        MAV_AUTOPILOT_GENERIC,
        modeFlags,
        0,
        MAV_STATE_ACTIVE
    );
    
    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    size_t message_length = mavlink_msg_to_send_buffer(buffer, &heartbeat);
    connection.sendto(buffer, message_length, serverAddress);
}

void UDPClient::sendPosition(float x, float y, float alt) {
    mavlink_message_t position;

    mavlink_msg_local_position_ned_pack(
        SYSID,
        COMPID,
        &position,
        0,
        x,
        y,
        -alt, // convert altitude to NED format
        0, 0, 0
    );

    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    size_t message_length = mavlink_msg_to_send_buffer(buffer, &position);
    connection.sendto(buffer, message_length, serverAddress);
}

void UDPClient::sendAck(mavlink_command_long_t command, int senderSysID, int senderCompID) {
    mavlink_message_t ack;
    int commandID = command.param2;
    
    mavlink_msg_command_ack_pack(
        SYSID,
        COMPID,
        &ack,
        commandID,
        MAV_RESULT_ACCEPTED,
        0,
        0,
        senderSysID,
        senderCompID
    );

    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    size_t message_length = mavlink_msg_to_send_buffer(buffer, &ack);
    connection.sendto(buffer, message_length, serverAddress);
}

bool UDPClient::pollSocket(int timeout_ms) {
    return connection.poll_read(timeout_ms);
}

mavlink_command_long_t UDPClient::receiveCommand() {
    mavlink_command_long_t errorPlaceholder;

    while (connection.available()) {
        char buffer[1024];
        IpAddress from;
        int bytesReceived = connection.recvfrom(buffer, sizeof(buffer), from);
        if (bytesReceived <= 0) {
            errorPlaceholder.command = 1;
            return errorPlaceholder; // not connected
        }
        printf("%d",bytesReceived);
        mavlink_message_t msg;
        mavlink_status_t status;
        
        for (size_t i = 0; i < bytesReceived; ++i) {
            if (mavlink_parse_char(MAVLINK_COMM_0, buffer[i], &msg, &status)) {
                mavlink_command_long_t command;
                mavlink_msg_command_long_decode(&msg, &command);
                return command;
            }
            
        }
        this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    errorPlaceholder.command = 0; // did not get full packet
    return errorPlaceholder;
}