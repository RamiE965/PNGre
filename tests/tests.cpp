#include "test_macro.hpp"
#include "ChunkTypeTests.cpp"
#include "ChunkTests.cpp"
#include "PNGTests.cpp"

int main() {
    std::cout << "===== ChunkType tests started =====" << std::endl;
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

    std::cout << "===== Chunk tests started =====" << std::endl;
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
    
    std::cout << "===== PNG tests started =====" << std::endl;
    try {
        // PNG tests
        RUN_TEST(test_from_chunks);
        RUN_TEST(test_valid_from_bytes);
        RUN_TEST(test_invalid_header);
        RUN_TEST(test_invalid_chunk);
        RUN_TEST(test_list_chunks);
        RUN_TEST(test_chunk_by_type);
        RUN_TEST(test_append_chunk);
        RUN_TEST(test_remove_first_chunk);
        RUN_TEST(test_png_from_image_file);
        RUN_TEST(test_as_bytes);
        RUN_TEST(test_png_trait_impls);
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