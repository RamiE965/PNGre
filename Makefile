CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -Isrc

# Main program
TARGET = chunk
SRCS = src/ChunkType.cpp src/Chunk.cpp src/PNG.cpp src/main.cpp
OBJS = $(SRCS:.cpp=.o)

# Test program
TEST_TARGET = run_tests
TEST_SRCS = src/ChunkType.cpp src/Chunk.cpp src/PNG.cpp tests/tests.cpp
TEST_OBJS = $(TEST_SRCS:.cpp=.o)

.PHONY: all build run clean test

all: build

build: $(TARGET) $(TEST_TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJS)
	$(CXX) $(TEST_OBJS) -o $(TEST_TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TEST_OBJS) $(TARGET) $(TEST_TARGET)