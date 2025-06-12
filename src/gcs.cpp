#include "gcs.h"

#define SYSID 255
#define COMPID 190

using namespace std;

GCS::GCS(int serverPort):server(serverPort, "127.0.0.1", 4498) {
    printf("[GCS]: Setup complete.\n");
}

GCS::~GCS() {
    if (dispRunning) {
        stopDisplayLoop();
    }
}

void GCS::setDroneState(float x, float y, float alt, int mode) {
    droneState.x = x;
    droneState.y = y;
    droneState.alt = alt;
    droneState.mode = mode;
}

DroneState GCS::getDroneState() {
    return droneState;
}

void GCS::updateDronePos(float x, float y, float alt) {
    droneState.x = x;
    droneState.y = y;
    droneState.alt = alt;
}

void GCS::updateHeartbeat(int mode) {
    droneState.mode = mode;
    droneState.lastHeartbeatTime = chrono::steady_clock::now();
}

void GCS::startDisplayLoop() {
    dispRunning = true;
    dispThread = thread(&GCS::displayStatus, this);
}

void GCS::stopDisplayLoop() {
    dispRunning = false;
    if (dispThread.joinable()) {
        dispThread.join();
    }
}

void GCS::displayStatus() {
    while (dispRunning) {
        chrono::steady_clock::time_point now = chrono::steady_clock::now();
        chrono::milliseconds elapsed = chrono::duration_cast<chrono::milliseconds>(now - droneState.lastHeartbeatTime);
        if (pollIncoming(50)){
            handleIncoming();
        }

        bool guided = (droneState.mode & MAV_MODE_FLAG_GUIDED_ENABLED) != 0;
        bool armed = (droneState.mode & MAV_MODE_FLAG_SAFETY_ARMED) != 0;
        string armedMode;

        if (guided && !armed){
            armedMode = "GUIDED_DISARMED";
        } else if (guided && armed) {
            armedMode = "GUIDED_ARMED";
        } else {
            armedMode = "UNKNOWN";
        }
        printf("[HBT]: ms since last - %d\n[ARM]: %s\n[ X ]: %f\n[ Y ]: %f\n[Alt]: %f\n", 
            (int)elapsed.count(),
            armedMode.c_str(),
            droneState.x,
            droneState.y,
            droneState.alt
        );
    }
    printf("[GCS]: Information displaying stopped.\n");
}

void GCS::sendArm() {
    server.sendArm();
}

void GCS::sendDisarm() {
    server.sendDisarm();
}

void GCS::sendGoTo(float x, float y, float alt) {
    server.sendGoTo(x, y, alt);
}

bool GCS::pollIncoming(int timeout_ms) {
    return server.pollSocket(timeout_ms);
}

void GCS::handleIncoming(){
    mavlink_message_t incoming = server.receiveMessage();
    switch (incoming.msgid) {
        case MAVLINK_MSG_ID_HEARTBEAT:
            mavlink_heartbeat_t heartbeat;
            heartbeat = server.decodeHeartbeat(incoming);
            updateHeartbeat(heartbeat.base_mode);
            break;

        case MAVLINK_MSG_ID_LOCAL_POSITION_NED:
            mavlink_local_position_ned_t dronePos;
            dronePos = server.decodePosition(incoming);
            updateDronePos(dronePos.x, dronePos.y, -dronePos.z);
            break;

        case MAVLINK_MSG_ID_COMMAND_ACK:
            mavlink_command_ack_t ack;
            ack = server.decodeAck(incoming);
            printf("\n[GCS]: Received ACK for: %d\n", ack.command);
            break;

        default:
            printf("[GCS]: Received unexpected message: %d \n", incoming.msgid);
            break;
    }
}