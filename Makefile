all: build run

build:
	g++ ChunkType.cpp Chunk.cpp -o chunk -std=c++17

run:
	./chunk