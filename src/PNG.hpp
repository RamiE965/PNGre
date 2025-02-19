#pragma once
#include <vector>
#include <cstdint>
#include <optional>
#include "Chunk.hpp"

class PNG {
private:
    std::vector<Chunk> chunks_m;
    // header (4) + data (4) + crc (4)
    const size_t REMAINDER = 12;
    
public:
    static const std::vector<uint8_t> STANDARD_HEADER;
    
    PNG(std::vector<uint8_t>);
    PNG(std::vector<Chunk>);

    const std::vector<Chunk>& chunks() const;
    const std::vector<uint8_t>& header() const;
    void append_chunk(Chunk);
    Chunk remove_first_chunk(ChunkType);
    const std::vector<uint8_t> as_bytes() const;
    std::optional<Chunk> chunk_by_type(const ChunkType& type) const;

    friend std::ostream& operator<<(std::ostream&, const PNG&);
};