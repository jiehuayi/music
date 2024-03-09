#include <gtest/gtest.h>

#include "Playlist.hpp"

class PlaylistTest : public ::testing::Test {
    protected:
        void SetUp() override {
            p3 = Playlist("/");
            p4 = Playlist("/");
        }

        Playlist p1, p2, p3, p4;
};


TEST_F(PlaylistTest, PlaylistOperatorTest_Equal) {
    ASSERT_TRUE(p1 == p2);
    ASSERT_TRUE(p3 == p4);
    ASSERT_FALSE(p1 == p3);
}

TEST_F(PlaylistTest, PlaylistOperatorTest_NotEqual) {
    ASSERT_TRUE(p1 != p3);
    ASSERT_FALSE(p1 != p2);
    ASSERT_FALSE(p3 != p4);
}

TEST(PlaylistBasicTest, PlaylistReadTest) {
    Playlist p("/dne/path/probably");
    Playlist ph("/etc/hosts");
    EXPECT_THROW(p.readPlaylist(), std::runtime_error); 
    EXPECT_THROW(ph.readPlaylist(), std::runtime_error);
}

TEST(PlaylistBasicTest, PlaylistGetPathTest) {
    Playlist p1;
    Playlist p2("/abc/def/geh");
    Playlist p3("/");
    Playlist p4("");

    ASSERT_EQ(p1.getPath(), std::string(getenv("HOME")) + "/Music/");
    ASSERT_EQ(p2.getPath(), "/abc/def/geh");
    ASSERT_EQ(p3.getPath(), "/");
    ASSERT_EQ(p4.getPath(), "");
}
