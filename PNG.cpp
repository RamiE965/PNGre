#include "PNG.hpp"

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
    while (i + REMAINDER < bytes.size())
    {
        uint32_t data_length = (bytes[i] << 24) | (bytes[i+1] << 16) | (bytes[i+2] << 8) | bytes[i+3];

        if (i + REMAINDER + data_length > bytes.size()) 
        {
            throw std::invalid_argument("Invalid Chunk!");
        }

        std::vector<uint8_t> chunk_bytes;
        
        // length
        chunk_bytes.insert(chunk_bytes.end(), bytes.begin() + i, bytes.begin() + i + 4);
        // type
        chunk_bytes.insert(chunk_bytes.end(), bytes.begin() + i + 4, bytes.begin() + i + 8);
        // data
        chunk_bytes.insert(chunk_bytes.end(), bytes.begin() + i + 8, bytes.begin() + i + 8 + data_length);
        // crc
        chunk_bytes.insert(chunk_bytes.end(), bytes.begin() + i + 8 + data_length, bytes.begin() + i + 8 + data_length + 4);

        // Use the constructor that takes raw bytes
        chunks.push_back(Chunk(chunk_bytes));

        i += REMAINDER + data_length; 
    }

    chunks_m = chunks;
}