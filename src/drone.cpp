#include "drone.h"

#include <lnl/net_address.h>
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

Drone::Drone(float x, float y, float alt, lnl::net_address serverAdress) :    x(x), y(y), alt(alt),
                                                                                mode(MAV_MODE::GUIDED_DISARMED),
                                                                                udpClient(serverAdress) {
    printf("[DRONE]: Configuration complete!\n");
}
void Drone::update() {}

void Drone::sendTelemetry() {}

void Drone::processCommands() {}

bool Drone::isConnected() {
    return Drone::connected;
}