CXX := g++
CXXFLAGS := -std=c++17 -w -O2 -I./libraries/bass/include
LDFLAGS := -L./libraries/bass/bin -lncurses -lbass

# List of source files
SRCS := Main.cpp Window.cpp Track.cpp Playlist.cpp

# List of object files
OBJS := $(SRCS:.cpp=.o)

# Name of the executable
TARGET := main.out

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET) *~
