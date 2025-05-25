#include <gtest/gtest.h>
#include "greeting.h"

TEST(GreetingTest, ReturnsCorrectGreeting) {
    EXPECT_EQ(getGreeting(), "Hello, World!");
}
TEST(GreetingTest, ReturnsCorrectGreeting2) {
    EXPECT_EQ(getGreeting(), "Hea!");
}