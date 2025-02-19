#pragma once
#include <iostream>
#include "../src/Chunk.hpp"
#include "../src/ChunkType.hpp"
#include "../src/PNG.hpp"
#include <cassert>
#include <sstream>
#include <optional>
#include <vector>

// test macro
#define RUN_TEST(test) \
    std::cout << "Running " << #test << "... "; \
    test(); \
    std::cout << "OK" << std::endl;
