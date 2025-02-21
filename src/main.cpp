#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "ChunkType.hpp"
#include "Chunk.hpp"
#include "PNG.hpp"

PNG generate_png(std::string_view path)
{
    // attempt to open the file in binary mode
    std::ifstream image_file(path, std::ios::binary);
    std::vector<uint8_t> bytes; 

    if (image_file.good())
    {
        // read data into vector
        bytes = std::vector<uint8_t>(
            std::istreambuf_iterator<char>(image_file),
            std::istreambuf_iterator<char>()
        );
        image_file.close();

        // construct PNG object from bytes
        return PNG(bytes);
    }
    else 
    {
        throw std::invalid_argument("There was an issue reading the PNG file!");
    }   
}

/* 
* input[0]: encode <command>
* input[1]: <source_file.png>
* input[2]: <chunktype>
* input[3]: <message>
* input[4]: <output_file.png> [OPTIONAL]
*
* encodes a message into a PNG file 
*/
void handle_encode(std::vector<std::string_view> input)
{
    if (input.size() < 3)
    {
        throw std::invalid_argument("Invalid number of arguments for encode. Usability: ./pngre encode ./<image_name>.png <chunktype> <Message>");
    }

    // construct PNG object
    PNG image = generate_png(input[1]);

    // validate chunktype, and append new chunk
    auto chunktype = ChunkType::fromStr(input[2]);
    if (chunktype.is_valid())
    {
        // todo: validate data
        std::vector<uint8_t> data(input[3].begin(), input[3].end());
        auto chunk = Chunk(chunktype, data);
        image.append_chunk(chunk);
    }
    else 
    {
        throw std::invalid_argument("Invalid ChunkType!");
    }

    std::ofstream output_file(input[1], std::ios::binary);
    auto bytes = image.as_bytes();
    output_file.write(reinterpret_cast<const char*>(bytes.data()), bytes.size());
    output_file.close();

    std::cout << "Encoded: " << input[3] << " into " << input[2] << " file successfully!" << std::endl;
}

/* 
* input[0]: decode <command>
* input[1]: <source_file.png>
* input[2]: <chunktype>
*
* decodes a message from a PNG file
*/
void handle_decode(std::vector<std::string_view> input)
{
    if (input.size() < 3)
    {
        throw std::invalid_argument("Invalid number of arguments for decode. Usability: ./pngre decode ./<image_name>.png <chunktype>");
    }

    // construct PNG object
    PNG image = generate_png(input[1]);

    // validate chunktype
    auto chunktype = ChunkType::fromStr(input[2]);
    std::optional<Chunk> matching_chunk;

    if (chunktype.is_valid()) 
    {
        // get first matching chunk by type
        matching_chunk = image.chunk_by_type(chunktype);   
    }
    else
    {
        throw std::invalid_argument("Invalid ChunkType!");
    }
    
    if (matching_chunk.has_value())
    {
        std::cout << "Decoded: " << matching_chunk.value().data_as_string() << std::endl;
    }
    else
    {
        std::cout << "No message matching " << input[2] << " ChunkType in provided image." << std::endl;
    }
}

void handle_remove(std::vector<std::string_view> input)
{
    std::cout << input[0] << std::endl;
}

void handle_print(std::vector<std::string_view> input)
{
    std::cout << input[0] << std::endl;
}

int main(int argc, char** argv) 
{
    if (argc < 2 || argc > 6)
    {
        std::cout << "Usability: ./pngre encode ./<image_name>.png <chunktype> <Message>\n" << "Type -h or --help for help" << std::endl;
        return 0;
    }

    // handle commands: encode, decode, remove, print, help: -h or --help
    std::string_view command = argv[1];

    std::vector<std::string_view> inputArr;

    for (int i = 1; i < argc; i++)
    {
        inputArr.push_back(argv[i]);
    }

    if (command == "encode")
    {
        handle_encode(inputArr);
    }
    else if (command == "decode")
    {
        handle_decode(inputArr);
    }
    else if (command == "remove")
    {
        handle_remove(inputArr);
    }
    else if (command == "print")
    {
        handle_print(inputArr);
    }
    else if (command == "-h" || command == "--help")
    {
        std::cout << "TODO" << std::endl;
    }
    else 
    {
        std::cout << "Usability: ./pngre encode ./<image_name>.png <chunktype> <Message>\n" << "Type -h or --help for help" << std::endl;
        return 0;
    }
}