#include <gtest/gtest.h>
#include <string>

std::string getGreeting(){
    return "Hello, World!";
}

TEST(GreetingTest, ReturnsCorrectGreeting) {
    EXPECT_EQ(getGreeting(), "Hello, World!");
}
TEST(GreetingTest, ReturnsCorrectGreeting2) {
    EXPECT_EQ(getGreeting(), "Hea!");
}