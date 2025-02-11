#include <iostream>
#include <cstdint>
#include <vector>
#include "ChunkType.cpp"

class Chunk
{
private:
    uint32_t length_m;
    ChunkType chunktype_m;
    std::vector<uint8_t> data_m;
    uint32_t crc_m;

    static uint32_t crc_table[256];
    static bool crc_table_computed;

    // crc look-up table
    static void make_crc_table() {
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

    uint32_t calculate_crc()
    {
        if (!crc_table_computed) { make_crc_table(); }

        uint32_t c = 0xffffffffL;

        const char* type_str = chunktype_m.toString().c_str();
        for (int n = 0; n < 4; n++) 
        {
            c = crc_table[(c ^ type_str[n]) & 0xff] ^ (c >> 8);
        }

        for (uint8_t byte : data_m) 
        {
            c = crc_table[(c ^ byte) & 0xff] ^ (c >> 8);
        }
        
        return c ^ 0xffffffffL;
    }
public:
    uint32_t length() const { return length_m; }
    uint32_t crc() const { return crc_m; }
    const ChunkType& chunktype() const { return chunktype_m; }

    Chunk(ChunkType chunktype, std::vector<uint8_t> data) 
    : chunktype_m(chunktype)
    , data_m(data)
    , length_m(data.size())
    {
        crc_m = calculate_crc();
    }
};

uint32_t Chunk::crc_table[256];
bool Chunk::crc_table_computed = false;

// test macro
#define RUN_TEST(test) \
    std::cout << "Running " << #test << "... "; \
    test(); \
    std::cout << "OK" << std::endl;


void test_new_chunk() {
    std::array<uint8_t, 4> bytes = {'R', 'u', 'S', 't'};  // Fixed: create array for ChunkType
    ChunkType chunktype(bytes);  // Fixed: use array constructor
    
    std::string message = "This is where your secret message will be!";
    std::vector<uint8_t> data(message.begin(), message.end());  // Fixed: proper vector construction
    
    Chunk chunk(chunktype, data);
    assert(chunk.length() == 42);
    assert(chunk.crc() == 2882656334);
}

int main()
{
    try {
        RUN_TEST(test_new_chunk);
    } catch(const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "tests passed" << std::endl;
    return 0;
}
