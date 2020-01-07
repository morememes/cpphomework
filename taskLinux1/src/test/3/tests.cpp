#include <gtest/gtest.h> // заголовочный файл Google Test
#include "myproject/3/message.h"
#include "myproject/3/cat.h"
#include "myproject/3/tie.h"

TEST(Message, Compare) {
    std::stringstream s;
    message(s, "% + % = 3", 1, 2.0f);
    EXPECT_EQ("1 + 2 = 3", s.str());
    std::stringstream s1;
    message(s1, "Hello %!", "World");
    EXPECT_EQ("Hello World!", s1.str());
}

TEST(Message, Overfill) {
    std::stringstream s;
    message(s, "% + % = %", 1, 'a', 4.2f, 'b');
    EXPECT_EQ("1 + a = 4.2", s.str());
}

TEST(Message, failure) {
    std::stringstream s;
    message(s, "% + % = %", 1, 'a');
    EXPECT_EQ("1 + a = a", s.str());
}


TEST(Cat, Compare) {
    std::array<float, 2> a = {2.0f, 3.0f};
    std::array<float, 2> b = {1.0f, 3.0f};
    std::array<float, 2> c = {2.0f, 3.0f};
    std::array<float, 2> d = {2.0f, 1.0f};
    std::array<float, 8> res = {2.0f, 3.0f, 1.0f, 3.0f, 2.0f, 3.0f, 2.0f, 1.0f};
    std::array<float, 6> res2 = {2.0f, 3.0f, 2.0f, 3.0f, 2.0f, 1.0f};
    EXPECT_EQ(res, cat(a, b, c, d));
    EXPECT_EQ(res2, cat(a, c, d));
}

TEST(Tie, Compare) {
    std::array<float, 2> a = {2.0f, 3.0f};
    std::array<float, 2> b = {1.0f, 3.0f};
    std::array<float, 2> c = {2.0f, 3.0f};
    std::array<float, 2> r1 = {5.0f, 6.0f};
    std::array<float, 2> r2 = {1.0f, 0.0f};
    std::array<float, 2> r3 = {7.0f, 7.0f};
    std::array<float, 6> input = {5.0f, 6.0f, 1.0f, 0.0f, 7.0f, 7.0f};
    MyTieF(a, b, c) = input;
    EXPECT_EQ(r1, a);
    EXPECT_EQ(r2, b);
    EXPECT_EQ(r3, c);
}
