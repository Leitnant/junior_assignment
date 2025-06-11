#include <mavlink.h>
#include <chrono>

#include "gcs.h"

#define SYSID 255
#define COMPID 190

using namespace std;

GCS::GCS(int port) : udpServer(port){
    lastHeartbeatTime = chrono::steady_clock::now();
    udpServer.setOwner(this);
}

void GCS::send_arm(){
    mavlink_message_t msg;

    mavlink_msg_command_long_pack(
        SYSID,    //GCS ID
        COMPID,    //GCS component ID
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
        SYSID,    //GCS ID
        COMPID,    //GCS component ID
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
        SYSID,
        COMPID,
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
}

void GCS::updateHeartbeat(int mode){
    droneState.mode = mode;
    lastHeartbeatTime = chrono::steady_clock::now();
}

void GCS::startDisplayLoop(){
    dispRunning = true;
    dispThread = thread(&GCS::displayStatus, this);
}

void GCS::stopDisplayLoop(){
    dispRunning = false;
    if (dispThread.joinable()) {
        dispThread.join();
    }
}


void GCS::displayStatus(){
    while (dispRunning) {
        chrono::steady_clock::time_point now = chrono::steady_clock::now();
        chrono::milliseconds elapsed = chrono::duration_cast<chrono::milliseconds>(now - lastHeartbeatTime);
        
        string armedMode;
        if (droneState.mode == MAV_MODE_FLAG_GUIDED_ENABLED){
            armedMode = "GUIDED_DISARMED";
        } else if (droneState.mode == MAV_MODE_FLAG_GUIDED_ENABLED || MAV_MODE_FLAG_SAFETY_ARMED) {
            armedMode = "GUIDED_ARMED";
        } else {
            armedMode = "UNKNOWN";
        }
        
        if (isConnected()){
            printf("[GCS]: Drone connected\n");
            printf("[ARM]: %s\n", armedMode.c_str());
            printf("[ X ]: %f\n", droneState.x);
            printf("[ Y ]: %f\n", droneState.y);
            printf("[Alt]: %f\n", droneState.alt);
        } else {
            printf("[GCS]: Drone disconnected\n");
        }
        printf("[HBT]: %d ms since HBT\n", (int)elapsed.count());
        this_thread::sleep_for(chrono::milliseconds(1000));
    }
    printf("[GCS]: Information displaying stopped.\n");
}

bool GCS::isConnected() {
        return this->udpServer.isConnected();
    }
UDPServer* GCS::getServerPointer(){
    return &udpServer;
}