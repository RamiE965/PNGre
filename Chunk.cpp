#include "ChunkType.hpp"
#include "Chunk.hpp"

uint32_t Chunk::crc_table[256];
bool Chunk::crc_table_computed = false;

void Chunk::make_crc_table() {
    for (uint32_t n = 0; n < 256; n++) {
        uint32_t c = n;
        for (int k = 0; k < 8; k++) {
            if (c & 1)
                c = 0xedb88320L ^ (c >> 1);
            else
                c = c >> 1;
        }
        crc_table[n] = c;
    }
    crc_table_computed = true;
}

uint32_t Chunk::calculate_crc() {
    if (!crc_table_computed) {
        make_crc_table();
    }
    uint32_t c = 0xffffffffL;
    auto bytes = chunktype_m.bytes();
    for (int n = 0; n < 4; n++) {
        c = crc_table[(c ^ bytes[n]) & 0xff] ^ (c >> 8);
    }
    for (uint8_t byte : data_m) {
        c = crc_table[(c ^ byte) & 0xff] ^ (c >> 8);
    }
    return c ^ 0xffffffffL;
}

uint32_t Chunk::length() const {
    return length_m;
}

uint32_t Chunk::crc() const {
    return crc_m;
}

const std::vector<uint8_t>& data Chunk::data() const {
    return data_m;
}

const ChunkType& Chunk::chunktype() const {
    return chunktype_m;
}

std::string Chunk::data_as_string() const {
    return std::string(data_m.begin(), data_m.end());
}

std::vector<uint8_t> as_bytes() const {

}

Chunk::Chunk(ChunkType chunktype, std::vector<uint8_t> data)
    : chunktype_m(chunktype)
    , data_m(data)
    , length_m(data.size())
{
    crc_m = calculate_crc();
}