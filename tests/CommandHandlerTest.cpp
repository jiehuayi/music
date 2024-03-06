#include <gtest/gtest.h>

#include "CommandHandler.hpp"
#include "CoreCommands.hpp"
#include "Command.hpp"

class CommandHandlerTest : public ::testing::Test {
    protected:
        CommandHandler _handler;
};

#define CMD_DUMMY "dummy"
COMMAND_DEFINITION_BUILDER(Dummy);

void Dummy::execute(Parameters args) {
    TRACE_COMMAND();
}

TEST_F(CommandHandlerTest, RegisterCommandTest) {
    PlaylistManager pm = PlaylistManager();
    Window wm = Window(pm);

    PlaylistManager& library = pm;
    Window& window = wm;
    _handler.registerCommand(CMD_DUMMY, COMMAND_DEFINE(Dummy));
    ASSERT_EQ(1,1);
}
