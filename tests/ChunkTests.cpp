#include "test_macro.hpp"

// Chunk tests
void test_new_chunk() {
    std::array<uint8_t, 4> bytes = {'R', 'u', 'S', 't'};
    ChunkType chunktype(bytes);
    std::string message = "This is where your secret message will be!";
    std::vector<uint8_t> data(message.begin(), message.end());
    Chunk chunk(chunktype, data);
    assert(chunk.length() == 42);
    assert(chunk.crc() == 2882656334);
}

void test_chunk_length() {
    // Create test chunk
    ChunkType chunk_type = ChunkType::fromStr("RuSt");
    std::string message = "This is where your secret message will be!";
    std::vector<uint8_t> data(message.begin(), message.end());
    Chunk chunk(chunk_type, data);
    
    assert(chunk.length() == 42);
}

void test_chunk_type() {
    ChunkType chunk_type = ChunkType::fromStr("RuSt");
    std::string message = "This is where your secret message will be!";
    std::vector<uint8_t> data(message.begin(), message.end());
    Chunk chunk(chunk_type, data);
    
    assert(chunk.chunktype().toString() == "RuSt");
}

void test_chunk_string() {
    ChunkType chunk_type = ChunkType::fromStr("RuSt");
    std::string message = "This is where your secret message will be!";
    std::vector<uint8_t> data(message.begin(), message.end());
    Chunk chunk(chunk_type, data);
    
    std::string chunk_string = chunk.data_as_string();
    std::string expected_chunk_string = "This is where your secret message will be!";
    assert(chunk_string == expected_chunk_string);
}

void test_chunk_crc() {
    ChunkType chunk_type = ChunkType::fromStr("RuSt");
    std::string message = "This is where your secret message will be!";
    std::vector<uint8_t> data(message.begin(), message.end());
    Chunk chunk(chunk_type, data);
    
    assert(chunk.crc() == 2882656334);
}

void test_chunk_trait_impls() {
    ChunkType chunk_type = ChunkType::fromStr("RuSt");
    std::string message = "This is where your secret message will be!";
    std::vector<uint8_t> data(message.begin(), message.end());
    Chunk chunk(chunk_type, data);
    
    std::stringstream ss;
    ss << chunk;
    std::string chunk_string = ss.str();
    assert(!chunk_string.empty());  // too basic!
}

void test_valid_chunk_from_bytes() {
    // Prepare test data
    uint32_t data_length = 42;
    ChunkType chunk_type = ChunkType::fromStr("RuSt");
    std::string message = "This is where your secret message will be!";
    std::vector<uint8_t> data(message.begin(), message.end());
    uint32_t crc = 2882656334;

    Chunk chunk(chunk_type, data);
    auto chunk_bytes = chunk.as_bytes();
    
    assert((chunk_bytes[0] == ((data_length >> 24) & 0xFF)));
    assert((chunk_bytes[1] == ((data_length >> 16) & 0xFF)));
    assert((chunk_bytes[2] == ((data_length >> 8) & 0xFF)));
    assert((chunk_bytes[3] == (data_length & 0xFF)));

    auto type_bytes = chunk_type.bytes();
    assert(chunk_bytes[4] == type_bytes[0]);
    assert(chunk_bytes[5] == type_bytes[1]);
    assert(chunk_bytes[6] == type_bytes[2]);
    assert(chunk_bytes[7] == type_bytes[3]);

    for (size_t i = 0; i < message.length(); i++) {
        assert(chunk_bytes[8 + i] == message[i]);
    }

    assert((chunk_bytes[50] == ((crc >> 24) & 0xFF)));
    assert((chunk_bytes[51] == ((crc >> 16) & 0xFF)));
    assert((chunk_bytes[52] == ((crc >> 8) & 0xFF)));
    assert((chunk_bytes[53] == (crc & 0xFF)));
}

void test_invalid_chunk_from_bytes() {
    ChunkType chunk_type = ChunkType::fromStr("RuSt");
    std::string message = "This is where your secret message will be!";
    std::vector<uint8_t> data(message.begin(), message.end());
    
    Chunk chunk(chunk_type, data);
    auto chunk_bytes = chunk.as_bytes();
    
    uint32_t incorrect_crc = 2882656333;
    
    uint32_t actual_crc = chunk.crc();
    assert(actual_crc != incorrect_crc);
}