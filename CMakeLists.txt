cmake_minimum_required(VERSION 3.10)
project(MyProject)

# Compiler options
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2")

# Include directories
include_directories(
    libraries/bass/include
    /usr/local/Cellar/ncurses/6.4/include
)

# Link directories
link_directories(
    /usr/local/Cellar/ncurses/6.4/lib
    libraries/bass/bin
)

# Source files
set(SRCS
    Main.cpp
    Window.cpp
    Color.cpp
    Track.cpp
    Playlist.cpp
    Library.cpp
    Log.cpp
    CoreCommands.cpp
    CommandHandler.cpp
    Command.cpp
    Component.cpp
    ConsoleComponent.cpp
    ListComponent.cpp
    VisualComponent.cpp
    PopupComponent.cpp
)

# Executable
add_executable(main.out ${SRCS})

# Link libraries
target_link_libraries(main.out
    ncurses
    bass
)

# Install target (optional)
install(TARGETS main.out DESTINATION bin)

# Run target (optional)
add_custom_target(run
    COMMAND main.out
    DEPENDS main.out
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)

# Clean target (optional)
add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/*.o ${CMAKE_BINARY_DIR}/main.out
)
