#include "drone.h"

#define SYSID 1
#define COMPID 1
#define GCS_SYSID 255
#define GCS_COMPID 190

using namespace std;

Drone::Drone(float speed, float geofence, int clientPort):
speed(speed),
geofence(geofence),
client(clientPort, "127.0.0.1", 4499) {
    printf("[DRONE]: Setup complete.\n");
}


void Drone::updatePosition() {
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

void Drone::setCurrentPosition(float x, float y, float alt) {
    currentPos.x = x;
    currentPos.y = y;
    currentPos.alt = alt;
}

void Drone::setArmedState(bool state) {
    armed = state;
}

bool Drone::getArmedState() {
    return armed;
}

void Drone::setTargetPos(float x, float y, float alt) {
    targetPos.x = x;
    targetPos.y = y;
    targetPos.alt = alt;
}

Position Drone::getTargetPos() {
    return targetPos;
}

void Drone::sendHeartbeat() {
    client.sendHeartbeat(armed);
    printf("[DRONE]: Sent heartbeat packet.\n");
}

void Drone::sendPosition() {
    client.sendPosition(currentPos.x, currentPos.y, currentPos.alt);
    printf("[DRONE]: Sent position packet.\n");
    printf("[DRONE]: X: %f Y: %f ALT: %f\n", currentPos.x, currentPos.y, currentPos.alt);
}

bool Drone::pollIncoming(int timeout_ms) {
    return client.pollSocket(timeout_ms);
}

void Drone::handleIncoming() {
    mavlink_command_long_t command = client.receiveCommand();
    switch (command.command) {
        case MAV_CMD_DO_SET_MODE:
            switch ((int)command.param2) {
                client.sendAck(command, GCS_SYSID, GCS_COMPID);
                printf("[DRONE]: Sent ACK\n");
                case MAV_MODE_GUIDED_ARMED:
                    printf("[DRONE]: ARM\n");
                    setArmedState(true);
                    break;
                
                case MAV_MODE_GUIDED_DISARMED:
                    printf("[DRONE]: DISARM\n");
                    setArmedState(false);
                    break;

                default:
                    printf("[DRONE]: Received unknown armed state.\n");
                    break;
                }
                break;
        case MAV_CMD_OVERRIDE_GOTO: 
            client.sendAck(command, GCS_SYSID, GCS_COMPID);
            printf("[DRONE]: Sent ACK\n");
            {
                float x = command.param5;
                float y = command.param6;
                float alt = -command.param7;
                printf("[DRONE]: GOTO - X: %f Y: %f ALT: %f\n", x, y, alt);
                setTargetPos(x, y, alt);
                break;
            }
        case 1:
            printf("[DRONE]: Not connected to server!\n");
            break;
        case 0:
            printf("[DRONE]: Failed to read complete packet!\n");
            break;
        default:
            printf("[DRONE]: Received unknown command!\n");
            break;
        }
}