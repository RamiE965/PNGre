all: build run

build:
	g++ chunk.cpp -o chunk -std=c++17

run:
	./chunk