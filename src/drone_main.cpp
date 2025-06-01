#include "drone.h"
#include <lnl/lnl.h>
#include <thread>
#include <chrono>
#include <iostream>
#include <string>
using namespace std;

int main() {
    lnl::initialize();

    printf("[DRONE]: Initialising drone...\n");
    lnl::net_address address("127.0.0.1", 4499);
    Drone drone(0, 0, 0, address);

    while (1){
    }
    return 0;
}