#include "gcs.h"
#include <iostream>
#include <string>
#include <mavlink.h>

using namespace std;

    GCS::GCS(int port) : udpServer(port){
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
            216,    // GUIDED_ARMED
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
            88,    // GUIDED_ARMED
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
            MAV_FRAME_LOCAL_ENU, // set frame as X+ = east, Y+ = North, Z+ = Alt
            0, x, y, alt
        );
        
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        size_t message_length = mavlink_msg_to_send_buffer(buffer, &msg);
        
        this->udpServer.sendBinary(buffer, message_length);
    }

    void GCS::displayStatus(){
    }

    bool GCS::isConnected() {
        return this->udpServer.isConnected();
    }

