CXX := g++
CXXFLAGS := -std=c++17 -w -O2 -I./libraries/bass/include -I/usr/local/Cellar/ncurses/6.4/include/
LDFLAGS := -D_XOPEN_SOURCE_EXTENDED -L/usr/local/Cellar/ncurses/6.4/lib -lncurses -lbass -L./libraries/bass/bin/

# List of source files
SRCS := Main.cpp Window.cpp Track.cpp Playlist.cpp Library.cpp Log.cpp

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

run: $(TARGET)
	export DYLD_LIBRARY_PATH=./libraries/bass/bin:$DYLD_LIBRARY_PATH	
	./main.out


clean:
	rm -f $(OBJS) $(TARGET) *~
