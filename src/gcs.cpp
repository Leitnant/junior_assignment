#include <mavlink.h>
#include <chrono>

#include "gcs.h"

using namespace std;

GCS::GCS(int port) : udpServer(port){
    lastHeartbeatTime = chrono::steady_clock::now();
    udpServer.setOwner(this);
}

void GCS::send_arm(){
    mavlink_message_t msg;

    mavlink_msg_command_long_pack(
        255,    //GCS ID
        190,    //GCS component ID
        &msg,   //struct to pack
        1,      //target ID
        1,      //target component ID
        MAV_CMD_DO_SET_MODE,
        0,      //confirm?
        MAV_MODE_FLAG_CUSTOM_MODE_ENABLED,
        MAV_MODE_GUIDED_ARMED,
        0, 0, 0, 0, 0
    );

    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    size_t message_length = mavlink_msg_to_send_buffer(buffer, &msg);
    
    this->udpServer.sendBinary(buffer, message_length);
}

void GCS::send_disarm(){
    mavlink_message_t msg;

    mavlink_msg_command_long_pack(
        255,    //GCS ID
        190,    //GCS component ID
        &msg,   //struct to pack
        1,      //target ID
        1,      //target component ID
        MAV_CMD_DO_SET_MODE,
        0,      //confirm?
        MAV_MODE_FLAG_CUSTOM_MODE_ENABLED,
        MAV_MODE_GUIDED_DISARMED,
        0, 0, 0, 0, 0
    );
    
    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    size_t message_length = mavlink_msg_to_send_buffer(buffer, &msg);
    
    this->udpServer.sendBinary(buffer, message_length);
}

void GCS::send_goto(float x, float y, float alt){
    mavlink_message_t msg;

    mavlink_msg_command_long_pack(
        255,
        190,
        &msg,
        1,
        1,
        MAV_CMD_OVERRIDE_GOTO, // command to set goto
        0,
        MAV_GOTO_DO_CONTINUE, // command to resume operation immediately
        MAV_GOTO_HOLD_AT_SPECIFIED_POSITION, // command to go to the new position
        MAV_FRAME_LOCAL_NED, // set frame as X+ = north, Y+ = east, Z+ = down
        0, 
        x, 
        y, 
        -alt // flip the Z value to point up
    );
    
    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    size_t message_length = mavlink_msg_to_send_buffer(buffer, &msg);
    
    this->udpServer.sendBinary(buffer, message_length);
}

void GCS::updateDronePos(float x, float y, float alt){
    droneState.x = x;
    droneState.y = y;
    droneState.alt = alt;
    printf("X: %f Y: %f ALT: %f\n", droneState.x, droneState.y, droneState.alt);
}

void GCS::updateHeartbeat(int mode){
    droneState.mode = mode;
    lastHeartbeatTime = chrono::steady_clock::now();
}

void GCS::displayStatus(){
    chrono::steady_clock::time_point now = chrono::steady_clock::now();
    chrono::milliseconds elapsed = chrono::duration_cast<chrono::milliseconds>(now - lastHeartbeatTime);
    printf("[GCS]: Drone status\n");
    printf("[ X ]: %f\n", droneState.x);
    printf("[ Y ]: %f\n", droneState.y);
    printf("[Alt]: %f\n", droneState.alt);
    printf("[HBT]: %d ms since HBT\n", (int)elapsed.count());        
}

bool GCS::isConnected() {
        return this->udpServer.isConnected();
    }
UDPServer* GCS::getServerPointer(){
    return &udpServer;
}