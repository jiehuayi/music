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
COMMAND_DEFINITION_BUILDER(Dummy2);

void Dummy::execute(Parameters args) {
    TRACE_COMMAND();
    _lib.newPlaylist("/");
}

void Dummy2::execute(Parameters args) { TRACE_COMMAND(); }

TEST_F(CommandHandlerTest, RegisterCommandTest) {
    PlaylistManager pm = PlaylistManager();
    Window wm = Window(pm);

    PlaylistManager& library = pm;
    Window& window = wm;
    
    // Test: registering a new command
    int beforeCount = pm.playlistCount();
    _handler.registerCommand(CMD_DUMMY, COMMAND_DEFINE(Dummy));
    _handler.processCommand(CMD_DUMMY);

    int newCount = pm.playlistCount();
    ASSERT_NE(beforeCount, newCount);
    
    // Test: registering a command to an existing bind
    _handler.registerCommand(CMD_DUMMY, COMMAND_DEFINE(Dummy2));
    _handler.processCommand(CMD_DUMMY);

    ASSERT_EQ(newCount, pm.playlistCount());
}
