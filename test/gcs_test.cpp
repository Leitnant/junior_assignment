#include <gtest/gtest.h>
#include <mavlink.h>

#include "drone.h"
#include "gcs.h"

using namespace std;

class GCSFixture : public::testing::Test {
    protected:
        int gcsPort = 4499;
        int dronePort = 4498;

        GCSFixture():
        gcs(gcsPort),
        drone(0.5, 100, dronePort){}

        GCS gcs;
        Drone drone;
};

TEST_F(GCSFixture, HandlesHeartbeat) {
    gcs.setDroneState(0, 0, 0, 0);
    drone.setArmedState(false);

    drone.sendHeartbeat();
    gcs.handleIncoming();

    int disarmedMode = gcs.getDroneState().mode;
    
    gcs.setDroneState(0, 0, 0, 0);
    drone.setArmedState(true);
    
    drone.sendHeartbeat();
    gcs.handleIncoming();

    int armedMode = gcs.getDroneState().mode;

    EXPECT_EQ(disarmedMode, 8);
    EXPECT_EQ(armedMode, 136);
}

TEST_F(GCSFixture, HandlesPosition) {
    gcs.setDroneState(0, 0, 0, 0);
    drone.setCurrentPosition(10, 10, 10);

    drone.sendPosition();
    gcs.handleIncoming();

    auto droneState = gcs.getDroneState();

    EXPECT_EQ(droneState.x, 10);
    EXPECT_EQ(droneState.y, 10);
    EXPECT_EQ(droneState.alt, 10);
}