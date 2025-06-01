#include "gcs.h"
#include <lnl/lnl.h>
#include <thread>
#include <chrono>
#include <iostream>

using namespace std;

int main() {
    lnl::initialize();

    printf("[DRONE]: Starting GCS...\n");
    GCS gcs(4499);

    while (1) {
        gcs.update();
        gcs.displayStatus();
        this_thread::sleep_for(chrono::milliseconds(100));
    }

    return 0;
}