#include "drone.h"

using namespace std;
using time_source = chrono::system_clock;
using time_point = time_source::time_point;
using duration = time_source::duration;

float speed = 0.5;
float geofence = 100;
int clientPort = 4498;
duration messageInterval = 100ms;

int main() {
    printf("[DRONE]: Initialising drone...\n");
    Drone drone(speed, geofence, clientPort);

    time_point prevTime = time_source::now();
    while (1) {
        time_point currentTime = time_source::now();
        if (currentTime - prevTime >= messageInterval){
            if (drone.getArmedState()) {
                drone.updatePosition();
            }
            drone.sendHeartbeat();
            drone.sendPosition();
            prevTime = currentTime;
        }
    if (drone.pollIncoming(15)) {
        drone.handleIncoming();
    }
    this_thread::sleep_for(5ms);
    }
    return 0;
}