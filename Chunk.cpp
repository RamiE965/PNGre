#include "ChunkType.hpp"
#include "Chunk.hpp"

uint32_t Chunk::crc_table[256];
bool Chunk::crc_table_computed = false;

void Chunk::make_crc_table() {
    for (uint32_t n = 0; n < 256; n++) {
        uint32_t c = n;
        for (int k = 0; k < 8; k++) {
            if (c & 1)
                c = 0xedb88320L ^ (c >> 1);
            else
                c = c >> 1;
        }
        crc_table[n] = c;
    }
    crc_table_computed = true;
}

uint32_t Chunk::calculate_crc() {
    if (!crc_table_computed) {
        make_crc_table();
    }
    uint32_t c = 0xffffffffL;
    auto bytes = chunktype_m.bytes();
    for (int n = 0; n < 4; n++) {
        c = crc_table[(c ^ bytes[n]) & 0xff] ^ (c >> 8);
    }
    for (uint8_t byte : data_m) {
        c = crc_table[(c ^ byte) & 0xff] ^ (c >> 8);
    }
    return c ^ 0xffffffffL;
}

uint32_t Chunk::length() const {
    return length_m;
}

uint32_t Chunk::crc() const {
    return crc_m;
}

const std::vector<uint8_t>& Chunk::data() const {
    return data_m;
}

const ChunkType& Chunk::chunktype() const {
    return chunktype_m;
}

std::string Chunk::data_as_string() const {
    return std::string(data_m.begin(), data_m.end());
}

// This returns a full Chunk object and returns it as bytes
// 1. length
// 2. Chunk Type
// 3. Chunk Data
// 4. CRC
std::vector<uint8_t> Chunk::as_bytes() const {
    // PNG File Signature
    std::vector<uint8_t> bytes {137, 80, 78, 71, 13, 10, 26, 10};
    
    // Chunk length, uint8_t so we only push 8 bits at a time
    bytes.push_back((length_m >> 24) & 0xFF);
    bytes.push_back((length_m >> 16) & 0xFF);
    bytes.push_back((length_m >> 8) & 0xFF);
    bytes.push_back((length_m) & 0xFF);

    // Chunk Type
    auto type_bytes = chunktype_m.bytes(); 
    bytes.push_back(type_bytes[0]);
    bytes.push_back(type_bytes[1]);
    bytes.push_back(type_bytes[2]);
    bytes.push_back(type_bytes[3]);

    // Chunk Data
    for (auto data_chunk : data_m)
    {
        bytes.push_back(data_chunk);
    }

    // CRC
    bytes.push_back((crc_m >> 24) & 0xFF);
    bytes.push_back((crc_m >> 16) & 0xFF);
    bytes.push_back((crc_m >> 8) & 0xFF);
    bytes.push_back((crc_m) & 0xFF);

    return bytes;
}

Chunk::Chunk(ChunkType chunktype, std::vector<uint8_t> data)
    : chunktype_m(chunktype)
    , data_m(data)
    , length_m(data.size())
{
    crc_m = calculate_crc();
}

Chunk::Chunk(const std::vector<uint8_t>& bytes)
    : chunktype_m({bytes[4], bytes[5], bytes[6], bytes[7]})
{
    if (bytes.size() < 8) { throw std::invalid_argument("Not enough bytes for valid chunk!"); }

    length_m = (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3];

    if (bytes.size() < 8 + length_m + 4) {
        throw std::invalid_argument("Not enough bytes for chunk data and CRC!");
    }

    if (!chunktype_m.is_valid()) {throw std::invalid_argument("Invalid Chunktype!"); }

    data_m.assign(bytes.begin() + 8, bytes.begin() + 8 + length_m);

    uint32_t received_crc = (bytes[8 + length_m] << 24) | 
                            (bytes[8 + length_m + 1] << 16) | 
                            (bytes[8 + length_m + 2] << 8) | 
                            bytes[8 + length_m + 3];

    crc_m = calculate_crc();
    if (crc_m != received_crc) {
        throw std::invalid_argument("CRC mismatch");
    }
}

std::ostream& operator<<(std::ostream& os, const Chunk& chunk) 
{
    return os << "Chunk { length: " << chunk.length() 
              << ", type: " << chunk.chunktype().toString()
              << ", data size: " << chunk.data().size() 
              << ", crc: " << chunk.crc() << " }";
}