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

class ChunkType 
{
private:
    std::array<uint8_t, 4> type;
public:
    explicit ChunkType(const std::array<uint8_t, 4>& bytes)
    {
        // validate lower or uppercase alphabet ascii character
        for (auto byte : bytes)
        {
            if (!((byte >= 65 && byte < 91) || (byte >= 97 && byte < 123)))
            {
                throw std::invalid_argument("Invalid Type, each byte must represent an alphabetic ASCII character!");
            }
        }
        type = bytes;
    }

    std::array<uint8_t, 4> const bytes() const
    {
        return type;
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

    static ChunkType fromStr(std::string_view input)
    {
        if (input.size() != 4) { throw std::invalid_argument("Your string must contain 4 characters!"); }

        std::array<uint8_t,4> input_cast;

        for (int i = 0; i < 4; i++)
        {
            input_cast[i] = input[i];
        }

        ChunkType result(input_cast);

        return result;
    }
};

int main()
{
    {
        std::array<uint8_t, 4> expected = {82, 117, 83, 116};
        auto actual = ChunkType(expected);

        assert(expected == actual.bytes());
    }

    {
        std::array<uint8_t, 4> expected = {82, 117, 83, 116};
        auto actual = ChunkType::fromStr("RuSt");
        assert(expected == actual.bytes());
    }
    
    std::cout << "test passed" << std::endl;
    return 0;
}