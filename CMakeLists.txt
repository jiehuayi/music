cmake_minimum_required(VERSION 3.14)
project(Penelope)

# Compiler options
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(
    libraries/bass/include
    /usr/local/Cellar/ncurses/6.4/include
)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Link directories
link_directories(
    /usr/local/Cellar/ncurses/6.4/lib
    libraries/bass/bin
)

set(SRCS
    Main.cpp
    Window.cpp
    Library.cpp
    Playlist.cpp
    Track.cpp
    Color.cpp
    CommandHandler.cpp
    Command.cpp
    CommandParser.cpp
    CoreCommands.cpp
    Component.cpp
    ConsoleComponent.cpp
    ListComponent.cpp
    VisualComponent.cpp
    PopupComponent.cpp
)
add_executable(main.out ${SRCS})

target_precompile_headers(main.out PRIVATE Log.hpp)

target_link_libraries(main.out
    ncurses
    bass
)

set(TESTS
    tests/SanityTest.cpp
    tests/CommandParserTest.cpp
)

set(TEST_CLASSES
    CommandParser.cpp
)

enable_testing()

add_executable(tests.out ${TESTS} ${TEST_CLASSES})

target_link_libraries(
    tests.out
    GTest::gtest_main
)

target_include_directories(tests.out PRIVATE ./)

include(GoogleTest)
gtest_discover_tests(tests.out)

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
