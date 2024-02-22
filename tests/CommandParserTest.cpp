#include <gtest/gtest.h>
#include <string>
#include <vector>

#include "CommandParser.hpp"

#define DELM ','

#define VSTR(name) std::vector<std::string> name
#define STR(name) std::string name
#define STRMK(val) std::string(val)
#define STRNMK(name, val) std::string name(val)

class CommandParserTest : public ::testing::Test {
    protected:
        CommandParser _parser;
};

TEST_F(CommandParserTest, CommandSplitTest_Empty) {
    STRNMK(in, "");
    VSTR(rv) = _parser.split(in, DELM);
    EXPECT_EQ(0, rv.size()); 
}

TEST_F(CommandParserTest, CommandSplitTest_Single) {
    STRNMK(in, "penelope");
    VSTR(rv) = _parser.split(in, DELM);
    VSTR(ex) = { STRMK("penelope") };

    ASSERT_EQ(rv.size(), ex.size());
    
    for (int i = 0; i < rv.size(); ++i) {
        EXPECT_EQ(ex[i], rv[i]);
    }
}
