#include "drone.h"

#include <thread>
#include <chrono>

using namespace std;
Drone::Drone(float x, float y, float alt, lnl::net_address serverAdress) :  x(x), y(y), alt(alt), udpClient(serverAdress){
    printf("[DRONE]: Drone setup complete!\n");
}

bool Drone::isConnected() {
    return Drone::connected;
}