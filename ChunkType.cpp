#include "ChunkType.hpp"
#include <stdexcept>

ChunkType::ChunkType(const std::array<uint8_t, 4>& bytes) {
    type = bytes;
}

std::array<uint8_t, 4> ChunkType::bytes() const {
    return type;
}

bool ChunkType::is_valid() const {
    bool valid = true;
    for (auto byte : type) {
        if (!((byte >= 65 && byte < 91) || (byte >= 97 && byte < 123))) {
            return false;
        }
    }
    return valid && is_reserved_bit_valid();
}

bool ChunkType::is_critical() const {
    uint8_t first_byte = type[0];
    auto critical_bit = ((first_byte >> 5) & 0x01);
    return !critical_bit;
}

bool ChunkType::is_public() const {
    uint8_t second_byte = type[1];
    auto public_bit = ((second_byte >> 5) & 0x01);
    return !public_bit;
}

bool ChunkType::is_reserved_bit_valid() const {
    uint8_t third_byte = type[2];
    auto reserved_bit = ((third_byte >> 5) & 0x01);
    return !reserved_bit;
}

bool ChunkType::is_safe_to_copy() const {
    uint8_t fourth_byte = type[3];
    auto safe_to_copy_bit = ((fourth_byte >> 5) & 0x01);
    return safe_to_copy_bit;
}

ChunkType ChunkType::fromStr(std::string_view input) {
    if (input.size() != 4) {
        throw std::invalid_argument("Your string must contain 4 characters!");
    }

    std::array<uint8_t, 4> input_cast;

    for (char c : input) {
        if (!((c >= 65 && c < 91) || (c >= 97 && c < 123))) {
            throw std::invalid_argument("bad");
        }
    }

    for (int i = 0; i < 4; i++) {
        input_cast[i] = input[i];
    }

    return ChunkType(input_cast);
}

std::string ChunkType::toString() const {
    return std::string(type.begin(), type.end());
}

bool ChunkType::operator==(const ChunkType& other) const {
    return type == other.type;
}