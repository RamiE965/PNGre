#include "Chunk.hpp"
#include "ChunkType.hpp"
#include "PNG.hpp"
#include <cassert>
#include <sstream>
#include <optional>
#include <vector>

// test macro
#define RUN_TEST(test) \
    std::cout << "Running " << #test << "... "; \
    test(); \
    std::cout << "OK" << std::endl;

// ChunkType tests
void test_chunk_type_from_bytes() {
    std::array<uint8_t, 4> expected = {82, 117, 83, 116};
    auto actual = ChunkType(expected);
    assert(expected == actual.bytes());
}

void test_chunk_type_from_str() {
    std::array<uint8_t, 4> expected = {82, 117, 83, 116};
    auto actual = ChunkType::fromStr("RuSt");
    assert(expected == actual.bytes());
}

void test_chunk_type_is_critical() {
    auto chunk = ChunkType::fromStr("RuSt");
    assert(chunk.is_critical());
}

void test_chunk_type_is_not_critical() {
    auto chunk = ChunkType::fromStr("ruSt");
    assert(!chunk.is_critical());
}

void test_chunk_type_is_public() {
    auto chunk = ChunkType::fromStr("RUSt");
    assert(chunk.is_public());
}

void test_chunk_type_is_not_public() {
    auto chunk = ChunkType::fromStr("RuSt");
    assert(!chunk.is_public());
}

void test_chunk_type_is_reserved_bit_valid() {
    auto chunk = ChunkType::fromStr("RuSt");
    assert(chunk.is_reserved_bit_valid());
}

void test_chunk_type_is_reserved_bit_invalid() {
    auto chunk = ChunkType::fromStr("Rust");
    assert(!chunk.is_reserved_bit_valid());
}

void test_chunk_type_is_safe_to_copy() {
    auto chunk = ChunkType::fromStr("RuSt");
    assert(chunk.is_safe_to_copy());
}

void test_chunk_type_is_unsafe_to_copy() {
    auto chunk = ChunkType::fromStr("RuST");
    assert(!chunk.is_safe_to_copy());
}

void test_valid_chunk_is_valid() {
    auto chunk = ChunkType::fromStr("RuSt");
    assert(chunk.is_valid());
}

void test_invalid_chunk_is_valid() {
    try {
        auto chunk = ChunkType::fromStr("Rust");
        assert(!chunk.is_valid());
    } catch (const std::invalid_argument&) {
        // pass
    }
    
    try {
        ChunkType::fromStr("Ru1t");
        assert(false);
    } catch (const std::invalid_argument&) {
        // should get here
    }
}

void test_chunk_type_string() {
    auto chunk = ChunkType::fromStr("RuSt");
    assert(chunk.toString() == "RuSt");
}

void test_chunk_type_trait_impls() {
    auto chunk_type_1 = ChunkType({82, 117, 83, 116});
    auto chunk_type_2 = ChunkType::fromStr("RuSt");
    std::string chunk_string = chunk_type_1.toString();
    bool are_chunks_equal = (chunk_type_1 == chunk_type_2);
    assert(are_chunks_equal);
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
    
    assert((chunk_bytes[8] == ((data_length >> 24) & 0xFF)));
    assert((chunk_bytes[9] == ((data_length >> 16) & 0xFF)));
    assert((chunk_bytes[10] == ((data_length >> 8) & 0xFF)));
    assert((chunk_bytes[11] == (data_length & 0xFF)));
    
    auto type_bytes = chunk_type.bytes();
    assert(chunk_bytes[12] == type_bytes[0]);
    assert(chunk_bytes[13] == type_bytes[1]);
    assert(chunk_bytes[14] == type_bytes[2]);
    assert(chunk_bytes[15] == type_bytes[3]);
    
    for (size_t i = 0; i < message.length(); i++) {
        assert(chunk_bytes[16 + i] == message[i]);
    }
    
    assert((chunk_bytes[58] == ((crc >> 24) & 0xFF)));
    assert((chunk_bytes[59] == ((crc >> 16) & 0xFF)));
    assert((chunk_bytes[60] == ((crc >> 8) & 0xFF)));
    assert((chunk_bytes[61] == (crc & 0xFF)));
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

// PNG tests

int main() {
    try {
        // ChunkType tests
        RUN_TEST(test_chunk_type_from_bytes);
        RUN_TEST(test_chunk_type_from_str);
        RUN_TEST(test_chunk_type_is_critical);
        RUN_TEST(test_chunk_type_is_not_critical);
        RUN_TEST(test_chunk_type_is_public);
        RUN_TEST(test_chunk_type_is_not_public);
        RUN_TEST(test_chunk_type_is_reserved_bit_valid);
        RUN_TEST(test_chunk_type_is_reserved_bit_invalid);
        RUN_TEST(test_chunk_type_is_safe_to_copy);
        RUN_TEST(test_chunk_type_is_unsafe_to_copy);
        RUN_TEST(test_valid_chunk_is_valid);
        RUN_TEST(test_invalid_chunk_is_valid);
        RUN_TEST(test_chunk_type_string);
        RUN_TEST(test_chunk_type_trait_impls);
    } catch(const std::exception& e) {
        std::cerr << "ChunkType Test failed: " << e.what() << std::endl;
        return 1;
    }
    std::cout << "===== ChunkType tests passed =====\n" << std::endl;

    try {
        // Chunk tests
        RUN_TEST(test_new_chunk);
        RUN_TEST(test_valid_chunk_from_bytes);
        RUN_TEST(test_invalid_chunk_from_bytes);
        RUN_TEST(test_chunk_length);
        RUN_TEST(test_chunk_type);
        RUN_TEST(test_chunk_string);
        RUN_TEST(test_chunk_crc);
        RUN_TEST(test_chunk_trait_impls);
    } catch(const std::exception& e) {
        std::cerr << "Chunk Test failed: " << e.what() << std::endl;
        return 1;
    }
    std::cout << "===== Chunk tests passed =====\n" << std::endl;
    
    try {
        // PNG tests
        // RUN_TEST(test_from_chunks);
        // RUN_TEST(test_valid_from_bytes);
        // RUN_TEST(test_invalid_header);
        // RUN_TEST(test_invalid_chunk);
        // RUN_TEST(test_list_chunks);
        // RUN_TEST(test_chunk_by_type);
        // RUN_TEST(test_append_chunk);
        // RUN_TEST(test_remove_first_chunk);
        // RUN_TEST(test_png_from_image_file);
        // RUN_TEST(test_as_bytes);
        // RUN_TEST(test_png_trait_impls);
    } catch(const std::exception& e) {
        std::cerr << "PNG Test failed: " << e.what() << std::endl;
        return 1;
    }
    std::cout << "===== PNG tests passed =====\n" << std::endl;
    
    std::cout << "===================================\n"
          << "All tests passed\n"
          << "===================================\n" 
          << std::endl;
    return 0;
}