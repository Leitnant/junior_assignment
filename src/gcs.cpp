#include "gcs.h"
#include <iostream>

using namespace std;

GCS::GCS(int port) {
    udpServer.init(port);
}

void GCS::update() {
    string incoming = udpServer.receive();
    if (!incoming.empty()) {
        message = incoming;
    }
}

void GCS::displayStatus() {
    cout << "GCS: Current Drone Telemetry: " << incoming << "\n";
}
