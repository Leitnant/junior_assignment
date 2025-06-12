#include <gtest/gtest.h>
#include <mavlink.h>

#include "drone.h"
#include "gcs.h"

using namespace std;

class DroneFixture : public::testing::Test {
    protected:
        int gcsPort = 4499;
        int dronePort = 4498;

        DroneFixture():
        gcs(gcsPort),
        drone(0.5, 100, dronePort){}

        GCS gcs;
        Drone drone;
};

TEST_F(DroneFixture, HandlesArmCommand) {
    drone.setArmedState(false);
    gcs.sendArm();
    drone.handleIncoming();

    EXPECT_TRUE(drone.getArmedState());
}

TEST_F(DroneFixture, HandlesDisarmCommand) {
    drone.setArmedState(true);
    gcs.sendDisarm();
    drone.handleIncoming();

    EXPECT_FALSE(drone.getArmedState());
}

TEST_F(DroneFixture, HandlesGoToCommand) {
    drone.setTargetPos(0, 0, 0);
    gcs.sendGoTo(100, 100, 100);
    drone.handleIncoming();

    auto targetPos = drone.getTargetPos();
    EXPECT_FLOAT_EQ(targetPos.x,   100);
    EXPECT_FLOAT_EQ(targetPos.y,   100);
    EXPECT_FLOAT_EQ(targetPos.alt, 100);
}