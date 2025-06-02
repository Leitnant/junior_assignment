#include "drone.h"
#include <lnl/lnl.h>
using namespace std;

int main() {
    lnl::initialize();

    printf("[DRONE]: Initialising drone...\n");
    lnl::net_address address("127.0.0.1", 4499);
    Drone drone(0.5, 10, address);

    while (1) {
        if (drone.isConnected()) {
            if (drone.isArmed()) {
                drone.updatePosition();
            }
            drone.send_heartbeat();
            drone.send_position();
            this_thread::sleep_for(chrono::milliseconds(1000));
        }
    }

    return 0;
}