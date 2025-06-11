#include "drone.h"

#include <algorithm>
#include <thread>
#include <chrono>
#include <cmath>

#define SYSID 1
#define COMPID 1

using namespace std;

Drone::Drone(float speed, float geofence, lnl::net_address serverAdress) : speed(speed), geofence(geofence), udpClient(serverAdress){
    udpClient.setOwner(this);
    printf("[DRONE]: Drone setup complete!\n");
}

void Drone::updatePosition(){
    float dx = targetPos.x - currentPos.x;
    float dy = targetPos.y - currentPos.y;
    float dz = targetPos.alt - currentPos.alt;
    
    float distance = sqrt(dx * dx + dy * dy + dz * dz);

    if (distance == 0){return;}
        
    float step = min(speed, distance);

    currentPos.x = clamp(currentPos.x + (dx / distance) * step, -geofence, geofence);
    currentPos.y = clamp(currentPos.y + (dy / distance) * step, -geofence, geofence);
    currentPos.alt = clamp(currentPos.alt + (dz / distance) * step, -geofence, geofence);
}

void Drone::setArmedState(bool state){
    armed = state;
}

void Drone::setTargetPos(float x, float y, float alt){
    targetPos.x = x;
    targetPos.y = y;
    targetPos.alt = alt;
}

void Drone::send_heartbeat(){
    mavlink_message_t heartbeat;
    int mode = MAV_MODE_FLAG_GUIDED_ENABLED;

    if (armed) {
        mode |= MAV_MODE_FLAG_SAFETY_ARMED;
    }

    mavlink_msg_heartbeat_pack(
        SYSID,
        COMPID,
        &heartbeat,
        MAV_TYPE_QUADROTOR,
        MAV_AUTOPILOT_GENERIC,
        mode,
        0,
        MAV_STATE_ACTIVE
    );

    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    size_t message_length = mavlink_msg_to_send_buffer(buffer, &heartbeat);

    this->udpClient.sendBinary(buffer, message_length);
}

void Drone::send_position(){
    mavlink_message_t position;
    
    mavlink_msg_local_position_ned_pack(
        1, //system ID
        1, //component ID
        &position,
        0, //ms since system boot, won't be sending
        currentPos.x,
        currentPos.y,
        -currentPos.alt, //to convert altitude to NED format
        0, 0, 0 // velocities, not simulating atm.
    );

    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    size_t message_length = mavlink_msg_to_send_buffer(buffer, &position);

    this->udpClient.sendBinary(buffer, message_length);
}

void Drone::send_ack(int command){
    mavlink_message_t ack;

    mavlink_msg_command_ack_pack(
        1, //system ID
        1, //component ID
        &ack,
        command, // what command is being acknowledged
        MAV_RESULT_ACCEPTED, // is good?
        0, // progress unused
        0, // extra param unused
        255, // target system ID - GCS for me
        190 // target component ID - GCS for me
    );

    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    size_t message_length = mavlink_msg_to_send_buffer(buffer, &ack);

    this->udpClient.sendBinary(buffer, message_length);
}

bool Drone::isConnected() {
    return udpClient.isConnected();
}

bool Drone::isArmed() {
    return armed;
}

UDPClient* Drone::getClientPointer(){
    return &udpClient;
}