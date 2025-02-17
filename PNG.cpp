#include "PNG.hpp"

const std::vector<uint8_t> PNG::STANDARD_HEADER {137, 80, 78, 71, 13, 10, 26, 10};

// Creates a PNG object from a vector of bytes
PNG::PNG(std::vector<uint8_t> bytes)
{
    if (bytes.size() < 8) {
        throw std::invalid_argument("Not enough bytes for PNG header!");
    }

    for (int i = 0; i < 8; i++)
    {
        if (STANDARD_HEADER[i] != bytes[i])
        {
            throw std::invalid_argument("First 8 bytes need to match standard header!");
        }
    }

    std::vector<Chunk> chunks;

    // parse chunks 
    size_t i = 8; // skip header
    while (i < bytes.size())
    {
        uint32_t data_length = (bytes[i] << 24) | (bytes[i+1] << 16) | (bytes[i+2] << 8) | bytes[i+3];

        if (i + REMAINDER + data_length > bytes.size()) 
        {
            throw std::invalid_argument("Invalid Chunk!");
        }

        std::vector<uint8_t> chunk_bytes;

        // length + type + data + crc
        size_t total_chunk_size = 4 + 4 + data_length + 4;

        chunk_bytes.insert(chunk_bytes.end(), 
                  bytes.begin() + i, 
                  bytes.begin() + i + total_chunk_size);

        chunks.push_back(Chunk(chunk_bytes));

        i += REMAINDER + data_length; 
    }

    chunks_m = chunks;
}

PNG::PNG(std::vector<Chunk> chunks)
{
    if (chunks.empty()) {
        throw std::invalid_argument("PNG must contain at least one chunk");
    }

    chunks_m = chunks;
}

void PNG::append_chunk(Chunk chunk)
{
    chunks_m.push_back(chunk);
}

std::optional<Chunk> PNG::chunk_by_type(const ChunkType& type) const
{
    for (size_t i = 0; i < chunks_m.size(); i++) {
        if (chunks_m[i].chunktype() == type) {
            return chunks_m[i];
        }
    }

    // not found
    return std::nullopt;
}

Chunk PNG::remove_first_chunk(ChunkType type) 
{
    for (size_t i = 0; i < chunks_m.size(); i++) {
        if (chunks_m[i].chunktype() == type) {
            Chunk chunk_to_remove = chunks_m[i];
            chunks_m.erase(chunks_m.begin() + i);
            return chunk_to_remove;
        }
    }
    throw std::runtime_error("Chunk not found");
}

const std::vector<uint8_t>& PNG::header() const
{
    return STANDARD_HEADER;
}

const std::vector<Chunk>& PNG::chunks() const 
{
    return chunks_m;
}

const std::vector<uint8_t> PNG::as_bytes() const
{
    std::vector<uint8_t> bytes;
    size_t total_size = STANDARD_HEADER.size();

    for (const auto& chunk : chunks_m) {
        total_size += chunk.as_bytes().size(); 
    }
    bytes.reserve(total_size);

    bytes.insert(bytes.end(), STANDARD_HEADER.begin(), STANDARD_HEADER.end());
    
    for (const auto& chunk : chunks_m)
    {
        const auto chunk_bytes = chunk.as_bytes();
        bytes.insert(bytes.end(), chunk_bytes.begin(), chunk_bytes.end());
    }

    return bytes;
}

std::ostream& operator<<(std::ostream& os, const PNG& png)
{
    os << "PNG { length: " << png.chunks_m.size() << ", chunks: [";

    bool first = true;
    for (const auto& chunk : png.chunks())
    {
        if (!first) 
        {
            os << ", ";
        }
        os <<  chunk;
        first = false;
    }

    return os << "]}";    
}