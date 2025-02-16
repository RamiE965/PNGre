#pragma once
#include <vector>
#include <cstdint>
#include "Chunk.hpp"

class PNG {
private:
    std::vector<Chunk> chunks_m;
    // header (4) + data (4) + crc (4)
    const size_t REMAINDER = 12;
    
public:
    const std::vector<uint8_t> STANDARD_HEADER {137, 80, 78, 71, 13, 10, 26, 10};
    
    PNG(std::vector<uint8_t>);
    PNG(std::vector<Chunk>);
};