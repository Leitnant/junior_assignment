#include "drone.h"
#include <lnl/lnl.h>
using namespace std;

int main() {
    lnl::initialize();

    printf("[DRONE]: Initialising drone...\n");
    lnl::net_address address("127.0.0.1", 4499);
    Drone drone(0.5, 10, address);

    chrono::steady_clock::time_point prevTime = chrono::steady_clock::now();

    while (1) {
        if (drone.isConnected()) {
            if (drone.isArmed()) {
                drone.updatePosition();
            }
            chrono::steady_clock::time_point currentTime = chrono::steady_clock::now();

            if (currentTime - prevTime >= chrono::milliseconds(100)){
                drone.send_heartbeat();
                drone.send_position();
                prevTime = currentTime;
            }
        }
        this_thread::sleep_for(chrono::milliseconds(5));
    }

    return 0;
}