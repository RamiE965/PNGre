/*
Write a ChunkType class with your implementation of PNG chunk types.

Implement a constructor that takes an array of 4 bytes (uint8_t[4]) and validates them.
Implement a constructor or static factory method that creates a ChunkType from a string.
Implement operator << for stream output or a toString() method.
Implement operator == for equality comparison

Required methods:

std::array<uint8_t, 4> bytes() const
bool isValid() const
bool isCritical() const
bool isPublic() const
bool isReservedBitValid() const
bool isSafeToCopy() const
*/


#include <cstdint>
#include <vector>
#include <iostream>
#include <cassert>
#include <array>
#include <string>

class ChunkType 
{
private:
    std::array<uint8_t, 4> type;
public:
    explicit ChunkType(const std::array<uint8_t, 4>& bytes)
    {
        type = bytes;
    }

    std::array<uint8_t, 4> bytes() const
    {
        return type;
    }

    // Validate lower/uppercase alphabet ASCII character
    bool is_valid() const
    {
        bool valid = true;
        for (auto byte : type)
        {
            if (!((byte >= 65 && byte < 91) || (byte >= 97 && byte < 123)))
            {
                return false;
            }
        }
        return valid && is_reserved_bit_valid();
    }
    
    // Ancillary bit: bit 5 of first byte
    // 0 (uppercase) = critical, 1 (lowercase) = ancillary. 
    bool is_critical() const
    {
        uint8_t first_byte = type[0];
        auto critical_bit = ((first_byte >> 5) & 0x01);
        // if bit = 1 return false and vice versa
        return !critical_bit;
    }

    // Private bit: 5 bits of second byte
    // 0 (uppercase) = public, 1 (lowercase) = private
    bool is_public() const 
    {
        uint8_t second_byte = type[1];
        auto public_bit = ((second_byte >> 5) & 0x01);
        return !public_bit;
    }

    // Reserved bit: bit 5 of third byte
    // valid if 0
    bool is_reserved_bit_valid() const
    {
        uint8_t third_byte = type[2];
        auto reserved_bit = ((third_byte >> 5) & 0x01);
        return !reserved_bit;
    }

    // Safe-copy-bit: bit 5 of fourth byte
    // 0 (uppercase) = unsafe to copy, 1 (lowercase) = safe to copy
    bool is_safe_to_copy() const
    {
        uint8_t fourth_byte = type[3];
        auto safe_to_copy_bit = ((fourth_byte >> 5) & 0x01);
        return safe_to_copy_bit; 
    }

    static ChunkType fromStr(std::string_view input)
    {
        if (input.size() != 4) { throw std::invalid_argument("Your string must contain 4 characters!"); }

        std::array<uint8_t,4> input_cast;

        for (char c : input) {
            if (!((c >= 65 && c < 91) || (c >= 97 && c < 123))) 
            {
                throw std::invalid_argument("bad");
            }
        }

        for (int i = 0; i < 4; i++)
        {
            input_cast[i] = input[i];
        }

        ChunkType result(input_cast);

        return result;
    }

    std::string toString() const 
    {
        return std::string(type.begin(), type.end());
    }

    bool operator==(const ChunkType& other) const
    {
        return type == other.type;
    }
};


// test macro
#define RUN_TEST(test) \
    std::cout << "Running " << #test << "... "; \
    test(); \
    std::cout << "OK" << std::endl;

// basic unit tests
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


int main()
{
    try {
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
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    }
    
    std::cout << "tests passed" << std::endl;
    return 0;
}