#include "test_macro.hpp"

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