#pragma once
#include <cstdint>
#include <array>
#include <string>
#include <string_view>

class ChunkType {
private:
    std::array<uint8_t, 4> type;

public:
    explicit ChunkType(const std::array<uint8_t, 4>& bytes);
    
    std::array<uint8_t, 4> bytes() const;  // Added const
    
    // Validate lower/uppercase alphabet ASCII character
    bool is_valid() const;  // Added const
    
    // Ancillary bit: bit 5 of first byte
    // 0 (uppercase) = critical, 1 (lowercase) = ancillary
    bool is_critical() const;  // Added const
    
    // Private bit: 5 bits of second byte
    // 0 (uppercase) = public, 1 (lowercase) = private
    bool is_public() const;  // Added const
    
    // Reserved bit: bit 5 of third byte
    // valid if 0
    bool is_reserved_bit_valid() const;  // Added const
    
    // Safe-copy-bit: bit 5 of fourth byte
    // 0 (uppercase) = unsafe to copy, 1 (lowercase) = safe to copy
    bool is_safe_to_copy() const;  // Added const
    
    static ChunkType fromStr(std::string_view input);
    std::string toString() const;  // Added const
    bool operator==(const ChunkType& other) const;  // Added const
};