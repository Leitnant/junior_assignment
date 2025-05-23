#include <gtest/gtest.h>
#include "greeting.h"

TEST(HelloTest, ReturnsCorrectGreeting) {
    EXPECT_EQ(getGreeting(), "Hello, World!");
}
