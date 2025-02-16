CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17
TARGET = chunk
SRCS = ChunkType.cpp Chunk.cpp main.cpp PNG.cpp
OBJS = $(SRCS:.cpp=.o)

.PHONY: all build run clean

all: build run

build: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)