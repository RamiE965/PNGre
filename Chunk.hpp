#pragma once
#include <iostream>
#include <cstdint>
#include <vector>
#include "ChunkType.hpp"

class Chunk {
private:
    ChunkType chunktype_m;
    std::vector<uint8_t> data_m;
    uint32_t length_m;
    uint32_t crc_m;
    static uint32_t crc_table[256];
    static bool crc_table_computed;

    static void make_crc_table();
    uint32_t calculate_crc();

public:
    uint32_t length() const;
    uint32_t crc() const;
    const ChunkType& chunktype() const;
    std::vector<uint8_t>& data() const;
    Chunk(ChunkType chunktype, std::vector<uint8_t> data);
};