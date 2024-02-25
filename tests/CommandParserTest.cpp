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

TEST_F(CommandParserTest, CommandSplitTest_Single) { STRNMK(in, "penelope");
    VSTR(rv) = _parser.split(in, DELM);
    VSTR(ex) = { STRMK("penelope") };

    ASSERT_EQ(rv.size(), ex.size());
    
    for (unsigned int i = 0; i < rv.size(); ++i) {
        EXPECT_EQ(ex[i], rv[i]);
    }
}

TEST_F(CommandParserTest, CommandSplitTest_List) {
    STRNMK(in, "penelope, hello,world, x,var dev");
    VSTR(rv) = _parser.split(in, DELM);
    VSTR(ex) = { STRMK("penelope"), STRMK(" hello"), STRMK("world"),
        STRMK(" x"), STRMK("var dev") };

    ASSERT_EQ(rv.size(), ex.size());
    
    for (unsigned int i = 0; i < rv.size(); ++i) {
        EXPECT_EQ(ex[i], rv[i]);
    }
}

TEST_F(CommandParserTest, CommandSplitTest_Edge) {
    STRNMK(in, "penelope,, , ,,, , ,z,");
    VSTR(rv) = _parser.split(in, DELM);
    VSTR(ex) = { STRMK("penelope"), STRMK(""), STRMK(" "), STRMK(" "), STRMK(""), 
        STRMK(""), STRMK(" "), STRMK(" "), STRMK("z") };

    ASSERT_EQ(rv.size(), ex.size());
    
    for (unsigned int i = 0; i < rv.size(); ++i) {
        EXPECT_EQ(ex[i], rv[i]);
    }
}TEST_F(CommandParserTest, CommandSplitTest_MultipleDelimiters) {
    STRNMK(in, "apple,orange,,banana,,grape");
    VSTR(rv) = _parser.split(in, DELM);
    VSTR(ex) = { STRMK("apple"), STRMK("orange"), STRMK(""), 
        STRMK("banana"), STRMK(""), STRMK("grape") };

    ASSERT_EQ(rv.size(), ex.size());

    for (unsigned int i = 0; i < rv.size(); ++i) {
        EXPECT_EQ(ex[i], rv[i]);
    }
}

TEST_F(CommandParserTest, CommandSplitTest_TrailingDelimiter) {
    STRNMK(in, "one,two,three,");
    VSTR(rv) = _parser.split(in, DELM);
    VSTR(ex) = { STRMK("one"), STRMK("two"), STRMK("three") };

    ASSERT_EQ(rv.size(), ex.size());

    for (unsigned int i = 0; i < rv.size(); ++i) {
        EXPECT_EQ(ex[i], rv[i]);
    }
}

TEST_F(CommandParserTest, CommandSplitTest_LeadingDelimiter) {
    STRNMK(in, ",one,two,three");
    VSTR(rv) = _parser.split(in, DELM);
    VSTR(ex) = { STRMK(""), STRMK("one"), STRMK("two"), STRMK("three") };

    ASSERT_EQ(rv.size(), ex.size());

    for (unsigned int i = 0; i < rv.size(); ++i) {
        EXPECT_EQ(ex[i], rv[i]);
    }
}

TEST_F(CommandParserTest, CommandSplitTest_MixedDelimiters) {
    STRNMK(in, ",apple,orange,,banana,,grape,");
    VSTR(rv) = _parser.split(in, DELM);
    VSTR(ex) = { STRMK(""), STRMK("apple"), STRMK("orange"), STRMK(""), 
        STRMK("banana"), STRMK(""), STRMK("grape") };

    ASSERT_EQ(rv.size(), ex.size());

    for (unsigned int i = 0; i < rv.size(); ++i) {
        EXPECT_EQ(ex[i], rv[i]);
    }
}

TEST_F(CommandParserTest, CommandSplitTest_WhiteSpace) {
    STRNMK(in, "   apple,  orange  , banana  ,  grape   ");
    VSTR(rv) = _parser.split(in, DELM);
    VSTR(ex) = { STRMK("   apple"), STRMK("  orange  "), 
        STRMK(" banana  "), STRMK("  grape   ") };

    ASSERT_EQ(rv.size(), ex.size());

    for (unsigned int i = 0; i < rv.size(); ++i) {
        EXPECT_EQ(ex[i], rv[i]);
    }
}
