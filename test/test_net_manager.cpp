#include <gtest/gtest.h>
#include <mavlink.h>
#include <lnl/lnl.h>
#include <cstdio>
#include <cstring>
#include <vector>
#include <memory>
#include <thread>

#include "udp_server.h"
#include "udp_client.h"
#include "gcs.h"
#include "drone.h"

#define GCS_SYSID 255
#define GCS_COMPID 190

#define DRONE_SYSID 1
#define DRONE_COMPID 1

using namespace std;

// Test Classes due to me being a dumbass and designing myself into a corner

class TestGCS : public GCS {
    public:
        using GCS::GCS;
        using GCS::getServerPointer;

        bool heartbeatUpdated = false;
        int heartbeatState = 0;

        bool positionUpdated = false;
        float x = 0, y = 0, alt = 0;
    
        void updateHeartbeat(int mode) override {
            heartbeatUpdated = true;
            heartbeatState = mode;
        }
        void updateDronePos(float newX, float newY, float newAlt) override {
            positionUpdated = true;
            x = newX;
            y = newY;
            alt = newAlt;
        }
};

class TestDrone : public Drone {
    public:
        using Drone::Drone;
        using Drone::getClientPointer;
        bool armed = false;

        bool targetSet = false;
        float targetX = 0;
        float targetY = 0;
        float targetAlt = 0;

        bool ackSent = false;
        int ackCommand = 0;

        

        void setArmedState(bool state) override {
            armed = state;
        }

        void setTargetPos(float x, float y, float alt) override {
            targetSet = true;
            targetX = x;
            targetY = y;
            targetAlt = alt;
        }

        void send_ack(int command) override {
            ackSent = true;
            ackCommand = command; // this is MAV_STATE on arm commands, 
        }

};

// ----- UDPServer Unit Tests ----- //

TEST(UDPServerTest, HandlesHeartbeatMessage) {
    TestGCS testGCS(2000);
    
    UDPServer* server = testGCS.getServerPointer();
    
    mavlink_message_t msg;
    mavlink_msg_heartbeat_pack(
        DRONE_SYSID, 
        DRONE_COMPID, 
        &msg, 
        MAV_TYPE_GCS, 
        MAV_AUTOPILOT_INVALID,
        0, 
        0, 
        MAV_STATE_ACTIVE
    );
    
    server->handleReceivedData(msg);
    
    EXPECT_TRUE(testGCS.heartbeatUpdated);
    EXPECT_EQ(testGCS.heartbeatState, MAV_STATE_ACTIVE);
}

TEST(UDPServerTest, HandlesLocalPositionMessage) {
    TestGCS testGCS(2000);
    
    UDPServer* server = testGCS.getServerPointer();

    mavlink_message_t position;
    float x = 1, y = 2, alt = 3;
    mavlink_msg_local_position_ned_pack(
        DRONE_SYSID, //system ID
        DRONE_COMPID, //component ID
        &position,
        0, //ms since system boot, won't be sending
        x,
        y,
        -alt, //to convert altitude to NED format
        0, 0, 0 // velocities, not simulating atm.
    );
    server->handleReceivedData(position);
    
    EXPECT_TRUE(testGCS.positionUpdated);
    EXPECT_FLOAT_EQ(testGCS.x, x);
    EXPECT_FLOAT_EQ(testGCS.y, y);
    EXPECT_FLOAT_EQ(testGCS.alt, alt);
}

// ----- UDPClient Unit Tests ----- //

TEST(UDPClientTest, HandlesArmCommand) {
    lnl::net_address serverAddr("127.0.0.1", 5000);
    TestDrone testDrone(0.5, 100, serverAddr);
    
    UDPClient* client = testDrone.getClientPointer();

    mavlink_message_t msg;
    mavlink_msg_command_long_pack(
        GCS_SYSID,
        GCS_COMPID,
        &msg,
        1,
        1,
        MAV_CMD_DO_SET_MODE,
        0,
        MAV_MODE_FLAG_CUSTOM_MODE_ENABLED,
        216, // armed
        0,0,0,0,0
    );
    
    client->handleReceivedData(msg);
    
    EXPECT_TRUE(testDrone.armed);
    EXPECT_TRUE(testDrone.ackSent);
    EXPECT_EQ(testDrone.ackCommand, 216);
}

TEST(UDPClientTest, HandlesDisarmCommand) {
    lnl::net_address serverAddr("127.0.0.1", 5000);
    TestDrone testDrone(0.5, 100, serverAddr);
    testDrone.armed = true;
    
    UDPClient* client = testDrone.getClientPointer();

    mavlink_message_t msg;
    mavlink_msg_command_long_pack(
        GCS_SYSID,
        GCS_COMPID,
        &msg,
        1,
        1,
        MAV_CMD_DO_SET_MODE,
        0,
        MAV_MODE_FLAG_CUSTOM_MODE_ENABLED,
        88, // disarmed
        0,0,0,0,0
    );
    
    client->handleReceivedData(msg);
    
    EXPECT_FALSE(testDrone.armed);
    EXPECT_TRUE(testDrone.ackSent);

}

TEST(UDPClientTest, HandlesGotoCommand) {
    lnl::net_address serverAddr("127.0.0.1", 5000);
    TestDrone testDrone(0.5, 100, serverAddr);
    
    UDPClient* client = testDrone.getClientPointer();

    mavlink_message_t msg;
    float x = 100, y = 200, alt = 300;
    mavlink_msg_command_long_pack(
        GCS_SYSID,
        GCS_COMPID,
        &msg,
        1,
        1,
        MAV_CMD_OVERRIDE_GOTO, //command to set goto
        0,
        MAV_GOTO_DO_CONTINUE, //command to resume operation immediately
        MAV_GOTO_HOLD_AT_SPECIFIED_POSITION, //command to go to the new position
        MAV_FRAME_LOCAL_NED, //set frame as X+ = north, Y+ = east, Z+ = down
        0, 
        100, 
        200, 
        -alt // Flipping to convert to NED
    );
    
    client->handleReceivedData(msg);
    
    EXPECT_TRUE(testDrone.targetSet);
    EXPECT_FLOAT_EQ(testDrone.targetX, x);
    EXPECT_FLOAT_EQ(testDrone.targetY, y);
    EXPECT_FLOAT_EQ(testDrone.targetAlt, alt);
}