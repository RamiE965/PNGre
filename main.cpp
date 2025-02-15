#include "Chunk.hpp"
#include "ChunkType.hpp"
#include <iostream>
#include <cassert>

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
        auto invalid_chunk = ChunkType::fromStr("Ru1t");
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

        // Chunk tests
        RUN_TEST(test_new_chunk);
    } catch(const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    }
    
    std::cout << "All tests passed" << std::endl;
    return 0;
}