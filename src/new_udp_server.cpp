#include "new_udp_server.h"

UDPServer::UDPServer(int serverPort, string clientIP, int clientPort) {
    if(!connection.create(serverPort, false)){
        printf("[UDP SERVER]: Failed to create socket.");
    }
    clientAddress = IpAddress(clientIP.c_str(), clientPort);
}

void UDPServer::sendArm() {
    mavlink_message_t msg;

    mavlink_msg_command_long_pack(
        SYSID,
        COMPID,
        &msg,
        1, // Target SYSID
        1, // Target COMPID
        MAV_CMD_DO_SET_MODE,
        0,
        MAV_MODE_FLAG_GUIDED_ENABLED,
        MAV_MODE_GUIDED_ARMED,
        0, 0, 0, 0, 0
    );

    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    size_t message_length = mavlink_msg_to_send_buffer(buffer, &msg);
    connection.sendto(buffer, message_length, clientAddress);
}

void UDPServer::sendDisarm() {
    mavlink_message_t msg;

    mavlink_msg_command_long_pack(
        SYSID,
        COMPID,
        &msg,
        1, // Target SYSID
        1, // Target COMPID
        MAV_CMD_DO_SET_MODE,
        0,
        MAV_MODE_FLAG_GUIDED_ENABLED,
        MAV_MODE_GUIDED_DISARMED,
        0, 0, 0, 0, 0
    );

    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    size_t message_length = mavlink_msg_to_send_buffer(buffer, &msg);
    connection.sendto(buffer, message_length, clientAddress);
}

void UDPServer::sendGoTo(float x, float y, float alt) {
    mavlink_message_t msg;

    mavlink_msg_command_long_pack(
        SYSID,
        COMPID,
        &msg,
        1, // Target SYSID
        1, // Target COMPID
        MAV_CMD_OVERRIDE_GOTO,
        0,
        MAV_GOTO_DO_CONTINUE,
        MAV_GOTO_HOLD_AT_SPECIFIED_POSITION,
        MAV_FRAME_LOCAL_NED,
        0, 
        x, 
        y, 
        -alt // flip the Z value to point up
    );
    
    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    size_t message_length = mavlink_msg_to_send_buffer(buffer, &msg);
    connection.sendto(buffer, message_length, clientAddress);
}

bool UDPServer::messageAvailable(){
    if (connection.available() > 0){
        return true;
    } else {
        return false;
    }
}

mavlink_message_t UDPServer::receiveMessage() {
    while (1) {
        if (connection.available() > 0) {
            char buffer[1024];
            IpAddress from;
            int bytesReceived = connection.recvfrom(buffer, sizeof(buffer), from);
            mavlink_message_t msg;
            mavlink_status_t status;
            
            for (size_t i = 0; i < bytesReceived; ++i) {
                if (mavlink_parse_char(MAVLINK_COMM_0, buffer[i], &msg, &status)) {
                    return msg;
                }
            }
        }
        this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    printf("[UDP SERVER]: Exited without getting complete message.\n");
}

mavlink_heartbeat_t UDPServer::decodeHeartbeat(mavlink_message_t message) {
    mavlink_heartbeat_t heartbeat;
    mavlink_msg_heartbeat_decode(&message, &heartbeat);
    return heartbeat;
}

mavlink_local_position_ned_t UDPServer::decodePosition(mavlink_message_t message) {
    mavlink_local_position_ned_t dronePosition;
    mavlink_msg_local_position_ned_decode(&message, &dronePosition);
    return dronePosition;
}

mavlink_command_ack_t UDPServer::decodeAck(mavlink_message_t message) {
    mavlink_command_ack_t ack;
    mavlink_msg_command_ack_decode(&message, &ack);
    return ack;
}