#include <gtest/gtest.h>
#include <string>
#include "FixedHashMap.hpp"
#include <vector>
#include <iostream>

class FixedHashMapTest : public ::testing::Test {
protected:
    std::vector<std::string*> s;

    virtual void SetUp() {
        for (int i = 0; i < 20; ++i) {
            s.push_back(new std::string(std::to_string(i)));
        }
    }
    virtual void TearDown() {
        for (int i = 0; i < 20; ++i)
            delete s[i];
    }
};

std::vector<std::string*> s;


TEST_F(FixedHashMapTest, construct) {
    FixedHashMap<int> empty{10};
    EXPECT_EQ(10, empty.maxSize());
    EXPECT_EQ(0, empty.size());
    EXPECT_FLOAT_EQ(0, empty.load());
}

TEST_F(FixedHashMapTest, fixedSize) {
    FixedHashMap<std::string> empty{5};
    EXPECT_TRUE(empty.set("one", s[1]));
    EXPECT_TRUE(empty.set("two", s[2]));
    EXPECT_TRUE(empty.set("three", s[3]));
    EXPECT_FLOAT_EQ(0.6, empty.load());
    EXPECT_TRUE(empty.set("four", s[4]));
    EXPECT_TRUE(empty.set("five", s[5]));
    EXPECT_FLOAT_EQ(1, empty.load());
    EXPECT_EQ(5, empty.size());
    EXPECT_FALSE(empty.set("six", s[6]));
    EXPECT_FALSE(empty.set("seven", s[7]));
    EXPECT_EQ(5, empty.size());
}

TEST_F(FixedHashMapTest, canAddToNewList)
{
    FixedHashMap<std::string> empty{10};
    empty.set("one", s[1]);
    EXPECT_EQ(1, empty.size());
    EXPECT_EQ(s[1], empty.get("one"));
}

TEST_F(FixedHashMapTest, canGetandRemove)
{
    FixedHashMap<std::string> empty{10};
    empty.set("one", s[1]);
    empty.set("two", s[2]);
    empty.set("three", s[3]);
    empty.set("four", s[4]);
    empty.set("five", s[5]);
    empty.set("six", s[6]);
    empty.set("seven", s[7]);
    ASSERT_EQ(7, empty.size());
    ASSERT_EQ(nullptr, empty.get("DNE"));
    ASSERT_EQ(s[7], empty.get("seven"));
    ASSERT_TRUE(empty.contains("one"));
    ASSERT_TRUE(empty.contains("two"));
    ASSERT_TRUE(empty.contains("three"));
    ASSERT_TRUE(empty.contains("four"));
    ASSERT_TRUE(empty.contains("five"));
    ASSERT_TRUE(empty.contains("six"));
    ASSERT_TRUE(empty.contains("seven"));
    ASSERT_EQ(s[7], empty.remove("seven"));
    ASSERT_FALSE(empty.contains("seven"));
    ASSERT_EQ(6, empty.size());
}

TEST_F(FixedHashMapTest, doesntLoseNodesOnDelete)
{
    FixedHashMap<std::string> empty{10};
    empty.set("one", s[1]);
    empty.set("two", s[2]);
    empty.set("three", s[3]);
    empty.set("four", s[4]);
    empty.set("five", s[5]);
    empty.set("six", s[6]);
    empty.set("seven", s[7]);
    empty.set("eight", s[8]);
    empty.set("nine", s[9]);
    empty.set("ten", s[10]);
    ASSERT_EQ(10, empty.size());
    ASSERT_TRUE(empty.contains("one"));
    ASSERT_TRUE(empty.contains("two"));
    ASSERT_TRUE(empty.contains("three"));
    ASSERT_TRUE(empty.contains("four"));
    ASSERT_TRUE(empty.contains("five"));
    ASSERT_TRUE(empty.contains("six"));
    ASSERT_TRUE(empty.contains("seven"));
    empty.remove("two");
    EXPECT_TRUE(empty.contains("ten"));
    // empty.printMap();

}

TEST_F(FixedHashMapTest, canCopyConstruct)
{
    FixedHashMap<std::string> empty{10};
    empty.set("one", s[1]);
    empty.set("two", s[2]);
    empty.set("three", s[3]);
    empty.set("four", s[4]);
    empty.set("five", s[5]);
    empty.set("six", s[6]);
    empty.set("seven", s[7]);

    FixedHashMap<std::string> copyConstruct(empty);
    ASSERT_EQ(copyConstruct.size(), empty.size());
    ASSERT_FLOAT_EQ(copyConstruct.load(), empty.load());
    ASSERT_EQ(copyConstruct.maxSize(), empty.maxSize());

    empty.set("eight", s[8]);
    EXPECT_EQ(8, empty.size());
    EXPECT_EQ(7, copyConstruct.size());
    copyConstruct.set("nine", s[9]);
    copyConstruct.set("ten", s[10]);
    copyConstruct.set("eleven", s[11]);
    EXPECT_EQ(8, empty.size());
    EXPECT_EQ(10, copyConstruct.size());

    ASSERT_EQ(s[1], copyConstruct.get("one"));
    ASSERT_EQ(s[2], copyConstruct.get("two"));
    ASSERT_EQ(s[3], copyConstruct.get("three"));
    ASSERT_EQ(s[4], copyConstruct.get("four"));
    ASSERT_EQ(s[5], copyConstruct.get("five"));
    ASSERT_EQ(s[6], copyConstruct.get("six"));
    ASSERT_EQ(s[7], copyConstruct.get("seven"));

}

TEST_F(FixedHashMapTest, canCopyAssign)
{
    FixedHashMap<std::string> empty{10};
    empty.set("one", s[1]);
    empty.set("two", s[2]);
    empty.set("three", s[3]);
    empty.set("four", s[4]);
    empty.set("five", s[5]);
    empty.set("six", s[6]);
    empty.set("seven", s[7]);

    FixedHashMap<std::string> copyAssign{20};
    copyAssign.set("eight", s[8]);
    ASSERT_EQ(7, empty.size());
    ASSERT_EQ(1, copyAssign.size());

    copyAssign = empty;
    ASSERT_EQ(copyAssign.size(), empty.size());
    ASSERT_FLOAT_EQ(copyAssign.load(), empty.load());
    ASSERT_EQ(copyAssign.maxSize(), empty.maxSize());

    ASSERT_EQ(7, copyAssign.size());
    empty.set("twelve", s[12]);
    copyAssign.set("nine", s[9]);
    copyAssign.set("ten", s[10]);
    copyAssign.set("eleven", s[11]);
    ASSERT_EQ(8, empty.size());
    ASSERT_EQ(10, copyAssign.size());

    ASSERT_FALSE(copyAssign.contains("eight"));
    ASSERT_EQ(s[1], copyAssign.get("one"));
    ASSERT_EQ(s[2], copyAssign.get("two"));
    ASSERT_EQ(s[3], copyAssign.get("three"));
    ASSERT_EQ(s[4], copyAssign.get("four"));
    ASSERT_EQ(s[5], copyAssign.get("five"));
    ASSERT_EQ(s[6], copyAssign.get("six"));
    ASSERT_EQ(s[7], copyAssign.get("seven"));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
